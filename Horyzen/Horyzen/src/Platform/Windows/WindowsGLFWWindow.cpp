#include "pchheader.h"

#include "WindowsGLFWWindow.h"
#include "Horyzen/Logger.h"
#include "Horyzen/Events/KeyboardEvents.h"
#include "Horyzen/Events/MouseEvents.h"
#include "Horyzen/Events/ApplicationEvents.h"
#include "Platform/OpenGL/OpenGLRenderingContext.h"

#include "Horyzen/Profiler/Profiler.h"

namespace Horyzen {

	WindowsGLFWWindow::WindowsGLFWWindow(const WindowProperties& p_propetries)
	{
		m_WindowData.Width  = p_propetries.Width;
		m_WindowData.Height = p_propetries.Height;
		m_WindowData.VSync = p_propetries.VSync;
		m_WindowData.Name = p_propetries.Name;
		Init();
	}

	WindowsGLFWWindow::~WindowsGLFWWindow()
	{
		glfwDestroyWindow(m_GLFWWindowHandle);
		glfwTerminate();
	}

	void Horyzen::WindowsGLFWWindow::Init()
	{
		HORYZEN_PROFILE_FUNCTION();
		// Initializing GLFW
		i32 glfwInitSuccess = glfwInit();
		HORYZEN_ASSERT(glfwInitSuccess, "GLFW failed to initialize!");
		HORYZEN_LOG_INFO("GLFW is successfully initialized!");
		FillKeyAndMouseButtonMaps();

		// Getting monitor info
		m_GLFWPrimaryMonitorHandle = glfwGetPrimaryMonitor();
		const char* primaryMonitorName = glfwGetMonitorName(m_GLFWPrimaryMonitorHandle);
		const GLFWvidmode* primaryMonitorMode = glfwGetVideoMode(m_GLFWPrimaryMonitorHandle);

		HORYZEN_LOG_INFO(
			"Primary monitor: {0}\n   Resolution: {1}x{2}\n   Refresh rate: {3}", 
			primaryMonitorName, 
			primaryMonitorMode->width,
			primaryMonitorMode->height,
			primaryMonitorMode->refreshRate);

		// Creating window
		glfwWindowHint(GLFW_SAMPLES, 4);
		m_GLFWWindowHandle = glfwCreateWindow(
			m_WindowData.Width, 
			m_WindowData.Height,
			m_WindowData.Name.c_str(),
			NULL, NULL);
		HORYZEN_ASSERT(m_GLFWWindowHandle, "Window or OpenGL context creation failed!");
		HORYZEN_LOG_INFO("Window and OpenGL context are successfully initialized!");
		glfwSetWindowUserPointer(m_GLFWWindowHandle, &m_WindowData);

		m_RenderingContext = new OpenGLRenderingContext(m_GLFWWindowHandle);
		m_RenderingContext->Init();

		glfwSwapInterval( m_WindowData.VSync ? 1 : 0);

		HORYZEN_LOG_ERROR("Hello, Alex! Nice to see you here again :) I hope you are having a good day!");

		// Setting callbacks
		glfwSetKeyCallback(m_GLFWWindowHandle, 
			[](GLFWwindow* p_window, i32 p_key, i32 p_scancode, i32 p_action, i32 p_mods) {
				if (p_key == GLFW_KEY_UNKNOWN) return;
				WindowData* windowData = reinterpret_cast<WindowData*>(glfwGetWindowUserPointer(p_window));
				if (p_action == GLFW_PRESS) {
					KeyPressedEvent event(s_GLFW_HoryzenKeyMap[p_key]);
					windowData->Callback(event);

				} else if (p_action == GLFW_RELEASE) {
					KeyReleasedEvent event(s_GLFW_HoryzenKeyMap[p_key]);
					windowData->Callback(event);
				}
			}
		);

		glfwSetCursorPosCallback(m_GLFWWindowHandle, 
			[](GLFWwindow* p_window, f64 p_xPos, f64 p_yPos) {
				WindowData* windowData = reinterpret_cast<WindowData*>(glfwGetWindowUserPointer(p_window));
				MouseMovedEvent event(p_xPos, p_yPos);
				windowData->Callback(event);
			}
		);

		glfwSetCursorEnterCallback(m_GLFWWindowHandle,
			[](GLFWwindow* p_window, i32 p_entered) {
				WindowData* windowData = reinterpret_cast<WindowData*>(glfwGetWindowUserPointer(p_window));
				
				if (p_entered) {
					MouseEnteredWindowEvent event;
					windowData->Callback(event);
				} else {
					MouseLeftWindowEvent event;
					windowData->Callback(event);
				}
			}
		);

		glfwSetMouseButtonCallback(m_GLFWWindowHandle,
			[](GLFWwindow* p_window, i32 p_button, i32 p_action, i32 p_mods) {
				WindowData* windowData = reinterpret_cast<WindowData*>(glfwGetWindowUserPointer(p_window));
				if (p_action == GLFW_PRESS) {
					MouseButtonPressedEvent event(s_GLFW_HoryzenMouseButtonMap[p_button]);
					windowData->Callback(event);
				} else {
					MouseButtonReleasedEvent event(s_GLFW_HoryzenMouseButtonMap[p_button]);
					windowData->Callback(event);
				}
			}
		);

		glfwSetScrollCallback(m_GLFWWindowHandle,
			[](GLFWwindow* p_window, f64 p_xOffset, f64 p_yOffset) {
				WindowData* windowData = reinterpret_cast<WindowData*>(glfwGetWindowUserPointer(p_window));
				MouseScrolledEvent event(p_xOffset, p_yOffset);
				windowData->Callback(event);
			}
		);

		glfwSetWindowCloseCallback(m_GLFWWindowHandle,
			[](GLFWwindow* p_window) {
				WindowData* windowData = reinterpret_cast<WindowData*>(glfwGetWindowUserPointer(p_window));
				WindowCloseRequestedEvent event;
				windowData->Callback(event);
			}
		);

		glfwSetWindowSizeCallback(m_GLFWWindowHandle,
			[](GLFWwindow* p_window, i32 p_width, i32 p_height) {
				WindowData* windowData = reinterpret_cast<WindowData*>(glfwGetWindowUserPointer(p_window));
				windowData->Width  = p_width;
				windowData->Height = p_height;
				WindowResizedEvent event(p_width, p_height);
				windowData->Callback(event);

			}
		);

		glfwSetWindowPosCallback(m_GLFWWindowHandle,
			[](GLFWwindow* p_window, i32 p_xPos, i32 p_yPos) {
				WindowData* windowData = reinterpret_cast<WindowData*>(glfwGetWindowUserPointer(p_window));
				WindowMovedEvent event(p_xPos, p_yPos);
				windowData->Callback(event);
			}
		);


		glfwSetJoystickCallback(
			[](i32 p_jid, i32 p_event) {
				if (p_event == GLFW_CONNECTED)
				{
					APP_LOG_TRACE("Joystick CONNECTED. ID: {0}", p_jid);
					const char* jname = glfwGetJoystickName(p_jid);
					APP_LOG_TRACE("Joystick name: {0}", jname);

				}
				else if (p_event == GLFW_DISCONNECTED)
				{
					APP_LOG_TRACE("Joystick DISCONNECTED. ID: {0}", p_jid);
				}
			}
		);
	}
	
