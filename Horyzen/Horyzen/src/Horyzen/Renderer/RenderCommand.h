#pragma once

#include "RendererAPI.h"
#include "../Math/Vec4.h"

namespace Horyzen {

	class HORYZEN_OBJECT RenderCommand
	{
	public:

		inline static void DrawIndexed(const std::shared_ptr<VertexArray>& p_vertexArray, BasePrimitive p_primitive)
		{
			s_RendererAPI->DrawIndexed(p_vertexArray, p_primitive);
		}

		inline static void SetClearColor(f32 p_r, f32 p_g, f32 p_b, f32 p_a)
		{
			s_RendererAPI->SetClearColor(p_r, p_g, p_b, p_a);
		}

		inline static void Clear()
		{
			s_RendererAPI->Clear();
		}

		inline static void ReadPixelFromScreenBuffer(u32 x, u32 y, void* data)
		{
			return s_RendererAPI->ReadPixelFromScreenBuffer(x, y, data);
		}

	private:

		static RendererAPI* s_RendererAPI;

	};

}

