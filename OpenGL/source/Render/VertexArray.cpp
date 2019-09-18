#include "VertexArray.h"

#include "GLError.h"

static GLenum ShaderDataTypeToOpenGL(ShaderDataType type) {
	switch (type) {
	case ShaderDataType::Float:    return GL_FLOAT;
	case ShaderDataType::Float2:   return GL_FLOAT;
	case ShaderDataType::Float3:   return GL_FLOAT;
	case ShaderDataType::Float4:   return GL_FLOAT;
	case ShaderDataType::Mat3:     return GL_FLOAT;
	case ShaderDataType::Mat4:     return GL_FLOAT;
	case ShaderDataType::Int:      return GL_INT;
	case ShaderDataType::Int2:     return GL_INT;
	case ShaderDataType::Int3:     return GL_INT;
	case ShaderDataType::Int4:     return GL_INT;
	case ShaderDataType::Bool:     return GL_BOOL;
	}

	return 0;
}

VertexArray::VertexArray() {
	GLCall(glGenVertexArrays(1, &m_RendererID));
	GLCall(glBindVertexArray(m_RendererID));
}

VertexArray::~VertexArray() {
	GLCall(glDeleteVertexArrays(1, &m_RendererID));
}

void VertexArray::Bind() const {
	GLCall(glBindVertexArray(m_RendererID));
}

void VertexArray::Unbind() const {
	GLCall(glBindVertexArray(0));
}

void VertexArray::AddVertexBuffer(VertexBuffer& vertexBuffer) {
	Bind();
	vertexBuffer.Bind();


	const auto& layout = vertexBuffer.GetLayout();
	for (const auto& element : layout) {
		GLCall(glEnableVertexAttribArray(m_VertexBufferIndex));
		GLCall(glVertexAttribPointer(m_VertexBufferIndex,
			element.GetComponentCount(),
			ShaderDataTypeToOpenGL(element.Type),
			element.Normalized ? GL_TRUE : GL_FALSE,
			layout.GetStride(),
			(const void*)(intptr_t)element.Offset));
		m_VertexBufferIndex++;
	}

	m_VertexBuffers.push_back(vertexBuffer);
}

void VertexArray::SetIndexBuffer(IndexBuffer& indexBuffer) {
	indexBuffer.Bind();
	GLCall(glBindVertexArray(m_RendererID));
	m_IndexBuffer = indexBuffer;
	
}