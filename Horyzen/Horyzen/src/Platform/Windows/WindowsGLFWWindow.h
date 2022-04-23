#pragma once
#include "Horyzen/Window.h"
#include "Horyzen/Input/KeyAndMouseValues.h"

//struct GLFWwindow;
//struct GLFWmonitor;

#include <GLFW/glfw3.h>



namespace Horyzen {

	class RenderingContext;
	class HORYZEN_OBJECT WindowsGLFWWindow : public Window
	{
	public:

		WindowsGLFWWindow(const WindowProperties& p_propetries);
		virtual ~WindowsGLFWWindow();

		void Init();

		virtual void Update() override;
		virtual void SetEventCallback(const EventCallback& p_callback) override;
		virtual void* GetNativeWindow() const override;
		virtual u32 GetWidth() const override;
		virtual u32 GetHeight() const override;

		virtual void SetVSync(bool p_enable) override;
		virtual bool IsVSync() const override;
		virtual void SetViewPortForRenderingContext(u32 p_x, u32 p_y, u32 p_width, u32 p_height) override;

	private:

		GLFWwindow*  m_GLFWWindowHandle = nullptr;
		GLFWmonitor* m_GLFWPrimaryMonitorHandle = nullptr;
		RenderingContext* m_RenderingContext;

		struct WindowData
		{
			u32 Width;
			u32 Height;
			bool VSync;
			std::string Name;
			EventCallback Callback;
		};

		WindowData m_WindowData;

		static std::array<Key, GLFW_KEY_LAST + 1> s_GLFW_HoryzenKeyMap;
		static std::array<MouseButton, GLFW_MOUSE_BUTTON_LAST + 1> s_GLFW_HoryzenMouseButtonMap;
		static void FillKeyAndMouseButtonMaps();

	};

}

