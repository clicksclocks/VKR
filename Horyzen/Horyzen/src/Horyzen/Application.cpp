#include "pchheader.h"

#include "Application.h"

#include "Logger.h"
#include "Profiler/Profiler.h"



namespace Horyzen {

	Application* Application::s_Instance = nullptr;

	Application::Application()
	{
		HORYZEN_PROFILE_FUNCTION();

		HORYZEN_ASSERT(!s_Instance, "Application already exists!");
		s_Instance = this;

		Horyzen::WindowProperties winProps;
		winProps.Width = 1600;
		winProps.Height = 900;
		winProps.VSync = true;
		winProps.Name = "Horyzen";

		m_Window = Window::Create(winProps);

		SetUpCallbackForWindow(m_Window);

		m_ImGuiLayer = new ImGuiLayer();
		AttachOverlay(m_ImGuiLayer);
	}

	void Application::AttachLayer(Layer* p_layer)
	{
		m_LayerStack.PushLayer(p_layer);
	}
	void Application::AttachOverlay(Layer* p_overlay)
	{
		m_LayerStack.PushLayer(p_overlay);
	}

	void Application::Init()
	{
	
	}


	void Application::Run()
	{
		while (m_Running) {
			{
				HORYZEN_PROFILE_SCOPE("Application::OnUpdate");
				for (auto it = m_LayerStack.begin(); it != m_LayerStack.end(); it++) {
					(*it)->OnUpdate();
				}
			}

			{
				HORYZEN_PROFILE_SCOPE("Application::OnImGuiRender");
				m_ImGuiLayer->Begin();
				for (auto it = m_LayerStack.begin(); it != m_LayerStack.end(); it++) {
					(*it)->OnImGuiRender();
				}
				m_ImGuiLayer->End();
			}

			Horyzen::InputTracker::SetMouseDeltasToZero();
			m_Window->Update();
		}
	}

	void Application::SendEventToLayers(Event& p_event)
	{
		for (auto it = m_LayerStack.rbegin(); it != m_LayerStack.rend(); it++) {
			(*it)->OnEvent(p_event);
		}
	}

	void Application::SetUpCallbackForWindow(std::shared_ptr<Window> p_window)
	{
		p_window->SetEventCallback(std::bind(&Application::WindowEventCallback, this, std::placeholders::_1));
	}

	void Application::WindowEventCallback(Event& p_event)
	{
		Horyzen::EventDispatcher dispatcher(p_event);

		dispatcher.Dispatch<Horyzen::MouseButtonPressedEvent>(
			[&](MouseButtonPressedEvent& p_event)
			{
				InputTracker::PressMouseButton(p_event.GetMouseButtonCode());
				this->SendEventToLayers(p_event);
			}
		);

		dispatcher.Dispatch<Horyzen::MouseButtonReleasedEvent>(
			[&](MouseButtonReleasedEvent& p_event)
			{
				InputTracker::ReleaseMouseButton(p_event.GetMouseButtonCode());
				SendEventToLayers(p_event);
			}	
		);

		dispatcher.Dispatch<Horyzen::MouseMovedEvent>(
			[&](MouseMovedEvent& p_event)
			{
				InputTracker::SetMousePosition(p_event.GetX(), p_event.GetY());
				SendEventToLayers(p_event);
			}
		);

		dispatcher.Dispatch<Horyzen::MouseScrolledEvent>(
			[&](MouseScrolledEvent& p_event)
			{
				SendEventToLayers(p_event);
			}
		);

		dispatcher.Dispatch<Horyzen::KeyPressedEvent>(
			[&](KeyPressedEvent& p_event)
			{
				InputTracker::PressKey(p_event.GetKeyCode());
				SendEventToLayers(p_event);

			#ifdef PROFILE_BUILD
				if (p_event.GetKeyCode() == Key::P && InputTracker::IsLeftControlPressed() && !Profiler::GetProfiler().IsEnabled()) {
					HORYZEN_START_PROFILING();
					HORYZEN_LOG_WARN("Statring profiling. Session: {0}", Profiler::GetProfiler().GetSessionName());
				}
				else if (p_event.GetKeyCode() == Key::P && InputTracker::IsLeftControlPressed() && Profiler::GetProfiler().IsEnabled()) {
					HORYZEN_STOP_PROFILING();
					HORYZEN_LOG_WARN("Stop profiling. Session: {0}", Profiler::GetProfiler().GetSessionName());
				}
			#endif
			}
		);

		dispatcher.Dispatch<Horyzen::KeyReleasedEvent>(
			[&](KeyReleasedEvent& p_event)
			{
				InputTracker::ReleaseKey(p_event.GetKeyCode());
				SendEventToLayers(p_event);
			}
		);

		dispatcher.Dispatch<Horyzen::WindowCloseRequestedEvent>(
			[&](WindowCloseRequestedEvent& p_event)
			{
				SendEventToLayers(p_event);
				m_Running = false;
			}
		);

		dispatcher.Dispatch<Horyzen::WindowResizedEvent>(
			[&](WindowResizedEvent& p_event)
			{
				SendEventToLayers(p_event);
			}
		);

		dispatcher.Dispatch<Horyzen::WindowMovedEvent>(
			[&](WindowMovedEvent& p_event)
			{
				InputTracker::SetMousePosition(p_event.GetX(), p_event.GetY());
				SendEventToLayers(p_event);
			}
		);

		dispatcher.Dispatch<Horyzen::MouseLeftWindowEvent>(
			[&](MouseLeftWindowEvent& p_event)
			{
				InputTracker::SetMouseInWindowFlag(false);
				SendEventToLayers(p_event);
			}
		);

		dispatcher.Dispatch<Horyzen::MouseEnteredWindowEvent>(
			[&](MouseEnteredWindowEvent& p_event)
			{
				InputTracker::SetMouseInWindowFlag(true);
				SendEventToLayers(p_event);
			}
		);
	}
}