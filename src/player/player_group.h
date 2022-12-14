#pragma once

// Defined before including GLEW to suppress deprecation messages on macOS
#ifdef __APPLE__
#define GL_SILENCE_DEPRECATION
#endif
#include <GL/glew.h>
#include <iostream>
#include <vector>
#include "player.h"
#include "t_utils/ShaderProgram.h"

class PlayerGroup {
	public: 
		void awake();
		void update(glm::vec2 time);
		void onSettingsChanged();
		void destroy();
		Player& getMainPlayer();

		void drawPlayers(Camera camera);
		void drawLocations();
		GLuint getVelocityBuffer() { return velocityBuffer; }
		glm::vec4 getVelocityBufferSamplingScale() 
			{ return glm::vec4(1/2.0f,1/2.0f,
				float(1/AREA_COVERAGE),float(1/AREA_COVERAGE)); }
		void assignHeightMap(GLuint heightMap) { this->heightMap = heightMap; }
	private:
        ShaderProgram shader;
        ShaderProgram shader_drawLocation;
        ShaderProgram shader_updateVelocityBuffer;

		std::vector<Player> players;

		const int VELOCITY_BUFFER_SZ = 1000;
		const int MASK_RADIUS = 100;
		const int MASK_SZ = MASK_RADIUS * 2 + 1;
		const float AREA_COVERAGE = 100;
		bool initialized = false;

		GLuint velocityBuffer;
		GLuint maskTexture;
		GLuint heightMap = 0;

		void setupMask();
		void setupVelocityBuffer();
		void updateVelocityBuffer(glm::vec2 time);
};
