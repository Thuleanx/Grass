#include <stdexcept>
#include "camera.h"
#include <iostream>
#include <glm/gtx/transform.hpp>
#include "glm/gtc/matrix_transform.hpp"
#include "settings.h"
using namespace glm;
using namespace std;

#define f(i,a,b) for (int i = a; i < b; i++)

Camera::Camera() {
}

Camera::Camera(int width, int height, const SceneCameraData &camData) {
	this->camData = camData;
	this->width = width;
	this->height = height;
	look = camData.look;
	up = camData.up;
	pos = camData.pos; 

	recalculateView();
	recalculateWidthAngle();
	recalculateProj();
}

glm::mat4 Camera::getViewMatrix() const { return view; }
glm::mat4 Camera::getViewMatrixInverse() const { return viewInv; }
glm::mat4 Camera::getProjectionMatrix() const { return proj; }


glm::vec4 Camera::getPosition() const { 
	return vec4(pos.x, pos.y, pos.z, 1);
}

float Camera::getAspectRatio() const { 
	return float(width) / height; 
}

float Camera::getHeightAngle() const { return camData.heightAngle; }

float Camera::getWidthAngle() const { return widthAngle; }

float Camera::getFocalLength() const { return camData.focalLength; }

float Camera::getAperture() const { return camData.aperture; }

void Camera::setSize(int w, int h) { 
	this->width = w; 
	this->height = h;
	recalculateWidthAngle();
	recalculateView();
	recalculateProj();
}

vec2 Camera::getNearPlaneSize() {
	return settings.nearPlane * 2 * vec2(tan(getWidthAngle()/2), tan(getHeightAngle()/2));
}

void Camera::recalculateWidthAngle() {
	float k = height/2.0f * (1 / tan(camData.heightAngle/2.0f));
	float theta_w2 = 2*atan2(width/2.0f, k);

	widthAngle = theta_w2;
}

void Camera::recalculateView() {
	vec3 w = -normalize((vec3) look);
	vec3 v = normalize((vec3) up - dot((vec3) up, w) * w);
	vec3 u = cross(v,w);

	mat4 R = mat4(
		u.x, v.x, w.x, 0,
		u.y, v.y, w.y, 0,
		u.z, v.z, w.z, 0,
		0, 0, 0, 1
	);

	mat4 T = mat4(
		1, 0, 0, 0,
		0, 1, 0, 0,
		0, 0, 1, 0,
		-pos.x, -pos.y, -pos.z, 1
	);

	view = R*T;
	viewInv = inverse(view);
}


void Camera::recalculateProj() {
	float c = -settings.nearPlane / settings.farPlane;
	float iFar = 1.0f/settings.farPlane;

	mat4 remap = mat4(
		1, 0, 0, 0,
		0, 1, 0, 0,
		0, 0, -2, 0,
		0, 0, -1, 1
	);
	mat4 A = mat4(
		1, 0, 0, 0,
		0, 1, 0, 0,
		0, 0, 1/(1 + c), -1,
		0, 0, -c/(1+c), 0
	);
	mat4 B = mat4(
		iFar / tan(getWidthAngle() / 2), 0, 0, 0,
		0, iFar / tan(getHeightAngle() / 2), 0, 0,
		0, 0, iFar, 0,
		0, 0, 0, 1
	);
    proj = remap * A * B;
}

void Camera::translate(vec3 movement) {
	pos += normalize(look) * movement.y + normalize(cross(look, up)) * movement.x + movement.z * vec3(0,1,0);
	recalculateView();
}

mat3 rotateAround(vec3 u, float theta) {
	u = normalize(u);
	float cosTheta = cos(theta);
	float sinTheta = sin(theta);

	return mat3(
		cosTheta + u.x * u.x * (1 - cosTheta), 
		u.x * u.y * (1 - cosTheta) + u.z * sinTheta, 
		u.x * u.z * ( 1 - cosTheta) - u.y * sinTheta,
		u.x * u.y * (1 - cosTheta) - u.z * sinTheta, 
		cosTheta + u.y * u.y * (1-cosTheta),
		u.y * u.z * (1 - cosTheta) + u.x * sinTheta,
		u.x * u.z * (1 - cosTheta) + u.y * sinTheta,
		u.y * u.z * ( 1 - cosTheta) - u.x * sinTheta,
        cosTheta + u.z * u.z *  (1 - cosTheta)
	);
}

void Camera::rotate(vec2 movement) {
	mat3 rot = rotateAround(vec3(0,1,0), movement.x) * rotateAround(cross(look, up), movement.y);
    look = rot*look;
    up = rot*up;
	recalculateView();
}
