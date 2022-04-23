#include "pchheader.h"
#include "Window.h"

#include "Platform/Windows/WindowsGLFWWindow.h"

namespace Horyzen {

	std::shared_ptr<Window> Window::Create(const WindowProperties& p_properties)
	{
	#ifdef HORYZEN_WINDOWS_BUILD
		return std::make_shared<WindowsGLFWWindow>(p_properties);
	#else
		HORYZEN_LOG_ERROR("Current platform is not supported now!");
		return  nullptr;
	#endif
	}

}