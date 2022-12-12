#pragma once

#include <glm/glm.hpp>
#include "utils/scenedata.h"

// A class representing a virtual camera.

// Feel free to make your own design choices for Camera class, the functions below are all optional / for your convenience.
// You can either implement and use these getters, or make your own design.
// If you decide to make your own design, feel free to delete these as TAs won't rely on them to grade your assignments.

class Camera {
public:
	Camera();
	Camera(int width, int height, const SceneCameraData &camData);

    // Returns the view matrix for the current camera settings.
    // You might also want to define another function that return the inverse of the view matrix.
    glm::mat4 getViewMatrix() const;

    glm::mat4 getViewMatrixInverse() const;

	glm::mat4 getProjectionMatrix() const;

	glm::vec4 getPosition() const;

	glm::vec3 getLook() const;
	glm::vec3 getUp() const;

    // Returns the aspect ratio of the camera.
    float getAspectRatio() const;

    // Returns the height angle of the camera in RADIANS.
    float getHeightAngle() const;

	float getWidthAngle() const;

    // Returns the focal length of this camera.
    // This is for the depth of field extra-credit feature only;
    // You can ignore if you are not attempting to implement depth of field.
    float getFocalLength() const;

    // Returns the focal length of this camera.
    // This is for the depth of field extra-credit feature only;
    // You can ignore if you are not attempting to implement depth of field.
    float getAperture() const;

	void setSize(int w, int h);
	glm::vec2 getNearPlaneSize();

	void recalculateWidthAngle();
	void recalculateView();
	void recalculateProj();

	void translate(glm::vec3 movement);
	void rotate(glm::vec2 movement);

private:
	glm::mat4 view, viewInv, proj;
	glm::vec3 look, up, pos;
	float widthAngle;
	SceneCameraData camData;
	int width, height;
};

