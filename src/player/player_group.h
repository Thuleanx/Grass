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
		void update();
		void destroy();
		Player& getMainPlayer();

		void drawPlayers(Camera camera);
		void drawLocations();
		GLuint getVelocityBuffer() { return velocityBuffer; }
	private:
        const int numPlayers = 1;
        ShaderProgram shader;

        ShaderProgram shader_drawLocation;

		std::vector<Player> players;

		const int VELOCITY_BUFFER_SZ = 1000;
		const int MASK_RADIUS = 30;
		const int MASK_SZ = MASK_RADIUS * 2 + 1;

		GLuint velocityBuffer;
		GLuint maskTexture;

		void setupMask();
		void setupVelocityBuffer();
};
