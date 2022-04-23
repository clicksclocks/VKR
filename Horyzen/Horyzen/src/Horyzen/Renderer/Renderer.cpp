#include "pchheader.h"

#include "Renderer.h"

namespace Horyzen {

	void Renderer::BeginScene()
	{
	}

	void Renderer::EndScene()
	{
	}

	void Renderer::Submit(const std::shared_ptr<VertexArray>& p_vertexArray, BasePrimitive p_primitive)
	{
		p_vertexArray->Bind();
		RenderCommand::DrawIndexed(p_vertexArray, p_primitive);
	}

}