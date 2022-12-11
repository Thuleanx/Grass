#include "grass_handler.h"
#include "t_utils/Blit.h"
#include "t_utils/ErrorHandler.h"
#include <ctime>

#define f(i,a,b) for (int i = a; i < b; i++)
using namespace glm;
using namespace std;

clock_t startTime;

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

	generateGrass();

	shader_default.useProgram();
	loadGrassData(shader_default);
	loadLightData(shader_default, renderData);
	shader_default.detach();

	ErrorHandler::errorCheck("-- on awake");
	startTime = clock();
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
	destroyVAOVBO();
	initVAOVBO();
	generateGrass();

	ErrorHandler::errorCheck("-- on settings changed");
}

void GrassHandler::update(Camera &camera) {
	rawPass: {
		shader_default.useProgram();
		shader_default.setFloat("time", float(clock()-startTime) / CLOCKS_PER_SEC);
		fbo_raw.use();
		// glViewport(0, 0, fbo_width, fbo_height);
		glViewport(0, 0, screen_width, screen_height);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		loadCameraData(shader_default, camera);

		glBindVertexArray(vao);
		glDrawArrays(GL_TRIANGLES, 0, trianglesPerBlade() * numGrassBlades() * 3);
		glBindVertexArray(0);
	}

	ErrorHandler::errorCheck("-- rendering raw pass");

	postProcessingBlit: {
		shader_postprocessing.useProgram();
		glViewport(0, 0, screen_width, screen_height);

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, default_screen);
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
}
