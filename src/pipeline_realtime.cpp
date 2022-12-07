#include "realtime.h"
#include <iostream>
#include "utils/scenedata.h"
#include "utils/shaderloader.h"
#include "settings.h"
#include <glm/gtc/type_ptr.hpp>

#define f(i,a,b) for (int i = a; i < b; i++)
using namespace glm;
using namespace std;

void Realtime::a_initFBOs() {
	// create default fbo
	defaultFBO: {
		a_createTexture(a_fbo_default_diffuseSpecular, GL_RGBA, GL_RGBA, GL_UNSIGNED_BYTE);
		a_createTexture(a_fbo_default_ambientColor, GL_RGBA, GL_RGBA, GL_UNSIGNED_BYTE);
		a_createTexture(a_fbo_default_posVS, GL_RGBA16F, GL_RGBA, GL_FLOAT);
		a_createTexture(a_fbo_default_normalVS, GL_RGBA16F, GL_RGBA, GL_FLOAT);
		a_createTexture(a_fbo_default_depth, GL_DEPTH_COMPONENT24, GL_DEPTH_COMPONENT, GL_FLOAT, GL_LINEAR, GL_CLAMP_TO_EDGE);

		glGenFramebuffers(1, &a_fbo_default);
		glBindFramebuffer(GL_FRAMEBUFFER, a_fbo_default);
		glViewport(0,0,m_fbo_width, m_fbo_height);

		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, a_fbo_default_diffuseSpecular, 0);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, GL_TEXTURE_2D, a_fbo_default_ambientColor, 0);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT2, GL_TEXTURE_2D, a_fbo_default_posVS, 0);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT3, GL_TEXTURE_2D, a_fbo_default_normalVS, 0);
		glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, a_fbo_default_depth, 0);

		const GLenum buffers[] = {GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1, 
			GL_COLOR_ATTACHMENT2, GL_COLOR_ATTACHMENT3};
		glDrawBuffers(4, buffers);

		GLenum err = glCheckFramebufferStatus(GL_FRAMEBUFFER);
		assert(err == GL_FRAMEBUFFER_COMPLETE);

		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}

	ambientOcclusionFBO: {
		// create noise texture
		a_createTexture(a_fbo_AO_value, GL_RED, GL_RED, GL_UNSIGNED_BYTE);

		glGenFramebuffers(1, &a_fbo_AO);
		glBindFramebuffer(GL_FRAMEBUFFER, a_fbo_AO);
		glViewport(0,0,m_fbo_width, m_fbo_height);

		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, a_fbo_AO_value, 0);

		GLenum err = glCheckFramebufferStatus(GL_FRAMEBUFFER);
		assert(err == GL_FRAMEBUFFER_COMPLETE);

		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}

	blurFBO: {
		a_createTexture(a_fbo_blur_value, GL_RED, GL_RED, GL_UNSIGNED_BYTE);

		glGenFramebuffers(1, &a_fbo_blur);
		glBindFramebuffer(GL_FRAMEBUFFER, a_fbo_blur);
		glViewport(0,0,m_fbo_width, m_fbo_height);

		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, a_fbo_blur_value, 0);

		GLenum err = glCheckFramebufferStatus(GL_FRAMEBUFFER);
		assert(err == GL_FRAMEBUFFER_COMPLETE);

		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}

	normalOutputFBO: {
        a_createTexture(a_fbo_normalOutput_color, GL_RGBA, GL_RGBA, GL_UNSIGNED_BYTE);

		glGenFramebuffers(1, &a_fbo_normalOutput);
		glBindFramebuffer(GL_FRAMEBUFFER, a_fbo_normalOutput);
		glViewport(0,0,m_fbo_width, m_fbo_height);

		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, a_fbo_normalOutput_color, 0);

		GLenum err = glCheckFramebufferStatus(GL_FRAMEBUFFER);
		assert(err == GL_FRAMEBUFFER_COMPLETE);

		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}
}

