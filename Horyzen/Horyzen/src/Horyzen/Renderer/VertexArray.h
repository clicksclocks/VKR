#pragma once

#include "Horyzen/Core.h"

#include "Horyzen/Renderer/VertexBuffer.h"
#include "Horyzen/Renderer/IndexBuffer.h"

namespace Horyzen {

	class HORYZEN_OBJECT VertexArray
	{
	public:
		
		virtual void Bind() const = 0;
		virtual void Unbind() const = 0;

		virtual void AddVertexBuffer(const std::shared_ptr<VertexBuffer>& p_vertexBuffer) = 0;
		virtual void SetIndexBuffer(const std::shared_ptr<IndexBuffer>& p_indexBuffer) = 0;
		virtual const std::shared_ptr<IndexBuffer>& GetIndexBuffer() const = 0;
		virtual const std::vector<std::shared_ptr<VertexBuffer>>& GetVertexBuffers() const = 0;

		static std::shared_ptr<VertexArray> Create();

	};

}

