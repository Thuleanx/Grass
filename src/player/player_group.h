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
	private:
        const int numPlayers = 1;
        ShaderProgram shader;
		std::vector<Player> players;

};
