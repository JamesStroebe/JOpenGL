#pragma once

#include <string>

#include "Renderer.h"

#include "Utils/Buffer.h"

class Texture2D {
public:
	Texture2D(const std::string& path);
	~Texture2D();

	void Bind(uint32_t slot = 0) const;
	void Unbind() const;

	inline uint32_t GetWidth() const { return m_Width; }
	inline uint32_t GetHeight() const { return m_Height; }

private:
	RendererID m_RendererID;
	std::string m_Path;

	uint32_t m_Width, m_Height;

	Buffer m_ImageData;
};
