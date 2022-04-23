#pragma once

#include "Horyzen/Window.h"

namespace Horyzen {

	class WindowsWinAPIWindow : public Window
	{
	public:

		WindowsWinAPIWindow(const WindowProperties& p_propetries);
		virtual ~WindowsWinAPIWindow();

		virtual void Update() override;
		virtual void SetEventCallback(const EventCallback& p_callback) override;
		virtual void* GetNativeWindow() const override;

		virtual u32 GetWidth() const override;
		virtual u32 GetHeight() const override;
		virtual void SetVSync(bool p_enable) override;
		virtual bool IsVSync() const override;

	private:

		void Init();

	private:

		struct WindowData
		{
			u32 Width;
			u32 Height;
			bool VSync;
			std::string Name;
			EventCallback Callback;
		};

		WindowData m_WindowData;

	};

}