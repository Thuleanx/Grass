#include "realtime.h"
#include <iostream>
#include "utils/scenedata.h"
#include "settings.h"
#include <glm/gtc/type_ptr.hpp>
#include <time.h>
#include <iomanip>
#include <random>
#include <chrono>

#define f(i,a,b) for (int i = a; i < b; i++)
using namespace glm;
using namespace std;

mt19937 rng(chrono::steady_clock::now().time_since_epoch().count());

const int ssaoKernelSize = 64;
const int NOISE_SZ = 8;
vector<vec3> ssaoKernelTS;
vector<vec3> ssaoNoiseTexture;

inline float ssao_random() {return uniform_real_distribution<float>(0,1)(rng); }
void Realtime::a_initSSAOKernel() {
	f(i,0,ssaoKernelSize) {
		do {
			vec3 sample(ssao_random() * 2 - 1, ssao_random() * 2 - 1, ssao_random());
			if (dot(sample,sample) <= 1) { // make sure they are in sphere
				// pull closer to origin * lerp(0.1f, 1.0f, float(i)/64)
				ssaoKernelTS.push_back(sample); // pull point closer to origin (so nonuniform distribution)
				break;
			}
		} while (true);
	}
	f(i,0,NOISE_SZ*NOISE_SZ) {
		ssaoNoiseTexture.push_back(vec3(
			ssao_random() * 2 - 1,
			ssao_random() * 2 - 1,
			0
		));
	}

	glGenTextures(1, &a_fbo_AO_noiseTexture);
	glBindTexture(GL_TEXTURE_2D, a_fbo_AO_noiseTexture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, NOISE_SZ, NOISE_SZ, 0, GL_RGB, GL_FLOAT, &ssaoNoiseTexture[0]);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);  
}

void Realtime::a_loadSSAOData(const GLuint &shader) {
	f(i,0,ssaoKernelSize) {
		string prop = "kernel[" + to_string(i) + "]";
		glUniform3fv(glGetUniformLocation(shader, prop.c_str()), 
			1, glm::value_ptr(ssaoKernelTS[i]));
	}


	glUniform4fv(glGetUniformLocation(shader, "_TexelSize"), 1,
		glm::value_ptr(vec4(1.0f/size().width(), 1.0f/size().height(), size().width(), size().height())));

	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, a_fbo_default_depth);
	glUniform1i(glGetUniformLocation(shader, "_depthTex"), 1);

	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_2D, a_fbo_default_normalVS);
	glUniform1i(glGetUniformLocation(shader, "_normalVS"), 2);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, a_fbo_default_posVS);
	glUniform1i(glGetUniformLocation(shader, "_posVS"), 0);

	glActiveTexture(GL_TEXTURE3);
	glBindTexture(GL_TEXTURE_2D, a_fbo_AO_noiseTexture);
	glUniform1i(glGetUniformLocation(shader, "_noiseTexture"), 3);

	glUniform4fv(glGetUniformLocation(shader, "_noiseTexture_TexelSize"), 1,
		glm::value_ptr(vec4(1.0f/NOISE_SZ, 1.0f/NOISE_SZ, NOISE_SZ, NOISE_SZ)));

	glUniform1f(glGetUniformLocation(shader, "zNear"), settings.nearPlane);
	glUniform1f(glGetUniformLocation(shader, "zFar"), settings.farPlane);


	glUniform2fv(glGetUniformLocation(shader, "nearPlaneSz"), 
		1, glm::value_ptr(t_camera.getNearPlaneSize()));
	glUniformMatrix4fv(glGetUniformLocation(shader, "projectionMatrix"), 1, GL_FALSE, 
		glm::value_ptr( t_camera.getProjectionMatrix() ));
}

void Realtime::a_loadSceneGlobalData(const GLuint &shader) {
	glUniform1f(glGetUniformLocation(shader, "ka"), t_renderData.globalData.ka);
	glUniform1f(glGetUniformLocation(shader, "kd"), t_renderData.globalData.kd);
	glUniform1f(glGetUniformLocation(shader, "ks"), t_renderData.globalData.ks);
}

