#include "player_group.h"
#include <algorithm>
#include "t_utils/Framebuffer.h"
#include "t_utils/ErrorHandler.h"
#define f(i,a,b) for (int i = a; i < b; i++)
using namespace std;
using namespace glm;

void PlayerGroup::setupMask() {
	vector<vec2> maskVector(MASK_SZ * MASK_SZ);
	f(x,0,MASK_SZ) f(y,0,MASK_SZ) {
		vec2 origin = vec2(MASK_RADIUS, MASK_RADIUS);
		vec2 pos = (vec2(x,y) - origin) / float(MASK_RADIUS);
		float len = length(pos);
		vec2 dir = len > 0 ? normalize(pos) : pos;
		maskVector[y*MASK_SZ + x] = dir * std::clamp(1-len, 0.0f, 1.0f);
	}

	Framebuffer::createTexture(maskTexture, GL_RGBA32F, GL_RG, 
		GL_FLOAT, MASK_SZ, MASK_SZ, GL_NEAREST, GL_REPEAT, &maskVector[0]);

	glBindImageTexture(1, maskTexture, 0, GL_FALSE, 0, GL_READ_ONLY, GL_RGBA32F);
}

void PlayerGroup::setupVelocityBuffer() {
	vector<vec2> initial(VELOCITY_BUFFER_SZ * VELOCITY_BUFFER_SZ, vec2(0.5, 0.5));
	Framebuffer::createTexture(velocityBuffer, GL_RGBA32F, GL_RG, GL_FLOAT, VELOCITY_BUFFER_SZ, VELOCITY_BUFFER_SZ,
		GL_LINEAR, GL_CLAMP, &initial[0]);
	glBindImageTexture(0, velocityBuffer, 0, GL_FALSE, 0, GL_READ_WRITE, GL_RGBA32F);
}

void PlayerGroup::awake() {
	shader.initializeProgram();
	shader.attachShader(GL_VERTEX_SHADER, ":/resources/shaders/player.vert");
	shader.attachShader(GL_FRAGMENT_SHADER, ":/resources/shaders/player.frag");
	shader.finalizeProgram();

	players = vector<Player>(numPlayers);
	f(i,0,numPlayers)
		players[i].awake();

	shader_drawLocation.initializeProgram();
	shader_drawLocation.attachShader(GL_COMPUTE_SHADER, ":/resources/shaders/playerDraw.compute");
	shader_drawLocation.finalizeProgram();

	shader_drawLocation.useProgram();
	setupMask();
	ErrorHandler::errorCheck("-- on gen mask");
	setupVelocityBuffer();
	ErrorHandler::errorCheck("-- on gen velocity buffer");
	shader_drawLocation.detach();
}

void PlayerGroup::update() {
	f(i,0,numPlayers)
		players[i].update();
	drawLocations();
}

void PlayerGroup::destroy() {
	f(i,0,numPlayers)
		players[i].destroy();
	shader.destroy();

	glDeleteTextures(1, &maskTexture);
	glDeleteTextures(1, &velocityBuffer);
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

void PlayerGroup::drawLocations() {
	shader_drawLocation.useProgram();
	for (auto player : players) {
		shader_drawLocation.setVec2("pos", vec2(player.getPosition().x, player.getPosition().z));
		glDispatchCompute(
			MASK_SZ, 1, MASK_SZ);
		glMemoryBarrier(GL_ALL_BARRIER_BITS);
	}
	shader_drawLocation.detach();
}