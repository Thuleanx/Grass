#pragma once

// Defined before including GLEW to suppress deprecation messages on macOS
#ifdef __APPLE__
#define GL_SILENCE_DEPRECATION
#endif
#include <GL/glew.h>
#include <QFile>
#include <QTextStream>
#include <iostream>

class ShaderProgram {
public:
	void initializeProgram() { programID = glCreateProgram(); }
	void attachShader(GLenum shaderType, const char *filepath) {
        GLuint shaderID = glCreateShader(shaderType);

        // Read shader file.
        std::string code;
        QString filepathStr = QString(filepath);
        QFile file(filepathStr);
        if (file.open(QIODevice::ReadOnly | QIODevice::Text)) {
            QTextStream stream(&file);
            code = stream.readAll().toStdString();
        }else{
            throw std::runtime_error(std::string("Failed to open shader: ")+filepath);
        }

        // Compile shader code.
        const char *codePtr = code.c_str();
        glShaderSource(shaderID, 1, &codePtr, nullptr); // Assumes code is null terminated
        glCompileShader(shaderID);

        // Print info log if shader fails to compile.
        GLint status;
        glGetShaderiv(shaderID, GL_COMPILE_STATUS, &status);

        if (status == GL_FALSE) {
            GLint length;
            glGetShaderiv(shaderID, GL_INFO_LOG_LENGTH, &length);

            std::string log(length, '\0');
            glGetShaderInfoLog(shaderID, length, nullptr, &log[0]);

            glDeleteShader(shaderID);
            throw std::runtime_error(log);
        }

		glAttachShader(programID, shaderID);
		createdShaders.push_back(shaderID);
	}
	void finalizeProgram() {
		glLinkProgram(programID);
		while (createdShaders.Count) {
			glDeleteShader(createdShaders.back());
			createdShaders.pop_back();
		}
	}
	void useProgram() {
		glUseProgram(programID);
	}
private:
	GLuint programID;
	std::vector<GLuint> createdShaders;
};
