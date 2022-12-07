#pragma once

// Defined before including GLEW to suppress deprecation messages on macOS
#ifdef __APPLE__
#define GL_SILENCE_DEPRECATION
#endif
#include <GL/glew.h>
#include <iostream>
#include <glm/gtc/type_ptr.hpp>
#include <vector>
#include "Framebuffer.h"

class Blit {
	public: 
		static void initialize();
		static void blit(const Framebuffer &fbo);
		static void destroy();
	private:
		static GLuint vao;
        static GLuint vbo;
		static std::vector<GLfloat> fullscreen_quad_data;
};
