#pragma once
#include "pchheader.h"

#include "Core.h"

namespace Horyzen {

	struct HORYZEN_OBJECT WindowProperties
	{
		u32 Width{ 640 };
		u32 Height{ 480 };
		bool VSync{ true };
		std::string Name {"Window"};
	};


	class Event;
	class HORYZEN_OBJECT Window
	{
	public:

		virtual ~Window() = default;

		using EventCallback = std::function<void(Event&)>;
		
		virtual void Update() = 0;
		virtual void SetEventCallback(const EventCallback& p_callback) = 0;
		virtual void* GetNativeWindow() const = 0;

		virtual u32 GetWidth() const = 0;
		virtual u32 GetHeight() const = 0;
		virtual void SetVSync(bool p_enable) = 0;
		virtual bool IsVSync() const = 0;
		virtual void SetViewPortForRenderingContext(u32 p_x, u32 p_y, u32 p_width, u32 p_height) = 0;

		/// <summary>
		/// Creates a Window objecs. Hides platform specific code.
		/// </summary>
		/// <param name="p_properties"> Window properties (Widht, Height, Name) </param>
		/// <returns> Shared pointer to the Window object </returns>
		static std::shared_ptr<Window> Create(const WindowProperties& p_properties);

	};

}
