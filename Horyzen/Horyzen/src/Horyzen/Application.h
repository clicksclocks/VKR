#pragma once
#include "pchheader.h"

#include "Core.h"

#include "Window.h"
#include "Horyzen/Events/KeyboardEvents.h"
#include "Horyzen/Events/MouseEvents.h"
#include "Horyzen/Events/ApplicationEvents.h"
#include "Horyzen/Input/InputTracker.h"

#include "LayerStack.h"
#include "ImGui/ImGuiLayer.h"

namespace Horyzen {

	class HORYZEN_OBJECT Application
	{
	public:

		Application();
		virtual ~Application() = default;

		void AttachLayer(Layer* p_layer);
		void AttachOverlay(Layer* p_overlay);

		virtual void Init();
		virtual void Run();

		void SetUpCallbackForWindow(std::shared_ptr<Window> p_window);

		Window& GetWindow() { return *m_Window; }

		ImGuiLayer* GetImGuiLayer() { return m_ImGuiLayer; }

		static Application& Get() { return *s_Instance; }

	private:

		void WindowEventCallback(Event& p_event);
		void SendEventToLayers(Event& p_event);

	private:

		LayerStack m_LayerStack;
		ImGuiLayer* m_ImGuiLayer;

		std::shared_ptr<Window> m_Window;
		bool m_Running = true;

	private:

		static Application* s_Instance;

	};

}

