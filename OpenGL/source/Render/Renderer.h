#pragma once

#include <array>

#include <glad/glad.h>

using RendererID = uint32_t;

class Renderer {
public:

	static void Clear() {
		SetClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	}

	static void Clear(float r, float g, float b, float a) {
		glClearColor(r, g, b, a);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	}

	static void SetClearColor(float r, float g, float b, float a) {}

	static void DrawIndexed(unsigned int count) {
		glDrawElements(GL_TRIANGLES, count, GL_UNSIGNED_INT, nullptr);
	}
};