	void WindowsGLFWWindow::Update()
	{
		m_RenderingContext->SwapBuffers();
		glfwPollEvents();
	}

	void WindowsGLFWWindow::SetEventCallback(const EventCallback& p_callback)
	{
		m_WindowData.Callback = p_callback;
	}

	void* WindowsGLFWWindow::GetNativeWindow() const
	{
		return m_GLFWWindowHandle;
	}

	u32 WindowsGLFWWindow::GetWidth() const
	{
		return m_WindowData.Width;
	}

	u32 WindowsGLFWWindow::GetHeight() const
	{
		return m_WindowData.Height;
	}

	void WindowsGLFWWindow::SetVSync(bool p_enable)
	{
		m_WindowData.VSync = p_enable;
		glfwSwapInterval(m_WindowData.VSync ? 1 : 0);
	}

	bool WindowsGLFWWindow::IsVSync() const
	{
		return m_WindowData.VSync;
	}

	void WindowsGLFWWindow::SetViewPortForRenderingContext(u32 p_x, u32 p_y, u32 p_width, u32 p_height)
	{
		m_RenderingContext->ViewPort(p_x, p_y, p_width, p_height);
	}

	std::array<Key, GLFW_KEY_LAST + 1> WindowsGLFWWindow::s_GLFW_HoryzenKeyMap;
	std::array<MouseButton, GLFW_MOUSE_BUTTON_LAST + 1> WindowsGLFWWindow::s_GLFW_HoryzenMouseButtonMap;

