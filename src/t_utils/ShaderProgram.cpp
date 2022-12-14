#include "ShaderProgram.h"
#include <QFile>
#include <QTextStream>
#include <glm/gtc/type_ptr.hpp>

using namespace std;

void ShaderProgram::attachShader(GLenum shaderType, const char *filepath) {
	GLuint shaderID = glCreateShader(shaderType);

	// Read shader file.
	std::string code;
	compileToString(filepath, code);

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


void ShaderProgram::finalizeProgram() {
	glLinkProgram(programID);
	while (createdShaders.size()) {
		glDeleteShader(createdShaders.back());
		createdShaders.pop_back();
	}
}

GLuint ShaderProgram::getShaderLoc(string variableName) const { return glGetUniformLocation(programID, variableName.c_str()); }

void ShaderProgram::setFloat(string varName, float value) const { glUniform1f(getShaderLoc(varName), value); }
void ShaderProgram::setInt(string varName, int value) const { glUniform1i(getShaderLoc(varName), value); }
void ShaderProgram::setVec2(string varName, glm::vec2 value) const { glUniform2fv(getShaderLoc(varName), 1, glm::value_ptr( value)); } 
void ShaderProgram::setVec3(string varName, glm::vec3 value) const { glUniform3fv(getShaderLoc(varName), 1, glm::value_ptr( value)); } 
void ShaderProgram::setVec4(string varName, glm::vec4 value) const { glUniform4fv(getShaderLoc(varName), 1, glm::value_ptr(value)); }
void ShaderProgram::setMat4(string varName, glm::mat4 value) const { glUniformMatrix4fv(getShaderLoc(varName), 1, GL_FALSE, glm::value_ptr( value ));}

void ShaderProgram::compileToString(const char* fileName, string &result) {
    QString filepathStr = QString(fileName);
	QFile file(filepathStr);
	if (file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        cout << "Parsing: " << fileName << endl;
		fflush(stdout);
		QTextStream stream(&file);

		while (!stream.atEnd()) {
			string line = stream.readLine().toStdString();
			if (line.rfind("#include", 0) == 0) {
				// if this is an include line, recurse
				string fileNameInclude = line.substr(9, line.size() - 9);
				string fileNameIncludeFull(fileName);

				while (fileNameIncludeFull.size() && fileNameIncludeFull.back() != '/') fileNameIncludeFull.pop_back();
				fileNameIncludeFull += fileNameInclude;

                compileToString(fileNameIncludeFull.c_str(), result);
			} else {
				result += line;
				result.push_back('\n');
			}
		}
	} else
		throw std::runtime_error(std::string("Failed to open shader: ")+ fileName);
}
