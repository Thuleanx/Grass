#pragma once

// Defined before including GLEW to suppress deprecation messages on macOS
#ifdef __APPLE__
#define GL_SILENCE_DEPRECATION
#endif
#include <GL/glew.h>
#include <iostream>
#include <vector>
#include "shapes/iShape.h"
#include "camera/camera.h"
#include "t_utils/ShaderProgram.h"

class Player {
	public: 
		void awake();
		void update();
		void destroy();

		void move(glm::vec2 displacement, Camera camera);
	private:
		GLuint vao, vbo;
		iShape *shape;
		glm::vec3 pos;
		ShaderProgram shader;

		void drawPlayer();
		glm::mat4 getCTM();
};
