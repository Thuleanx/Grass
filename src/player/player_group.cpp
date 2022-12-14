#include "player_group.h"
#include <algorithm>
#include "t_utils/Framebuffer.h"
#include "settings.h"
#include "t_utils/ErrorHandler.h"
#define f(i,a,b) for (int i = a; i < b; i++)
using namespace std;
using namespace glm;

void PlayerGroup::setupMask() {
	vector<vec4> maskVector(MASK_SZ * MASK_SZ);
	float mid = 0.2;
	f(x,0,MASK_SZ) f(y,0,MASK_SZ) {
		vec2 origin = vec2(MASK_RADIUS, MASK_RADIUS);
		vec2 pos = (vec2(x,y) - origin) / float(MASK_RADIUS);

		float len = length(pos);

		vec2 dir = len > 0 ? normalize(pos) : vec2(1,1);

		float strength = std::clamp(mid + 1 - len, 0.0f, 1.0f);

		strength = strength*strength;
		maskVector[y*MASK_SZ + x] = vec4(dir * strength, vec2(0,0));
	}

	Framebuffer::createTexture(maskTexture, GL_RGBA32F, GL_RGBA, 
		GL_FLOAT, MASK_SZ, MASK_SZ, GL_LINEAR, GL_REPEAT, &maskVector[0]);

	shader_drawLocation.setInt("maskTexture", 0);
	// glBindImageTexture(1, maskTexture, 0, GL_FALSE, 0, GL_READ_ONLY, GL_RGBA32F);
}

void PlayerGroup::setupVelocityBuffer() {
	vector<vec4> initial(VELOCITY_BUFFER_SZ * VELOCITY_BUFFER_SZ, vec4(0.5, 0.5, 0, 0));
	Framebuffer::createTexture(velocityBuffer, GL_RGBA32F, GL_RGBA, GL_FLOAT, VELOCITY_BUFFER_SZ, VELOCITY_BUFFER_SZ,
		GL_LINEAR, GL_CLAMP, &initial[0]);
	glBindImageTexture(0, velocityBuffer, 0, GL_FALSE, 0, GL_READ_WRITE, GL_RGBA32F);
}

void PlayerGroup::awake() {
	shader.initializeProgram();
	shader.attachShader(GL_VERTEX_SHADER, ":/resources/shaders/player.vert");
	shader.attachShader(GL_FRAGMENT_SHADER, ":/resources/shaders/player.frag");
	shader.finalizeProgram();

	players = vector<Player>(1+settings.extraPlayers.size());
	f(i,0,players.size())
		players[i].awake();

	shader_drawLocation.initializeProgram();
	shader_drawLocation.attachShader(GL_COMPUTE_SHADER, ":/resources/shaders/playerDraw.compute");
	shader_drawLocation.finalizeProgram();

	shader_updateVelocityBuffer.initializeProgram();
	shader_updateVelocityBuffer.attachShader(GL_COMPUTE_SHADER, ":/resources/shaders/grassSway.compute");
	shader_updateVelocityBuffer.finalizeProgram();

	shader_drawLocation.useProgram();
	setupMask();
	ErrorHandler::errorCheck("-- on gen mask");
	setupVelocityBuffer();
	ErrorHandler::errorCheck("-- on gen velocity buffer");
	shader_drawLocation.detach();

	shader_updateVelocityBuffer.useProgram();
	glBindImageTexture(0, velocityBuffer, 0, GL_FALSE, 0, GL_READ_WRITE, GL_RGBA32F);
	shader_updateVelocityBuffer.detach();
}

void PlayerGroup::update(vec2 time) {
    f(i,0,players.size()) {
		if (i) players[i].setPosition(
			vec3(
				settings.extraPlayers[i-1].r * cos(M_PI * settings.extraPlayers[i-1].g * time.x), 
				players[i].getPosition().y, 
				settings.extraPlayers[i-1].r * sin(M_PI * settings.extraPlayers[i-1].g * time.x)
			)
		);
		players[i].update();
	}
	drawLocations();
	updateVelocityBuffer();
}

void PlayerGroup::destroy() {
    f(i,0,players.size())
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
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, maskTexture);

	glMemoryBarrier(GL_ALL_BARRIER_BITS);
	for (auto player : players) {
		vec2 loc = vec2(
				player.getPosition().x * getVelocityBufferSamplingScale().z + getVelocityBufferSamplingScale().x, 
				player.getPosition().z * getVelocityBufferSamplingScale().w + getVelocityBufferSamplingScale().y
			);
		loc *= VELOCITY_BUFFER_SZ;
		shader_drawLocation.setVec2("pos", loc);
		shader_drawLocation.setVec3("playerVelocity", player.getVelocity());
		glDispatchCompute(
			ceil(MASK_SZ / 8.00f), 1, ceil(MASK_SZ / 8.00f));
		glMemoryBarrier(GL_ALL_BARRIER_BITS);
	}
	shader_drawLocation.detach();
}

void PlayerGroup::updateVelocityBuffer() {
	shader_updateVelocityBuffer.useProgram();
	glDispatchCompute(
		ceil(VELOCITY_BUFFER_SZ / 8.00f), 1, ceil(VELOCITY_BUFFER_SZ / 8.00f));
	shader_updateVelocityBuffer.detach();
}