void Realtime::a_pipelineInit() {
	a_initFBOs();

	// load shaders in
	a_shader_default = ShaderLoader::createShaderProgram(":/resources/shaders/default.vert", 
		":/resources/shaders/default.frag");
	a_shader_postprocessing = ShaderLoader::createShaderProgram(":/resources/shaders/postprocessing.vert", 
		":/resources/shaders/postprocessing.frag");
	a_shader_ambientCombine = ShaderLoader::createShaderProgram(":/resources/shaders/postprocessing.vert", 
		":/resources/shaders/ambientLighting.frag");
	a_shader_ambientOcclusion = ShaderLoader::createShaderProgram(":/resources/shaders/postprocessing.vert", 
		":/resources/shaders/ambientOcclusion.frag");
	a_shader_blur = ShaderLoader::createShaderProgram(":/resources/shaders/postprocessing.vert", 
		":/resources/shaders/blur.frag");
}

void Realtime::a_pipelineRun() {
    glViewport(0, 0, m_fbo_width, m_fbo_height);

	drawToDefault: {
		glBindFramebuffer(GL_FRAMEBUFFER, a_fbo_default);
		a_drawScene(a_shader_default);
	}

	if (settings.extraCredit3) {
		drawToAmbientOcclusion: {
			glUseProgram(a_shader_ambientOcclusion);
			a_loadSSAOData(a_shader_ambientOcclusion);
			a_blit(a_fbo_AO);
		}

		blurAmbientOcclusion: {
			glUseProgram(a_shader_blur);

			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, a_fbo_AO_value);
			glUniform1i(glGetUniformLocation(a_shader_blur, "_MainTex"), 0);

			glUniform4fv(glGetUniformLocation(a_shader_blur, "_MainTex_TexelSize"), 1,
				glm::value_ptr(vec4(1.0f/size().width(), 1.0f/size().height(), size().width(), size().height())));

			glBindFramebuffer(GL_FRAMEBUFFER, a_fbo_blur);
			glViewport(0, 0, m_fbo_width, m_fbo_height);
			a_blit(a_fbo_blur);
		}
	}

	lightingPass: {
		glUseProgram(a_shader_ambientCombine);

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, a_fbo_default_diffuseSpecular);
		glUniform1i(glGetUniformLocation(a_shader_ambientCombine, "diffuseAndSpecularColor"), 0);

		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, a_fbo_default_ambientColor);
		glUniform1i(glGetUniformLocation(a_shader_ambientCombine, "ambientColor"), 1);

		glActiveTexture(GL_TEXTURE2);
		glBindTexture(GL_TEXTURE_2D, a_fbo_blur_value);
		glUniform1i(glGetUniformLocation(a_shader_ambientCombine, "ambientOcclusion"), 2);

		glUniform1i(glGetUniformLocation(a_shader_ambientCombine, "applyAmbientOcclusion"), 
			settings.extraCredit3);

		glBindFramebuffer(GL_FRAMEBUFFER, a_fbo_normalOutput);
    	glViewport(0, 0, m_fbo_width, m_fbo_height);

        a_blit(a_fbo_normalOutput);
	}

	postprocessing: {
		glUseProgram(a_shader_postprocessing);

		glUniform4fv(glGetUniformLocation(a_shader_postprocessing, "_MainTex_TexelSize"), 1,
			glm::value_ptr(vec4(1.0f/size().width(), 1.0f/size().height(), size().width(), size().height())));

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, a_fbo_normalOutput_color);
		glUniform1i(glGetUniformLocation(a_shader_postprocessing, "_MainTex"), 0);

		glUniform1i(glGetUniformLocation(a_shader_postprocessing, "applyGrayscale"), 
			settings.perPixelFilter);
		glUniform1i(glGetUniformLocation(a_shader_postprocessing, "applySharpen"), 
			settings.kernelBasedFilter);

    	glViewport(0, 0, size().width() * m_devicePixelRatio, size().height() * m_devicePixelRatio);
		// draw to screen
        a_blit(t_fbo_main);
	}

	unbinding: {
		glActiveTexture(GL_TEXTURE0); glBindTexture(GL_TEXTURE_2D, 0);
		glActiveTexture(GL_TEXTURE1); glBindTexture(GL_TEXTURE_2D, 0);
		glActiveTexture(GL_TEXTURE2); glBindTexture(GL_TEXTURE_2D, 0);
		glActiveTexture(GL_TEXTURE3); glBindTexture(GL_TEXTURE_2D, 0);
		glUseProgram(0);
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}
}
