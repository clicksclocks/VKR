#pragma once

#include "RenderCommand.h"

namespace Horyzen {

	class HORYZEN_OBJECT Renderer
	{
	public:

		static void BeginScene();
		static void EndScene();

		static void Submit(const std::shared_ptr<VertexArray>& p_vertexArray, BasePrimitive p_primitive);

		inline static RendererAPI::API GetAPI() { return RendererAPI::GetAPI(); };

	};

}