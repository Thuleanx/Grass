#pragma once

// Defined before including GLEW to suppress deprecation messages on macOS
#ifdef __APPLE__
#define GL_SILENCE_DEPRECATION
#endif
#include <GL/glew.h>
#include <iostream>
#include <vector>
#include "shapes/Sphere.h"
#include "camera/camera.h"

class Player {
	public: 
		void awake();
		void update();
		void destroy();

		float moveSpeed = 6;
		void move(glm::vec2 displacement, Camera camera);

		void drawPlayer();
		glm::mat4 getCTM() const;
		glm::vec3 getPosition() const;
        void setPosition(glm::vec3 newPos) { pos = newPos; }
		glm::vec3 getVelocity() const { return velocity; }
	private:
		GLuint vao, vbo;
		Sphere sphere;
		glm::vec3 pos = glm::vec3(0,1,0);
		glm::vec3 size = glm::vec3(2,2,2);
		glm::vec3 velocity = glm::vec3(0,0,0);
};
