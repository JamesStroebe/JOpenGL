#include "Shader.h"

#include "GLError.h"

static GLenum ShaderTypeFromString(const std::string& type) {
	if (type == "vertex")
		return GL_VERTEX_SHADER;
	if (type == "fragment" || type == "pixel")
		return GL_FRAGMENT_SHADER;

	return 0;
}

Shader::Shader(const std::string& path) :
	m_Path(path) {

	std::string source = ReadFile(path);
	m_ShaderSource = PreProcess(source);
	Compile();

}



void Shader::Bind() const {
	GLCall(glUseProgram(m_RendererID));
}

void Shader::Unbind() const {
	GLCall(glUseProgram(0));
}

std::string Shader::ReadFile(const std::string& path) {
	std::string result;
	std::ifstream in(path, std::ios::in | std::ios::binary);
	if (in) {
		in.seekg(0, std::ios::end);
		result.resize(in.tellg());
		in.seekg(0, std::ios::beg);
		in.read(&result[0], result.size());
		in.close();
	}
	return result;

}

std::unordered_map<GLenum, std::string> Shader::PreProcess(const std::string& source) {
	std::unordered_map<GLenum, std::string> shaderSources;

	const char* typeToken = "#type";
	size_t typeTokenLength = strlen(typeToken);
	size_t pos = source.find(typeToken, 0);
	while (pos != std::string::npos) {
		size_t eol = source.find_first_of("\r\n", pos);
		size_t begin = pos + typeTokenLength + 1;
		std::string type = source.substr(begin, eol - begin);

		size_t nextLinePos = source.find_first_of("\r\n", eol);
		pos = source.find(typeToken, nextLinePos);
		shaderSources[ShaderTypeFromString(type)] = source.substr(nextLinePos, pos - (nextLinePos == std::string::npos ? source.size() - 1 : nextLinePos));
	}

	return shaderSources;
}

void Shader::Compile() {
	GLCall(GLuint program = glCreateProgram());

	std::array<GLenum, 2> glShaderIDs;
	int glShaderIDIndex = 0;
	for (auto& kv : m_ShaderSource)
	{
		GLenum type = kv.first;
		std::string& source = kv.second;

		GLCall(GLuint shader = glCreateShader(type));

		const GLchar* sourceCStr = (const GLchar*)source.c_str();
		GLCall(glShaderSource(shader, 1, &sourceCStr, 0));

		GLCall(glCompileShader(shader));

		GLint isCompiled = 0;
		GLCall(glGetShaderiv(shader, GL_COMPILE_STATUS, &isCompiled));
		if (isCompiled == GL_FALSE)
		{
			GLint maxLength = 0;
			GLCall(glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &maxLength));

			std::vector<GLchar> infoLog(maxLength);
			GLCall(glGetShaderInfoLog(shader, maxLength, &maxLength, &infoLog[0]));

			std::cout << "Shader Compilation failed:\n" << &infoLog[0];

			GLCall(glDeleteShader(shader));

			ASSERT(false);
		}

		glShaderIDs[glShaderIDIndex++] = shader;
		GLCall(glAttachShader(program, shader));
		
	}

	m_RendererID = program;

	// Link our program
	GLCall(glLinkProgram(program));

	// Note the different functions here: glGetProgram* instead of glGetShader*.
	GLint isLinked = 0;
	GLCall(glGetProgramiv(program, GL_LINK_STATUS, (int*)& isLinked));
	if (isLinked == GL_FALSE)
	{
		GLint maxLength = 0;
		GLCall(glGetProgramiv(program, GL_INFO_LOG_LENGTH, &maxLength));

		// The maxLength includes the NULL character
		std::vector<GLchar> infoLog(maxLength);
		GLCall(glGetProgramInfoLog(program, maxLength, &maxLength, &infoLog[0]));

		std::cout << "Shader compilation failed: \n" << &infoLog[0];
		// We don't need the program anymore.
		GLCall(glDeleteProgram(program));

		for (auto id : glShaderIDs) {
			GLCall(glDeleteShader(id));
		}
		

		
	}

	for (auto id : glShaderIDs) {
		GLCall(glDetachShader(program, id));
	}
	

	//m_RendererID = program;
}

void Shader::SetUniform1i(const std::string& name, int value) {
	GLCall(glUniform1i(GetUniformLocation(name), value));
}

void Shader::SetUniform1f(const std::string& name, float value) {
	GLCall(glUniform1f(GetUniformLocation(name), value));
}

void Shader::SetUniform3f(const std::string& name, const glm::vec3& vec) {
	GLCall(glUniform3f(GetUniformLocation(name), vec.x, vec.y, vec.z));
}

void Shader::SetUniform4f(const std::string& name, const glm::vec4& vec) {
	GLCall(glUniform4f(GetUniformLocation(name), vec.x, vec.y, vec.z, vec.w));
}

void Shader::SetUniformMat3f(const std::string& name, const glm::mat3& mat) {
	GLCall(glUniformMatrix3fv(GetUniformLocation(name), 1, GL_FALSE, &mat[0][0]));
}

void Shader::SetUniformMat4f(const std::string& name, const glm::mat4& mat) {
	GLCall(glUniformMatrix4fv(GetUniformLocation(name), 1, GL_FALSE, &mat[0][0]));
}

int Shader::GetUniformLocation(const std::string& name) {
	if (m_UniformLocationCache.find(name) != m_UniformLocationCache.end())
		return m_UniformLocationCache[name];
	GLCall(int location = glGetUniformLocation(m_RendererID, name.c_str()));
	if (location == -1)
		std::cout << "Uniform " << name << " does not exist";
	m_UniformLocationCache[name] = location;
	return location;
}