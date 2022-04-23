#pragma once

#include "Horyzen/Core.h"

#include "VertexArray.h"

namespace Horyzen {

	enum class BasePrimitive
	{
		Points,
		Lines,
		LineStrip,
		Triangles,
		TriangleStrip
	};

	class RendererAPI
	{
	public:

		enum class API
		{
			None = 0,
			OpenGL = 1
		};

	public:

		virtual void SetClearColor(f32 p_r, f32 p_g, f32 p_b, f32 p_a) = 0;
		virtual void Clear() = 0;

		virtual void DrawIndexed(const std::shared_ptr<VertexArray>& p_vertexArray, BasePrimitive p_primitive) = 0;
		virtual void ReadPixelFromScreenBuffer(u32 x, u32 y, void* data) = 0;

		inline static API GetAPI() { return s_API; }

	private:

		static API s_API;

	};

}