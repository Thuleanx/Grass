#pragma once

// Defined before including GLEW to suppress deprecation messages on macOS
#ifdef __APPLE__
#define GL_SILENCE_DEPRECATION
#endif
#include <GL/glew.h>
#include <iostream>
#include <glm/gtc/type_ptr.hpp>
#include <vector>

class Framebuffer {
	public: 
		Framebuffer() {}
		Framebuffer(GLuint fbo_value) : fbo(fbo_value) {}
		void initialize();
		void attachTexture(const GLuint &texture, GLenum attachmentPoint, bool is2D = true);
		void finalize();
		void use() const;
		void detach() const;
		void checkStatus();
		void destroy();
		static void createTexture(GLuint &texture, GLenum format, GLenum internalFormat, GLenum dataType, 
			int width, int height, GLenum filter = GL_LINEAR, GLenum wrapMode = GL_REPEAT);
	private:
		GLuint fbo;
		std::vector<GLenum> attachmentPoints;
};
