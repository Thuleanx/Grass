#include "player.h"
#include "shapes/Sphere.h"
using namespace glm;
using namespace std;

void Player::drawPlayer() {
	glBindVertexArray(vao);
	glDrawArrays(GL_TRIANGLES, 0, sphere.shapeCount());
    glBindVertexArray(0);
}

void Player::awake() {
	sphere.updateParams(15,15);

	glGenVertexArrays(1, &vao);
	glGenBuffers(1, &vbo);

	glBindVertexArray(vao);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);

	vector<float> data = sphere.generateShape();
	glBufferData(GL_ARRAY_BUFFER,data.size() * sizeof(GLfloat),data.data(), GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(GLfloat) * 6, reinterpret_cast<void*>(0));
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(GLfloat) * 6, reinterpret_cast<void*>(3 * sizeof(GLfloat)));

	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void Player::update() {
}

void Player::destroy() {
	glDeleteVertexArrays(1, &vao);
	glDeleteBuffers(1, &vbo);
}

void Player::move(vec2 displacement, Camera camera) {
	displacement *= moveSpeed;
	vec3 forward = camera.getLook();
	vec3 right = cross(camera.getLook(), camera.getUp());
	forward.y = 0; right.y = 0;
	forward = normalize(forward) * displacement.y;
	right = normalize(right) * displacement.x;
	pos += forward + right;

	if (length(displacement))
		velocity = normalize(forward + right) * moveSpeed;
	else velocity = vec3(0,0,0);
}

mat4 Player::getCTM() const {
	mat4 T = mat4(
		size.x, 0, 0, 0,
		0, size.y, 0, 0,
		0, 0, size.z, 0,
		pos.x, pos.y, pos.z, 1
	);
	return T;
}

vec3 Player::getPosition() const { return pos; }
