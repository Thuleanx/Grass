#include "ErrorHandler.h"
#include <iostream>

GLenum ErrorHandler::errorCheck(std::string msg)
{
	GLenum code;
	const GLubyte *string;
	code = glGetError();
	if (code != GL_NO_ERROR)
	{
		string = gluErrorString(code);
		fprintf(stderr, "OpenGL error: %s %s\n", string, msg.c_str());
		fflush(stderr);
	}
	return code;
}