#pragma once

#include "Horyzen/Renderer/IndexBuffer.h"

namespace Horyzen {

	class OpenGLIndexBuffer : public IndexBuffer
	{
	public:

		OpenGLIndexBuffer(u32* p_indices, u32 p_count);
		virtual ~OpenGLIndexBuffer();

		virtual void Bind() const override;
		virtual void Unbind() const override;

		virtual u32 GetCount() const override;
		virtual void SetCount(u32 p_count) override;

	private:

		u32 m_ID = 0;
		u32 m_Count = 0;

	};

}