	void WindowsGLFWWindow::FillKeyAndMouseButtonMaps()
	{
		s_GLFW_HoryzenKeyMap[GLFW_KEY_SPACE] = Key::Space;
		s_GLFW_HoryzenKeyMap[GLFW_KEY_APOSTROPHE] = Key::Apostrophe;
		s_GLFW_HoryzenKeyMap[GLFW_KEY_COMMA] = Key::Comma;
		s_GLFW_HoryzenKeyMap[GLFW_KEY_MINUS] = Key::Minus;
		s_GLFW_HoryzenKeyMap[GLFW_KEY_PERIOD] = Key::Period;
		s_GLFW_HoryzenKeyMap[GLFW_KEY_SLASH] = Key::Slash;
		s_GLFW_HoryzenKeyMap[GLFW_KEY_0] = Key::Num0;
		s_GLFW_HoryzenKeyMap[GLFW_KEY_1] = Key::Num1;
		s_GLFW_HoryzenKeyMap[GLFW_KEY_2] = Key::Num2;
		s_GLFW_HoryzenKeyMap[GLFW_KEY_3] = Key::Num3;
		s_GLFW_HoryzenKeyMap[GLFW_KEY_4] = Key::Num4;
		s_GLFW_HoryzenKeyMap[GLFW_KEY_5] = Key::Num5;
		s_GLFW_HoryzenKeyMap[GLFW_KEY_6] = Key::Num6;
		s_GLFW_HoryzenKeyMap[GLFW_KEY_7] = Key::Num7;
		s_GLFW_HoryzenKeyMap[GLFW_KEY_8] = Key::Num8;
		s_GLFW_HoryzenKeyMap[GLFW_KEY_9] = Key::Num9;
		s_GLFW_HoryzenKeyMap[GLFW_KEY_SEMICOLON] = Key::Semicolon;
		s_GLFW_HoryzenKeyMap[GLFW_KEY_EQUAL] = Key::Equal;
		s_GLFW_HoryzenKeyMap[GLFW_KEY_A] = Key::A;
		s_GLFW_HoryzenKeyMap[GLFW_KEY_B] = Key::B;
		s_GLFW_HoryzenKeyMap[GLFW_KEY_C] = Key::C;
		s_GLFW_HoryzenKeyMap[GLFW_KEY_D] = Key::D;
		s_GLFW_HoryzenKeyMap[GLFW_KEY_E] = Key::E;
		s_GLFW_HoryzenKeyMap[GLFW_KEY_F] = Key::F;
		s_GLFW_HoryzenKeyMap[GLFW_KEY_G] = Key::G;
		s_GLFW_HoryzenKeyMap[GLFW_KEY_H] = Key::H;
		s_GLFW_HoryzenKeyMap[GLFW_KEY_I] = Key::I;
		s_GLFW_HoryzenKeyMap[GLFW_KEY_J] = Key::J;
		s_GLFW_HoryzenKeyMap[GLFW_KEY_K] = Key::K;
		s_GLFW_HoryzenKeyMap[GLFW_KEY_L] = Key::L;
		s_GLFW_HoryzenKeyMap[GLFW_KEY_M] = Key::M;
		s_GLFW_HoryzenKeyMap[GLFW_KEY_N] = Key::N;
		s_GLFW_HoryzenKeyMap[GLFW_KEY_O] = Key::O;
		s_GLFW_HoryzenKeyMap[GLFW_KEY_P] = Key::P;
		s_GLFW_HoryzenKeyMap[GLFW_KEY_Q] = Key::Q;
		s_GLFW_HoryzenKeyMap[GLFW_KEY_R] = Key::R;
		s_GLFW_HoryzenKeyMap[GLFW_KEY_S] = Key::S;
		s_GLFW_HoryzenKeyMap[GLFW_KEY_T] = Key::T;
		s_GLFW_HoryzenKeyMap[GLFW_KEY_U] = Key::U;
		s_GLFW_HoryzenKeyMap[GLFW_KEY_V] = Key::V;
		s_GLFW_HoryzenKeyMap[GLFW_KEY_W] = Key::W;
		s_GLFW_HoryzenKeyMap[GLFW_KEY_X] = Key::X;
		s_GLFW_HoryzenKeyMap[GLFW_KEY_Y] = Key::Y;
		s_GLFW_HoryzenKeyMap[GLFW_KEY_Z] = Key::Z;
		s_GLFW_HoryzenKeyMap[GLFW_KEY_LEFT_BRACKET] = Key::LeftBracket;
		s_GLFW_HoryzenKeyMap[GLFW_KEY_BACKSLASH] = Key::Backslash;
		s_GLFW_HoryzenKeyMap[GLFW_KEY_RIGHT_BRACKET] = Key::RightBracket;
		s_GLFW_HoryzenKeyMap[GLFW_KEY_GRAVE_ACCENT] = Key::GraveAccent;
		s_GLFW_HoryzenKeyMap[GLFW_KEY_WORLD_1] = Key::WORLD_1;
		s_GLFW_HoryzenKeyMap[GLFW_KEY_WORLD_2] = Key::WORLD_2;
		s_GLFW_HoryzenKeyMap[GLFW_KEY_ESCAPE] = Key::Escape;
		s_GLFW_HoryzenKeyMap[GLFW_KEY_ENTER] = Key::Enter;
		s_GLFW_HoryzenKeyMap[GLFW_KEY_TAB] = Key::Tab;
		s_GLFW_HoryzenKeyMap[GLFW_KEY_BACKSPACE] = Key::Backspace;
		s_GLFW_HoryzenKeyMap[GLFW_KEY_INSERT] = Key::Insert;
		s_GLFW_HoryzenKeyMap[GLFW_KEY_DELETE] = Key::Delete;
		s_GLFW_HoryzenKeyMap[GLFW_KEY_RIGHT] = Key::Right;
		s_GLFW_HoryzenKeyMap[GLFW_KEY_LEFT] = Key::Left;
		s_GLFW_HoryzenKeyMap[GLFW_KEY_DOWN] = Key::Down;
		s_GLFW_HoryzenKeyMap[GLFW_KEY_UP] = Key::Up;
		s_GLFW_HoryzenKeyMap[GLFW_KEY_PAGE_UP] = Key::PageUp;
		s_GLFW_HoryzenKeyMap[GLFW_KEY_PAGE_DOWN] = Key::PageDown;
		s_GLFW_HoryzenKeyMap[GLFW_KEY_HOME] = Key::Home;
		s_GLFW_HoryzenKeyMap[GLFW_KEY_END] = Key::End;
		s_GLFW_HoryzenKeyMap[GLFW_KEY_CAPS_LOCK] = Key::CapsLock;
		s_GLFW_HoryzenKeyMap[GLFW_KEY_SCROLL_LOCK] = Key::ScrollLock;
		s_GLFW_HoryzenKeyMap[GLFW_KEY_NUM_LOCK] = Key::NumLock;
		s_GLFW_HoryzenKeyMap[GLFW_KEY_PRINT_SCREEN] = Key::PrintScreen;
		s_GLFW_HoryzenKeyMap[GLFW_KEY_PAUSE] = Key::Pause;
		s_GLFW_HoryzenKeyMap[GLFW_KEY_F1] = Key::F1;
		s_GLFW_HoryzenKeyMap[GLFW_KEY_F2] = Key::F2;
		s_GLFW_HoryzenKeyMap[GLFW_KEY_F3] = Key::F3;
		s_GLFW_HoryzenKeyMap[GLFW_KEY_F4] = Key::F4;
		s_GLFW_HoryzenKeyMap[GLFW_KEY_F5] = Key::F5;
		s_GLFW_HoryzenKeyMap[GLFW_KEY_F6] = Key::F6;
		s_GLFW_HoryzenKeyMap[GLFW_KEY_F7] = Key::F7;
		s_GLFW_HoryzenKeyMap[GLFW_KEY_F8] = Key::F8;
		s_GLFW_HoryzenKeyMap[GLFW_KEY_F9] = Key::F9;
		s_GLFW_HoryzenKeyMap[GLFW_KEY_F10] = Key::F10;
		s_GLFW_HoryzenKeyMap[GLFW_KEY_F11] = Key::F11;
		s_GLFW_HoryzenKeyMap[GLFW_KEY_F12] = Key::F12;
		s_GLFW_HoryzenKeyMap[GLFW_KEY_F13] = Key::F13;
		s_GLFW_HoryzenKeyMap[GLFW_KEY_F14] = Key::F14;
		s_GLFW_HoryzenKeyMap[GLFW_KEY_F15] = Key::F15;
		s_GLFW_HoryzenKeyMap[GLFW_KEY_F16] = Key::F16;
		s_GLFW_HoryzenKeyMap[GLFW_KEY_F17] = Key::F17;
		s_GLFW_HoryzenKeyMap[GLFW_KEY_F18] = Key::F18;
		s_GLFW_HoryzenKeyMap[GLFW_KEY_F19] = Key::F19;
		s_GLFW_HoryzenKeyMap[GLFW_KEY_F20] = Key::F20;
		s_GLFW_HoryzenKeyMap[GLFW_KEY_F22] = Key::F21;
		s_GLFW_HoryzenKeyMap[GLFW_KEY_F23] = Key::F22;
		s_GLFW_HoryzenKeyMap[GLFW_KEY_F21] = Key::F23;
		s_GLFW_HoryzenKeyMap[GLFW_KEY_F24] = Key::F24;
		s_GLFW_HoryzenKeyMap[GLFW_KEY_F25] = Key::F25;
		s_GLFW_HoryzenKeyMap[GLFW_KEY_KP_0] = Key::Numpad0;
		s_GLFW_HoryzenKeyMap[GLFW_KEY_KP_1] = Key::Numpad1;
		s_GLFW_HoryzenKeyMap[GLFW_KEY_KP_2] = Key::Numpad2;
		s_GLFW_HoryzenKeyMap[GLFW_KEY_KP_3] = Key::Numpad3;
		s_GLFW_HoryzenKeyMap[GLFW_KEY_KP_4] = Key::Numpad4;
		s_GLFW_HoryzenKeyMap[GLFW_KEY_KP_5] = Key::Numpad5;
		s_GLFW_HoryzenKeyMap[GLFW_KEY_KP_6] = Key::Numpad6;
		s_GLFW_HoryzenKeyMap[GLFW_KEY_KP_7] = Key::Numpad7;
		s_GLFW_HoryzenKeyMap[GLFW_KEY_KP_8] = Key::Numpad8;
		s_GLFW_HoryzenKeyMap[GLFW_KEY_KP_9] = Key::Numpad9;
		s_GLFW_HoryzenKeyMap[GLFW_KEY_KP_DECIMAL] = Key::NumpadDecimal;
		s_GLFW_HoryzenKeyMap[GLFW_KEY_KP_DIVIDE] = Key::NumpadDivide;
		s_GLFW_HoryzenKeyMap[GLFW_KEY_KP_MULTIPLY] = Key::NumpadMultiply;
		s_GLFW_HoryzenKeyMap[GLFW_KEY_KP_SUBTRACT] = Key::NumpadSubtract;
		s_GLFW_HoryzenKeyMap[GLFW_KEY_KP_ADD] = Key::NumpadAdd;
		s_GLFW_HoryzenKeyMap[GLFW_KEY_KP_ENTER] = Key::NumpadEnter;
		s_GLFW_HoryzenKeyMap[GLFW_KEY_KP_EQUAL] = Key::NumpadEqual;
		s_GLFW_HoryzenKeyMap[GLFW_KEY_LEFT_SHIFT] = Key::LeftShift;
		s_GLFW_HoryzenKeyMap[GLFW_KEY_LEFT_CONTROL] = Key::LeftControl;
		s_GLFW_HoryzenKeyMap[GLFW_KEY_LEFT_ALT] = Key::LeftAlt;
		s_GLFW_HoryzenKeyMap[GLFW_KEY_LEFT_SUPER] = Key::LeftSuper;
		s_GLFW_HoryzenKeyMap[GLFW_KEY_RIGHT_SHIFT] = Key::RightShift;
		s_GLFW_HoryzenKeyMap[GLFW_KEY_RIGHT_CONTROL] = Key::RightControl;
		s_GLFW_HoryzenKeyMap[GLFW_KEY_RIGHT_ALT] = Key::RightAlt;
		s_GLFW_HoryzenKeyMap[GLFW_KEY_RIGHT_SUPER] = Key::RightSuper;
		s_GLFW_HoryzenKeyMap[GLFW_KEY_MENU] = Key::Menu;

		s_GLFW_HoryzenMouseButtonMap[GLFW_MOUSE_BUTTON_1] = MouseButton::B1;
		s_GLFW_HoryzenMouseButtonMap[GLFW_MOUSE_BUTTON_2] = MouseButton::B2;
		s_GLFW_HoryzenMouseButtonMap[GLFW_MOUSE_BUTTON_3] = MouseButton::B3;
		s_GLFW_HoryzenMouseButtonMap[GLFW_MOUSE_BUTTON_4] = MouseButton::B4;
		s_GLFW_HoryzenMouseButtonMap[GLFW_MOUSE_BUTTON_5] = MouseButton::B5;
		s_GLFW_HoryzenMouseButtonMap[GLFW_MOUSE_BUTTON_6] = MouseButton::B6;
		s_GLFW_HoryzenMouseButtonMap[GLFW_MOUSE_BUTTON_7] = MouseButton::B7;
		s_GLFW_HoryzenMouseButtonMap[GLFW_MOUSE_BUTTON_8] = MouseButton::B8;
	}
}