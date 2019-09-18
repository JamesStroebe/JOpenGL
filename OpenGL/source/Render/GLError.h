#pragma once

#include <glad/glad.h>

#include <iostream>

#define ASSERT(x) if(!(x)) __debugbreak();
#define GLCall(x) GLCheckError();\
	x;\
	ASSERT(GLLogCall(#x,__FILE__,__LINE__))



static GLenum GLCheckError() {
	return glGetError();
}

static bool GLLogCall(const char* function, const char* file, int line) {
	GLenum error = GLCheckError();
	if (error != GL_NO_ERROR) {
		std::cout << "[OpenGL Error] (" << error << "): " << function << " " << file << "|" << line << "\n";
		return false;
	}
	return true;

}
