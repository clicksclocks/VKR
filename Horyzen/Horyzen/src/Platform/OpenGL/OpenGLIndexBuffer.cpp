#include "pchheader.h"
#include "OpenGLIndexBuffer.h"

#include "Horyzen/Profiler/Profiler.h"

#include "../../Horyzen/Logger.h"

#include <glad/glad.h>

namespace Horyzen {

	OpenGLIndexBuffer::OpenGLIndexBuffer(u32* p_indices, u32 p_count)
	{
		HORYZEN_PROFILE_FUNCTION();

		glCreateBuffers(1, &m_ID);
		HORYZEN_ASSERT(m_ID, "Failed to create OpenGL index buffer!");
		glNamedBufferData(m_ID, p_count * sizeof(u32), p_indices, GL_STATIC_DRAW);
		m_Count = p_count;
	}

	OpenGLIndexBuffer::~OpenGLIndexBuffer()
	{
		glDeleteBuffers(1, &m_ID);
	}

	void OpenGLIndexBuffer::Bind() const
	{
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ID);
	}

	void OpenGLIndexBuffer::Unbind() const
	{
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	}

	u32 OpenGLIndexBuffer::GetCount() const
	{
		return m_Count;
	}

	void OpenGLIndexBuffer::SetCount(u32 p_count)
	{
		m_Count = p_count;
	}

}
