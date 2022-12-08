#include "realtime.h"
#include <iostream>
#include "utils/scenedata.h"
#include "settings.h"
#include <time.h>
#include <iomanip>
#include <random>
#include <chrono>

#define f(i,a,b) for (int i = a; i < b; i++)
using namespace glm;
using namespace std;

void Realtime::a_loadSceneGlobalData(const ShaderProgram &shader) {
	shader.setFloat("ka", t_renderData.globalData.ka);
	shader.setFloat("kd", t_renderData.globalData.kd);
	shader.setFloat("ks", t_renderData.globalData.ks);
}

void Realtime::a_loadCameraData(const ShaderProgram &shader) {
	shader.setVec4("camPos", t_camera.getPosition());
	shader.setMat4("viewMatrix", t_camera.getViewMatrix());
    shader.setMat4("projMatrix", t_camera.getProjectionMatrix());
}

void Realtime::a_loadLightData(const ShaderProgram &shader) {
	int lightIndex[3] = {0,0,0};
	auto processLight = [&](LightType t, vec4 &pos, vec3 &function, vec4& dir, vec4 &color, float penumbra, float angle) {
		pos[3] = 1;
		dir[3] = 0;

		string prefixLightType = "";
		int i = lightIndex[(int) t];
		if (t == LightType::LIGHT_POINT) prefixLightType = "point";
		if (t == LightType::LIGHT_DIRECTIONAL) prefixLightType = "dir";
		if (t == LightType::LIGHT_SPOT) prefixLightType = "spot";

		if (t == LightType::LIGHT_SPOT || t == LightType::LIGHT_POINT) 
            shader.setVec4(prefixLightType + "Lights[" + to_string(i) + "].pos", pos);
		if (t == LightType::LIGHT_DIRECTIONAL || t == LightType::LIGHT_SPOT)
			shader.setVec4(prefixLightType + "Lights[" + to_string(i) + "].dir", dir);
		if (t == LightType::LIGHT_POINT || t == LightType::LIGHT_SPOT)
			shader.setVec3(prefixLightType + "Lights[" + to_string(i) + "].function", function);
		if (t == LightType::LIGHT_SPOT) shader.setFloat(prefixLightType + "Lights[" + to_string(i) + "].penumbra", penumbra);
		if (t == LightType::LIGHT_SPOT) {
			shader.setFloat(prefixLightType + "Lights[" + to_string(i) + "].angle",angle);
		}
        {
			shader.setVec4(prefixLightType + "Lights[" + to_string(i) + "].color", color);
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

void Realtime::a_loadShape(const ShaderProgram &shader, int index) {
	shader.setMat4("modelMatrix", t_renderData.shapes[index].ctm);

	const SceneMaterial &mat = t_renderData.shapes[index].primitive.material;
	shader.setFloat("shininess", mat.shininess);
	shader.setVec4("ambient", mat.cAmbient);
	shader.setVec4("diffuse", mat.cDiffuse);
	shader.setVec4("specular", mat.cSpecular);
}
