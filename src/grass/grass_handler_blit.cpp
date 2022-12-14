#include "grass_handler.h"
#include "t_utils/ErrorHandler.h"
#include <iostream>
using namespace std;

void GrassHandler::initFramebuffers() {
	ErrorHandler::errorCheck("-- on before framebuffer");
	fbo_raw.initialize();
	ErrorHandler::errorCheck("-- on initialize framebuffer");
	ErrorHandler::errorCheck("-- on use fbo raw");
	fbo_raw.createTexture(default_screen, GL_RGBA, GL_RGBA, GL_UNSIGNED_BYTE, 
		fbo_width, fbo_height);
	ErrorHandler::errorCheck("-- on create texture");
	fbo_raw.createRenderBuffer(default_screen_depth, GL_DEPTH24_STENCIL8, fbo_width, fbo_height);
	ErrorHandler::errorCheck("-- on create renderBuffer");

	fbo_raw.use();
	fbo_raw.attachTexture(default_screen, GL_COLOR_ATTACHMENT0);
	fbo_raw.attachRenderBuffer(default_screen_depth, GL_DEPTH_STENCIL_ATTACHMENT);
	ErrorHandler::errorCheck("-- on attach texture");
	fbo_raw.finalize();
	ErrorHandler::errorCheck("-- on finalize");
	fbo_raw.detach();
	ErrorHandler::errorCheck("-- on create framebuffer");
}

void GrassHandler::destroyFramebuffers() {
	fbo_raw.destroy();
	ErrorHandler::errorCheck("-- on destroy framebuffer");
	glDeleteTextures(1, &default_screen);
	ErrorHandler::errorCheck("-- on delete textures");
}