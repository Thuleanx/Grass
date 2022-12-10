#pragma once
#include <GL/glew.h>
#include <iostream>
#include <vector>

class ErrorHandler {
	public:
		static GLenum errorCheck(std::string message);
};
