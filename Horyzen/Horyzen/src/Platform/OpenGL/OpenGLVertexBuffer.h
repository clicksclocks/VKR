#pragma once

#include "Horyzen/Renderer/VertexBuffer.h"

namespace Horyzen {

	class OpenGLVertexBuffer : public VertexBuffer
	{
	public:

		OpenGLVertexBuffer(f32* p_vertices, u32 p_byteSize);
		OpenGLVertexBuffer(u32 p_byteSize);
		virtual ~OpenGLVertexBuffer();

		virtual void Bind() const override;
		virtual void Unbind() const override;
		virtual void UpdateSubData(u64 p_offsetInBytes, u64 p_byteSize, void* p_data) override;

		virtual void SetLayout(const VertexBufferLayout& p_layout) override;
		virtual const VertexBufferLayout& GetLayout() const override;

	private:

		u32 m_ID = 0;
		u32 m_ByteSize = 0;
		VertexBufferLayout m_Layout;

	};

}

