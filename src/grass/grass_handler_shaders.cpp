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

	shader_compute_windNoise.initializeProgram();
	shader_compute_windNoise.attachShader(GL_COMPUTE_SHADER, ":/resources/shaders/noiseGenerate.compute");
	shader_compute_windNoise.finalizeProgram();

	shader_postprocessing.useProgram();
	shader_postprocessing.setInt("_MainTex", 0);
	shader_postprocessing.detach();

}

void GrassHandler::onResizeShaders() {
	vec4 screenTexel = vec4(1.0f/screen_width, 1.0f/screen_height, screen_width, screen_height);

	shader_postprocessing.useProgram();
	shader_postprocessing.setVec4("_MainTex_TexelSize", screenTexel);
	shader_postprocessing.detach();
	shader_compute_windNoise.detach();

	shader_default.useProgram();
	shader_default.setVec4("_TexelSize", screenTexel);
	shader_default.detach();

	ErrorHandler::errorCheck("-- on resize shaders");
}

void GrassHandler::destroyShaders() {
	shader_default.destroy();
	shader_compute_grass.destroy();
	shader_postprocessing.destroy();
}