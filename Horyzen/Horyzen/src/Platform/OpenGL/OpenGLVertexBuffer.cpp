#include "pchheader.h"
#include "OpenGLVertexBuffer.h"

#include <glad/glad.h>

#include "Horyzen/Profiler/Profiler.h"

namespace Horyzen {

	OpenGLVertexBuffer::OpenGLVertexBuffer(f32* p_vertices, u32 p_byteSize)
	{
		HORYZEN_PROFILE_FUNCTION();

		glCreateBuffers(1, &m_ID);
		HORYZEN_ASSERT(m_ID, "Failed to create OpenGL vertex buffer!");
		glNamedBufferData(m_ID, p_byteSize, p_vertices, GL_STATIC_DRAW);
		
		m_ByteSize = p_byteSize;
	}

	OpenGLVertexBuffer::OpenGLVertexBuffer(u32 p_byteSize)
	{
		glCreateBuffers(1, &m_ID);
		HORYZEN_ASSERT(m_ID, "Failed to create OpenGL vertex buffer!");
		glNamedBufferData(m_ID, p_byteSize, nullptr, GL_DYNAMIC_DRAW);
	}

	OpenGLVertexBuffer::~OpenGLVertexBuffer()
	{
		glDeleteBuffers(1, &m_ID);
	}

	void OpenGLVertexBuffer::Bind() const
	{
		glBindBuffer(GL_ARRAY_BUFFER, m_ID);
	}

	void OpenGLVertexBuffer::Unbind() const
	{
		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}

	void OpenGLVertexBuffer::UpdateSubData(u64 p_offsetInBytes, u64 p_byteSize, void* p_data)
	{
		glNamedBufferSubData(m_ID, p_offsetInBytes, p_byteSize, p_data);
	}

	void OpenGLVertexBuffer::SetLayout(const VertexBufferLayout& p_layout)
	{
		m_Layout = p_layout;
	}

	const VertexBufferLayout& OpenGLVertexBuffer::GetLayout() const
	{
		return m_Layout;
	}

}

