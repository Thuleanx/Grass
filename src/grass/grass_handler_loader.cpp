#include "grass_handler.h"

#define f(i,a,b) for (int i = a; i < b; i++)
using namespace glm;
using namespace std;

void GrassHandler::loadCameraData(const ShaderProgram &shader, Camera &camera) {
	shader.setVec4("camPos", camera.getPosition());
	shader.setMat4("viewMatrix", camera.getViewMatrix());
    shader.setMat4("projMatrix", camera.getProjectionMatrix());
}
void GrassHandler::loadLightData(const ShaderProgram &shader, RenderData &renderData) {
	shader.setFloat("ka", renderData.globalData.ka);
	shader.setFloat("kd", renderData.globalData.kd);
	shader.setFloat("ks", renderData.globalData.ks);

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
    f(i,0,renderData.lights.size()) {
        if (lightIndex[(int) renderData.lights[i].type] < 8) {
			processLight(renderData.lights[i].type, 
				renderData.lights[i].pos,
                renderData.lights[i].function,
                renderData.lights[i].dir,
				renderData.lights[i].color,
				renderData.lights[i].penumbra,
				renderData.lights[i].angle
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

vec4 hexToColor(string hexColor) {
	int r, g, b;
	sscanf(hexColor.c_str(), "#%02x%02x%02x", &r, &g, &b);
	return vec4(r,g,b,255)/255.0f;
}

void GrassHandler::loadGrassData(const ShaderProgram &shader) {
	// shader.setVec4("ambient", vec4(.2f,0,.2f,1));
	// shader.setVec4("diffuse", vec4(1,1,0,1));
	// shader.setVec4("specular", vec4(0.3f,0.3f,0,1));

	shader.setVec4("grassColorTip", hexToColor(settings.grassColorTip));
	shader.setVec4("grassColorTop", hexToColor(settings.grassColorTop));
	shader.setVec4("grassColorBottom", hexToColor(settings.grassColorBottom));
	shader.setVec4("grassColorAmbientOcclusion", hexToColor(settings.grassColorAmbientOcclusion));

	shader.setVec4("grassColorOld", hexToColor(settings.grassColorOld));

	vec4 clearCol = hexToColor(settings.clearColor);

	shader.setFloat("partingRange", settings.partingRange);

	shader.setFloat("hillHeightMax", settings.hillHeightMax);
	shader.setFloat("hillHeightNoiseScale", settings.hillHeightNoiseScale);

	shader.setInt("applyColor", settings.applyColor);
	shader.setInt("applyColorVariance", settings.applyColorVariance);
	shader.setInt("applyWind", settings.applyWind);

	glClearColor(clearCol.r, clearCol.g, clearCol.b, clearCol.a);

}