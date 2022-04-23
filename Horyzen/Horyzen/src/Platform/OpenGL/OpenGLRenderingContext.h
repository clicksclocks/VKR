#pragma once

#include "Horyzen/Renderer/RenderingContext.h"

struct GLFWwindow;
namespace Horyzen {

	class OpenGLRenderingContext : public RenderingContext
	{
	public:

		OpenGLRenderingContext(GLFWwindow* p_windowHandle);

		virtual void Init() override;
		virtual void SwapBuffers() override;
		virtual void ViewPort(u32 p_x, u32 p_y, u32 p_width, u32 p_height) override;

	private:

		GLFWwindow* m_GLFWWindowHandle;

	};

}

