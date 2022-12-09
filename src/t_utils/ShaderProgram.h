#pragma once

// Defined before including GLEW to suppress deprecation messages on macOS
#ifdef __APPLE__
#define GL_SILENCE_DEPRECATION
#endif
#include <GL/glew.h>
#include <iostream>
#include <vector>
#include <glm/glm.hpp>

class ShaderProgram {
public:
	void initializeProgram() { programID = glCreateProgram(); }
	void attachShader(GLenum shaderType, const char *filepath);
	void finalizeProgram();
	void useProgram() const { glUseProgram(programID); }
	void detach() const { glUseProgram(0); }
	void destroy() { glDeleteProgram(programID); }

	void setFloat(std::string variableName, float value) const;
	void setInt(std::string variableName, int value) const;
	void setVec2(std::string variableName, glm::vec2 value) const;
	void setVec3(std::string variableName, glm::vec3 value) const;
	void setVec4(std::string variableName, glm::vec4 value) const;
	void setMat4(std::string variableName, glm::mat4 value) const;
private:
	GLuint programID;
	std::vector<GLuint> createdShaders;
	GLuint getShaderLoc(std::string variableName) const;
};
