#include "realtime.h"
#include <iostream>
#include "utils/scenedata.h"
#include "settings.h"
#include <glm/gtc/type_ptr.hpp>

#define f(i,a,b) for (int i = a; i < b; i++)
using namespace glm;
using namespace std;

void Realtime::a_createTexture(GLuint &texture,
	GLenum format, GLenum internalFormat, GLenum dataType,
    GLenum filter, GLenum wrapMode
) {
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, filter);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, filter);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, wrapMode);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, wrapMode);

	glTexImage2D(GL_TEXTURE_2D, 0, format, m_fbo_width, m_fbo_height, 
		0, internalFormat, dataType, nullptr);
	glBindTexture(GL_TEXTURE_2D, 0);
}

void Realtime::a_deleteFBOandTextures() {
    glBindTexture(GL_TEXTURE_2D, 0);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	glDeleteTextures(1, &a_fbo_default_ambientColor);
	glDeleteTextures(1, &a_fbo_default_depth);
	glDeleteTextures(1, &a_fbo_default_normalVS);
	glDeleteTextures(1, &a_fbo_default_diffuseSpecular);
	glDeleteTextures(1, &a_fbo_default_posVS);
	glDeleteTextures(1, &a_fbo_AO_value);
	glDeleteTextures(1, &a_fbo_blur_value);
	glDeleteTextures(1, &a_fbo_normalOutput_color);

	glDeleteFramebuffers(1, &a_fbo_default);
	glDeleteFramebuffers(1, &a_fbo_AO);
	glDeleteFramebuffers(1, &a_fbo_blur);
	glDeleteFramebuffers(1, &a_fbo_normalOutput);
}

void Realtime::a_deleteOpenGLVars() {
	for (const auto & [key, value] : t_vbo) 
		glDeleteBuffers(1, &value);
	for (const auto & [key, value] : t_vao) 
		glDeleteVertexArrays(1, &value);

	glDeleteVertexArrays(1, &a_blitVAO);
	glDeleteBuffers(1, &a_blitVBO);

	a_deleteFBOandTextures();
	glDeleteTextures(1, &a_fbo_AO_noiseTexture);

	glDeleteProgram(a_shader_default);
	glDeleteProgram(a_shader_postprocessing);
	glDeleteProgram(a_shader_ambientCombine);
	glDeleteProgram(a_shader_blur);
	glDeleteProgram(a_shader_ambientOcclusion);

	t_shapeMap.clear(); // effectively drops references to unique ptr 
}


void Realtime::a_drawScene(const GLuint &shader) {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glUseProgram(shader);
    a_loadSceneGlobalData(shader);
    a_loadCameraData(shader);
    a_loadLightData(shader);

	unordered_map<int, vector<int>> buckets;
	int n = t_renderData.shapes.size();
	f(i,0,n) {
        int key = t_getKey(t_camera.getPosition(), t_renderData.shapes[i]);
		if (!buckets.count(key)) buckets[key] = vector<int>();
		buckets[key].push_back(i);
	}

    for (const auto &[mapKey, shape_ptr] : t_shapeMap) {
		if (buckets.count(mapKey)) {
            GLuint vao = t_vao[mapKey];
			glBindVertexArray(vao);

			for (int& index : buckets[mapKey]) {
                a_loadShape(shader,index);
				glDrawArrays(GL_TRIANGLES, 0, shape_ptr->shapeCount());
			}
		}
	}

	glBindVertexArray(0);
	glUseProgram(0);
}
