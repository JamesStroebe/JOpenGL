#pragma once

#include <string>
#include <fstream>

#include <unordered_map>
#include <glad/glad.h>

#include "Renderer.h"

class Shader {
public:
	Shader(const std::string& path);

	void Bind() const;
	void Unbind() const;

private:
	std::string ReadFile(const std::string& path);
	std::unordered_map<GLenum, std::string> PreProcess(const std::string& path);
	void Compile();

	std::string m_Path;
	RendererID m_RendererID;

	std::unordered_map<GLenum, std::string> m_ShaderSource;
};