void Realtime::a_loadCameraData(const GLuint &shader) {
	glm::vec4 camPos = t_camera.getPosition();
	glUniform4fv(glGetUniformLocation(shader, "camPos"), 1, glm::value_ptr(camPos));

	GLint viewLocation = glGetUniformLocation(shader, "viewMatrix");
	glUniformMatrix4fv(viewLocation, 1, GL_FALSE, glm::value_ptr( t_camera.getViewMatrix() ));

	GLint projectionLocation = glGetUniformLocation(shader, "projMatrix");
	glUniformMatrix4fv(projectionLocation, 1, GL_FALSE, glm::value_ptr( t_camera.getProjectionMatrix() ));
}

void Realtime::a_loadLightData(const GLuint &shader) {
	int lightIndex[3] = {0,0,0};
	auto processLight = [&](LightType t, vec4 &pos, vec3 &function, vec4& dir, vec4 &color, float penumbra, float angle) {
		pos[3] = 1;
		dir[3] = 0;

		string prefixLightType = "";
		int i = lightIndex[(int) t];
		if (t == LightType::LIGHT_POINT) prefixLightType = "point";
		if (t == LightType::LIGHT_DIRECTIONAL) prefixLightType = "dir";
		if (t == LightType::LIGHT_SPOT) prefixLightType = "spot";

		if (t == LightType::LIGHT_SPOT || t == LightType::LIGHT_POINT) {
			string prop = prefixLightType + "Lights[" + to_string(i) + "].pos";
			glUniform4fv(glGetUniformLocation(shader, prop.c_str()), 
				1, glm::value_ptr(pos));
		}
		if (t == LightType::LIGHT_DIRECTIONAL || t == LightType::LIGHT_SPOT) {
			string prop = prefixLightType + "Lights[" + to_string(i) + "].dir";
			glUniform4fv(glGetUniformLocation(shader, prop.c_str()), 
				1, glm::value_ptr(dir));
		}
		if (t == LightType::LIGHT_POINT || t == LightType::LIGHT_SPOT) {
			string prop = prefixLightType + "Lights[" + to_string(i) + "].function";
			glUniform3fv(glGetUniformLocation(shader, prop.c_str()), 
				1, glm::value_ptr(function));
		}
		if (t == LightType::LIGHT_SPOT) {
			string prop = prefixLightType + "Lights[" + to_string(i) + "].penumbra";
			glUniform1f(glGetUniformLocation(shader, prop.c_str()), penumbra);
		}
		if (t == LightType::LIGHT_SPOT) {
			string prop = prefixLightType + "Lights[" + to_string(i) + "].angle";
            glUniform1f(glGetUniformLocation(shader, prop.c_str()), angle);
		}
        {
            string prop = prefixLightType + "Lights[" + to_string(i) + "].color";
            glUniform4fv(glGetUniformLocation(shader, prop.c_str()),
                1, glm::value_ptr(color));
        }
		lightIndex[(int) t]++;
	};
    f(i,0,t_renderData.lights.size()) {
        if (lightIndex[(int) t_renderData.lights[i].type] < 8) {
			processLight(t_renderData.lights[i].type, 
				t_renderData.lights[i].pos,
                t_renderData.lights[i].function,
                t_renderData.lights[i].dir,
				t_renderData.lights[i].color,
				t_renderData.lights[i].penumbra,
				t_renderData.lights[i].angle
			);
		}
	}
	f(t,0,3) while (lightIndex[t] < 8) {
		vec4 zero(0);
		vec4 zeroPos(0,0,0,1);
		vec3 zero3(0);
        processLight((LightType) t,
			zeroPos, zero3, zero, zero, 1, 1
		);
	}
}

void Realtime::a_loadShape(const GLuint &shader, int index) {
	GLint modelLocation = glGetUniformLocation(shader, "modelMatrix");
	glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr( t_renderData.shapes[index].ctm ));

	const SceneMaterial &mat = t_renderData.shapes[index].primitive.material;
	glUniform1f(glGetUniformLocation(shader, "shininess"), mat.shininess);
	glUniform4fv(glGetUniformLocation(shader, "ambient"), 1, glm::value_ptr(mat.cAmbient));
	glUniform4fv(glGetUniformLocation(shader, "diffuse"), 1, glm::value_ptr(mat.cDiffuse));
	glUniform4fv(glGetUniformLocation(shader, "specular"), 1, glm::value_ptr(mat.cSpecular));
}