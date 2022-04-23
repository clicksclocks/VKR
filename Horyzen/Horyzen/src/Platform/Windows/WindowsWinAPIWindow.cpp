#include "pchheader.h"
#include "WindowsWinAPIWindow.h"

#include <windows.h>

namespace Horyzen {

	LRESULT CALLBACK MainWindowCallback(
		HWND   hwnd,
		UINT   uMsg,
		WPARAM wParam,
		LPARAM lParam)
	{
		LRESULT result = 0;

		switch (uMsg)
		{
			case WM_PAINT:
			{
				
			} break;

			case WM_CLOSE:
			{

			} break;

			case WM_SIZE:
			{

			} break;

			case WM_ACTIVATEAPP:
			{

			} break;

			case WM_DESTROY:
			{

			} break;

			default:
			{
				result = DefWindowProc(hwnd, uMsg, wParam, lParam);
			} break;
		}

		return result;
	}

	WindowsWinAPIWindow::WindowsWinAPIWindow(const WindowProperties& p_propetries)
	{
		m_WindowData.Width = p_propetries.Width;
		m_WindowData.Height = p_propetries.Height;
		m_WindowData.VSync = p_propetries.VSync;
		m_WindowData.Name = p_propetries.Name;
		Init();
	}

	void WindowsWinAPIWindow::Init()
	{
		HINSTANCE hInstance = GetModuleHandle(0);

		typedef struct tagWNDCLASSA {
			UINT      style;
			WNDPROC   lpfnWndProc;
			int       cbClsExtra;
			int       cbWndExtra;
			HINSTANCE hInstance;
			HICON     hIcon;
			HCURSOR   hCursor;
			HBRUSH    hbrBackground;
			LPCSTR    lpszMenuName;
			LPCSTR    lpszClassName;
		} WNDCLASSA, * PWNDCLASSA, * NPWNDCLASSA, * LPWNDCLASSA;

		WNDCLASSA windowsClass = {};
		windowsClass.style = CS_OWNDC | CS_HREDRAW | CS_VREDRAW;
		windowsClass.lpfnWndProc;
		windowsClass.cbClsExtra;
		windowsClass.cbWndExtra;
		windowsClass.hInstance;
		windowsClass.hIcon;
		windowsClass.hCursor;
		windowsClass.hbrBackground;
		windowsClass.lpszMenuName;
		windowsClass.lpszClassName;

	}

	WindowsWinAPIWindow::~WindowsWinAPIWindow()
	{
	}

	void WindowsWinAPIWindow::Update()
	{
	}

	void WindowsWinAPIWindow::SetEventCallback(const EventCallback& p_callback)
	{
	}

	void* WindowsWinAPIWindow::GetNativeWindow() const
	{
		return nullptr;
	}

	u32 WindowsWinAPIWindow::GetWidth() const
	{
		return u32();
	}

	u32 WindowsWinAPIWindow::GetHeight() const
	{
		return u32();
	}

	void WindowsWinAPIWindow::SetVSync(bool p_enable)
	{
	}

	bool WindowsWinAPIWindow::IsVSync() const
	{
		return false;
	}
}