#pragma once

#include "Horyzen/Core.h"

#include "VertexBufferLayout.h"

namespace Horyzen {

	class HORYZEN_OBJECT VertexBuffer
	{
	public:

		virtual ~VertexBuffer() = default;

		virtual void Bind() const = 0;
		virtual void Unbind() const = 0;
		virtual void UpdateSubData(u64 p_offsetInBytes, u64 p_byteSize, void* p_data) = 0;

		virtual void SetLayout(const VertexBufferLayout& p_layout) = 0;
		virtual const VertexBufferLayout& GetLayout() const = 0;

		static std::shared_ptr<VertexBuffer> Create(f32* p_vertices, u32 p_byteSize);
		static std::shared_ptr<VertexBuffer> Create(u32 p_byteSize);

	};

}