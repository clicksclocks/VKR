#pragma once

#include "../Core.h"

namespace Horyzen {

	class RenderingContext
	{
	public:

		virtual void Init() = 0;
		virtual void SwapBuffers() = 0;
		virtual void ViewPort(u32 p_x, u32 p_y, u32 p_width, u32 p_height) = 0;

	};

}