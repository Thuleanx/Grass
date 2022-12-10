#include "grass_handler.h"
#include "t_utils/ErrorHandler.h"

void GrassHandler::initFramebuffers() {
	fbo_raw.initialize();
	ErrorHandler::errorCheck("-- on initialize framebuffer");
	fbo_raw.use();
	fbo_raw.createTexture(default_screen, GL_RGBA, GL_RGBA, GL_UNSIGNED_INT, 
		fbo_width, fbo_height);
	fbo_raw.createRenderBuffer(default_screen_depth, GL_DEPTH24_STENCIL8, fbo_width, fbo_height);

	ErrorHandler::errorCheck("-- on create texture");
	fbo_raw.attachTexture(default_screen, GL_COLOR_ATTACHMENT0);
	fbo_raw.attachRenderBuffer(default_screen_depth, GL_DEPTH_STENCIL_ATTACHMENT);
	ErrorHandler::errorCheck("-- on attach texture");
	fbo_raw.finalize();
	ErrorHandler::errorCheck("-- on finalize");
	fbo_raw.detach();
	ErrorHandler::errorCheck("-- on create framebuffer");
}

void GrassHandler::destroyFramebuffers() {
	glDeleteTextures(1, &default_screen);
	fbo_raw.destroy();
	ErrorHandler::errorCheck("-- on destroy framebuffer");
}