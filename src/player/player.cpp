#include "player.h"
#include "shapes/Sphere.h"
using namespace glm;
using namespace std;

void Player::drawPlayer() {
	shader.useProgram();
	glBindVertexArray(vao);

	glDrawArrays(GL_TRIANGLES, 0, shape->shapeCount());

	glBindVertexArray(0);
	shader.detach();
}

void Player::awake() {
	shape = new Sphere();
	shape->updateParams(1,1);
	glGenVertexArrays(1, &vao);
	glGenBuffers(1, &vbo);

	glBindVertexArray(vao);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);

	vector<float> data = shape->generateShape();
	glBufferData(GL_ARRAY_BUFFER,data.size() * sizeof(GLfloat),data.data(), GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(GLfloat) * 6, reinterpret_cast<void*>(0));
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(GLfloat) * 6, reinterpret_cast<void*>(3 * sizeof(GLfloat)));

	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	shader.initializeProgram();
	shader.attachShader(GL_VERTEX_SHADER, ":/resources/shaders/player.vert");
	shader.attachShader(GL_FRAGMENT_SHADER, ":/resources/shaders/player.frag");
	shader.finalizeProgram();
}

void Player::update() {
	drawPlayer();
}

void Player::destroy() {
	glDeleteVertexArrays(1, &vao);
	glDeleteBuffers(1, &vbo);
}

void Player::move(vec2 displacement, Camera camera) {
	vec3 forward = camera.getLook();
	vec3 right = cross(camera.getLook(), camera.getUp());
	forward.y = 0; right.y = 0;
	forward = normalize(forward) * displacement.y;
	right = normalize(right) * displacement.x;
	pos += forward + right;
}

mat4 Player::getCTM() {
	mat4 T = mat4(
		1, 0, 0, 0,
		0, 1, 0, 0,
		0, 0, 1, 0,
		-pos.x, -pos.y, -pos.z, 1
	);
}