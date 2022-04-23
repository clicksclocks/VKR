#pragma once

#include "Horyzen/Renderer/RendererAPI.h"

namespace Horyzen {

	class OpenGLRendererAPI : public RendererAPI
	{
	public:

		virtual void SetClearColor(f32 p_r, f32 p_g, f32 p_b, f32 p_a) override;
		virtual void Clear() override;
		virtual void DrawIndexed(const std::shared_ptr<VertexArray>& p_vertexArray, BasePrimitive p_primitive) override;
		virtual void ReadPixelFromScreenBuffer(u32 x, u32 y, void* data) override;
	};

}

