#include "grass_handler.h"
#include "t_utils/ErrorHandler.h"

#define f(i,a,b) for (int i = a; i < b; i++)
using namespace glm;
using namespace std;

void GrassHandler::initShaders() {
	shader_default.initializeProgram();
	shader_default.attachShader(GL_VERTEX_SHADER, ":/resources/shaders/grass.vert");
	shader_default.attachShader(GL_FRAGMENT_SHADER, ":/resources/shaders/grass.frag");
	shader_default.finalizeProgram();

	shader_compute_grass.initializeProgram();
	shader_compute_grass.attachShader(GL_COMPUTE_SHADER, ":/resources/shaders/grassGenerate.compute");
	shader_compute_grass.finalizeProgram();

	shader_postprocessing.initializeProgram();
	shader_postprocessing.attachShader(GL_VERTEX_SHADER, ":/resources/shaders/postprocessing.vert");
	shader_postprocessing.attachShader(GL_FRAGMENT_SHADER, ":/resources/shaders/FXAA.frag");
	shader_postprocessing.finalizeProgram();

	shader_postprocessing.useProgram();
	shader_postprocessing.setInt("_MainTex", 0);
	shader_postprocessing.detach();
}

void GrassHandler::onResizeShaders() {
	shader_postprocessing.useProgram();
	shader_postprocessing.setVec4("_MainTex_TexelSize", vec4(1.0f/screen_width, 1.0f/screen_height, screen_width, screen_height));
	shader_postprocessing.detach();

	ErrorHandler::errorCheck("-- on resize shaders");
}

void GrassHandler::destroyShaders() {
	shader_default.destroy();
	shader_compute_grass.destroy();
	shader_postprocessing.destroy();
}