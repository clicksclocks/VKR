#pragma once

#include "Horyzen/Core.h"

namespace Horyzen {

	class HORYZEN_OBJECT IndexBuffer
	{
	public:
		
		virtual ~IndexBuffer() = default;

		virtual void Bind() const = 0;
		virtual void Unbind() const = 0;

		virtual u32 GetCount() const = 0;
		virtual void SetCount(u32 p_count) = 0;

		static std::shared_ptr<IndexBuffer> Create(u32* p_indices, u32 p_count);

	};

}