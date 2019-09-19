#pragma once

#include <string>
#include <fstream>

#include <unordered_map>
#include <glad/glad.h>

#include <glm/glm.hpp>


#include "Renderer.h"

class Shader {
public:
	Shader(const std::string& path);

	void Bind() const;
	void Unbind() const;

	void SetUniform1i(const std::string& name, int value);
	void SetUniform1f(const std::string& name, float value);
	void SetUniform3f(const std::string& name, const glm::vec3& vec);
	void SetUniform4f(const std::string& name, const glm::vec4& vec);
	void SetUniformMat3f(const std::string& name, const glm::mat3& mat);
	void SetUniformMat4f(const std::string& name, const glm::mat4& mat);

	

private:
	std::string ReadFile(const std::string& path);
	std::unordered_map<GLenum, std::string> PreProcess(const std::string& path);
	void Compile();

	int GetUniformLocation(const std::string& name);

	std::string m_Path;
	RendererID m_RendererID;

	std::unordered_map<GLenum, std::string> m_ShaderSource;

	std::unordered_map<std::string, int> m_UniformLocationCache;
};