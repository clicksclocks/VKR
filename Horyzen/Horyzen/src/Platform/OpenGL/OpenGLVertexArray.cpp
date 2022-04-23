#include "pchheader.h"
#include "OpenGLVertexArray.h"

#include <glad/glad.h>

namespace Horyzen {

	static GLenum ShaderDataTypeToOpenGLBaseType(ShaderDataType p_type)
	{
		switch (p_type)
		{
			case ShaderDataType::Float:   return GL_FLOAT;
			case ShaderDataType::Float2:  return GL_FLOAT;
			case ShaderDataType::Float3:  return GL_FLOAT;
			case ShaderDataType::Float4:  return GL_FLOAT;
			case ShaderDataType::Mat3:    return GL_FLOAT;
			case ShaderDataType::Mat4:    return GL_FLOAT;
			case ShaderDataType::Int:     return GL_INT;
			case ShaderDataType::Int2:    return GL_INT;
			case ShaderDataType::Int3:    return GL_INT;
			case ShaderDataType::Int4:    return GL_INT;
			case ShaderDataType::Bool:    return GL_BOOL;
			default:
			{
				HORYZEN_ASSERT(false, "Unknown ShaderDataType!");
				return 0;
			}
		}
	}

	OpenGLVertexArray::OpenGLVertexArray()
	{
		glCreateVertexArrays(1, &m_ID);
	}

	OpenGLVertexArray::~OpenGLVertexArray()
	{
		glDeleteVertexArrays(1, &m_ID);
	}

	void OpenGLVertexArray::Bind() const
	{
		glBindVertexArray(m_ID);
	}

	void OpenGLVertexArray::Unbind() const
	{
		glBindVertexArray(0);
	}

	void OpenGLVertexArray::AddVertexBuffer(const std::shared_ptr<VertexBuffer>& p_vertexBuffer)
	{
		glBindVertexArray(m_ID);
		p_vertexBuffer->Bind();

		u32 index = 0;
		for (auto& elem : p_vertexBuffer->GetLayout()) {
			glEnableVertexAttribArray(index);
			glVertexAttribPointer(index,
				elem.GetComponentCount(),
				ShaderDataTypeToOpenGLBaseType(elem.Type),
				elem.Normalized ? GL_TRUE : GL_FALSE,
				p_vertexBuffer->GetLayout().GetStride(),
				(const void*)elem.Offset);
			index++;
		}
		
		m_VertexBuffers.push_back(p_vertexBuffer);
	}

	void OpenGLVertexArray::SetIndexBuffer(const std::shared_ptr<IndexBuffer>& p_indexBuffer)
	{
		glBindVertexArray(m_ID);
		p_indexBuffer->Bind();

		m_IndexBuffer = p_indexBuffer;
	}

	const std::shared_ptr<IndexBuffer>& OpenGLVertexArray::GetIndexBuffer() const
	{
		return m_IndexBuffer;
	}

	const std::vector<std::shared_ptr<VertexBuffer>>& OpenGLVertexArray::GetVertexBuffers() const
	{
		return m_VertexBuffers;
	}

}