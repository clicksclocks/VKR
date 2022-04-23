#pragma once

#include "Horyzen/Renderer/VertexArray.h"

namespace Horyzen {

	class OpenGLVertexArray : public VertexArray
	{
	public:

		OpenGLVertexArray();
		virtual ~OpenGLVertexArray();

		virtual void Bind() const override;
		virtual void Unbind() const override;

		virtual void AddVertexBuffer(const std::shared_ptr<VertexBuffer>& p_vertexBuffer) override;
		virtual void SetIndexBuffer(const std::shared_ptr<IndexBuffer>& p_indexBuffer) override;

		virtual const std::shared_ptr<IndexBuffer>& GetIndexBuffer() const override;
		virtual const std::vector<std::shared_ptr<VertexBuffer>>& GetVertexBuffers() const override;

	private:

		u32 m_ID;
		std::vector< std::shared_ptr<VertexBuffer> > m_VertexBuffers;
		std::shared_ptr<IndexBuffer> m_IndexBuffer;

	};

}