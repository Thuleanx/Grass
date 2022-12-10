#include "grass_handler.h"

void GrassHandler::initFramebuffers() {
	fbo_raw.initialize();
	fbo_raw.createTexture(default_screen, GL_RGBA, GL_RGBA, GL_UNSIGNED_INT, 
		fbo_width, fbo_height);
	fbo_raw.attachTexture(default_screen, GL_COLOR_ATTACHMENT0);
	fbo_raw.finalize();
}

void GrassHandler::destroyFramebuffers() {
	glDeleteTextures(1, &default_screen);
	fbo_raw.destroy();
}