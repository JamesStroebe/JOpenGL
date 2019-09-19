#include "Texture.h"
#include "GLError.h"


#include <stb_image/stb_image.cpp>

#include <glad/glad.h>

Texture2D::Texture2D(const std::string& filepath) {
	int width, height, channels;
	stbi_set_flip_vertically_on_load(true);
	//Images are required to have rgba format for now
	m_ImageData.Data = stbi_load(filepath.c_str(), &width, &height, &channels, STBI_rgb_alpha);

	m_Width = width;
	m_Height = height;

	GLCall(glGenTextures(1, &m_RendererID));
	GLCall(glBindTexture(GL_TEXTURE_2D, m_RendererID));
	GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
	GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST));
	GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE));
	GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE));

	GLCall(glPixelStorei(GL_PACK_ROW_LENGTH, m_Width));
	GLCall(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, m_Width, m_Height, 0, GL_RGBA, GL_UNSIGNED_BYTE, m_ImageData.Data));
	GLCall(glBindTexture(GL_TEXTURE_2D, 0));

	stbi_image_free(m_ImageData.Data);
}

Texture2D::~Texture2D() {
	GLCall(glDeleteTextures(1, &m_RendererID));
}

void Texture2D::Bind(uint32_t slot) const {
	GLCall(glActiveTexture(GL_TEXTURE0 + slot));
	GLCall(glBindTexture(GL_TEXTURE_2D, m_RendererID));
}

void Texture2D::Unbind() const {
	GLCall(glBindTexture(GL_TEXTURE_2D, 0));
}



