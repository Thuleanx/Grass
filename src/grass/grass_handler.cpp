#include "grass_handler.h"
#include "t_utils/Blit.h"

#define f(i,a,b) for (int i = a; i < b; i++)
using namespace glm;
using namespace std;

void GrassHandler::awake() {
    Blit::initialize();

	int work_grp_cnt[3];
	glGetIntegeri_v(GL_MAX_COMPUTE_WORK_GROUP_COUNT, 0, &work_grp_cnt[0]);
	glGetIntegeri_v(GL_MAX_COMPUTE_WORK_GROUP_COUNT, 1, &work_grp_cnt[1]);
	glGetIntegeri_v(GL_MAX_COMPUTE_WORK_GROUP_COUNT, 2, &work_grp_cnt[2]);

	printf("max global (total) work group counts x:%i y:%i z:%i\n",
		   work_grp_cnt[0], work_grp_cnt[1], work_grp_cnt[2]);

	int work_grp_size[3];
	glGetIntegeri_v(GL_MAX_COMPUTE_WORK_GROUP_SIZE, 0, &work_grp_size[0]);
	glGetIntegeri_v(GL_MAX_COMPUTE_WORK_GROUP_SIZE, 1, &work_grp_size[1]);
	glGetIntegeri_v(GL_MAX_COMPUTE_WORK_GROUP_SIZE, 2, &work_grp_size[2]);

	printf("max local (in one shader) work group sizes x:%i y:%i z:%i\n",
		   work_grp_size[0], work_grp_size[1], work_grp_size[2]);

	int work_grp_inv;
	glGetIntegerv(GL_MAX_COMPUTE_WORK_GROUP_INVOCATIONS, &work_grp_inv);
	printf("max local work group invocations %i\n", work_grp_inv);

	shader_default.initializeProgram();
	shader_default.attachShader(GL_VERTEX_SHADER, ":/resources/shaders/grass.vert");
	shader_default.attachShader(GL_FRAGMENT_SHADER, ":/resources/shaders/grass.frag");
	shader_default.finalizeProgram();

	shader_compute_grass.initializeProgram();
	shader_compute_grass.attachShader(GL_COMPUTE_SHADER, ":/resources/shaders/grassGenerate.compute");
	shader_compute_grass.finalizeProgram();
}

void GrassHandler::onResize(int screen_width, int screen_height, 
	int fbo_width, int fbo_height) {

	this->screen_width = screen_width;
	this->screen_height = screen_height;
	this->fbo_width = fbo_width;
	this->fbo_height = fbo_height;
}

void GrassHandler::update(Camera &camera) {
}

void GrassHandler::onDestroy() {
	Blit::destroy();
	shader_default.destroy();
	shader_compute_grass.destroy();
}

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