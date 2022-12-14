#include "grass_handler.h"
#include "t_utils/Blit.h"
#include "t_utils/ErrorHandler.h"
#include <ctime>

#define f(i,a,b) for (int i = a; i < b; i++)
using namespace glm;
using namespace std;

clock_t startTime;
float lastTime = 0;

void GrassHandler::awake(RenderData &renderData) {
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

	initShaders();
	initVAOVBO();
	initFramebuffers();
	initHeightMap();

	generateGrass();
	generateWindTexture();

	shader_default.useProgram();
	loadGrassData(shader_default);
	// loadLightData(shader_default, renderData);
	shader_default.detach();

	ErrorHandler::errorCheck("-- on awake");
	startTime = clock();
	lastTime = startTime;

	players.assignHeightMap(heightMap);
	players.awake();
}

void GrassHandler::onResize(int screen_width, int screen_height, 
	int fbo_width, int fbo_height) {

	this->screen_width = screen_width;
	this->screen_height = screen_height;
	this->fbo_width = fbo_width;
	this->fbo_height = fbo_height;

	onResizeShaders();
	destroyFramebuffers();
	initFramebuffers();

	ErrorHandler::errorCheck("-- on resize");
}

void GrassHandler::onSettingsChanged() {
	shader_default.useProgram();
	loadGrassData(shader_default);
	shader_default.detach();
	players.onSettingsChanged();
	ErrorHandler::errorCheck("-- on settings changed");
}

void GrassHandler::onForceGrassRedraw() {
	destroyVAOVBO();
	initVAOVBO();
	generateGrass();
	ErrorHandler::errorCheck("-- on force redraw");
}

void GrassHandler::update(Camera &camera) {
	float time = float(clock()-startTime) / CLOCKS_PER_SEC;
	float deltaTime = time - lastTime;
	lastTime = time;
	players.update(vec2(time, deltaTime));
	rawPass: {
		shader_default.useProgram();
		shader_default.setFloat("time", time);
		fbo_raw.use();

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, wind_noiseTexture);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, players.getVelocityBuffer());
		glActiveTexture(GL_TEXTURE2);
		glBindTexture(GL_TEXTURE_2D, heightMap);

		glViewport(0, 0, fbo_width, fbo_height);
		// glViewport(0, 0, screen_width, screen_height);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		loadCameraData(shader_default, camera);

		glBindVertexArray(vao);
		// glBindTexture(GL_TEXTURE_2D, 0);
		vec2 chunkSz = vec2(settings.bladeCnt / settings.density) * 2.0f;
		f(dx,-settings.chunking,settings.chunking+1)
		f(dy,-settings.chunking,settings.chunking+1) {
			vec2 displacement = chunkSz * vec2(dx,dy);
			shader_default.setVec3("displacement", vec3(displacement.x, 0, displacement.y));
			glDrawArrays(GL_TRIANGLES, 0, trianglesPerBlade() * numGrassBlades() * 3);
		}
		glBindVertexArray(0);

		players.drawPlayers(camera);
	}

	ErrorHandler::errorCheck("-- rendering raw pass");

	postProcessingBlit: {
		shader_postprocessing.useProgram();
		shader_postprocessing.setFloat("time", time);
		glViewport(0, 0, screen_width, screen_height);

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, default_screen);
		// glBindTexture(GL_TEXTURE_2D, players.getVelocityBuffer());
		// glBindTexture(GL_TEXTURE_2D, wind_noiseTexture);
		// glBindTexture(GL_TEXTURE_2D, heightMap);
		Blit::blit(fbo_main);

		glBindTexture(GL_TEXTURE_2D, 0);
	}
	ErrorHandler::errorCheck("-- rendering screen pass");

	// unbind
	shader_postprocessing.detach();
	fbo_raw.detach();
}

void GrassHandler::onDestroy() {
	Blit::destroy();
	destroyShaders();
	destroyVAOVBO();
	destroyFramebuffers();
	destroyWindTexture();
	destroyHeightMap();
	players.destroy();
}
