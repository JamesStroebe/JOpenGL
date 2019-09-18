#pragma once

#include "Buffer.h"

#include <vector>

class VertexArray {
public:
	VertexArray();
	~VertexArray();

	void Bind() const;
	void Unbind() const;

	void AddVertexBuffer(VertexBuffer& vertexBuffers);
	void SetIndexBuffer(IndexBuffer& indexBuffer);

	std::vector<VertexBuffer>& GetVertexBuffers() { return m_VertexBuffers; }
	IndexBuffer GetIndexBuffer() { return m_IndexBuffer; }
private:

	RendererID m_RendererID;
	uint32_t m_VertexBufferIndex = 0;
	std::vector<VertexBuffer> m_VertexBuffers;
	IndexBuffer m_IndexBuffer;
};
