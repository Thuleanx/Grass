#include "player_group.h"
#define f(i,a,b) for (int i = a; i < b; i++)
using namespace std;

void PlayerGroup::awake() {
	shader.initializeProgram();
	shader.attachShader(GL_VERTEX_SHADER, ":/resources/shaders/player.vert");
	shader.attachShader(GL_FRAGMENT_SHADER, ":/resources/shaders/player.frag");
	shader.finalizeProgram();

	players = vector<Player>(numPlayers);
	f(i,0,numPlayers)
		players[i].awake();
}

void PlayerGroup::update() {
	f(i,0,numPlayers)
		players[i].update();
}

void PlayerGroup::destroy() {
	f(i,0,numPlayers)
		players[i].destroy();
	shader.destroy();
}

Player& PlayerGroup::getMainPlayer() {
	return players[0];
}

void PlayerGroup::drawPlayers(Camera camera) {
	shader.useProgram();
	shader.setMat4("viewMatrix", camera.getViewMatrix());
	shader.setMat4("projMatrix", camera.getProjectionMatrix());

	for (auto player : players) {
		shader.setMat4("modelMatrix", player.getCTM());
		player.drawPlayer();
	}
	shader.detach();
}