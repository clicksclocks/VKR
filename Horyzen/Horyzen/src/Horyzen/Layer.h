#pragma once

#include "Horyzen/Events/MouseEvents.h"
#include "Horyzen/Events/KeyboardEvents.h"
#include "Horyzen/Events/ApplicationEvents.h"

#include "Logger.h"

namespace Horyzen {

	class HORYZEN_OBJECT Layer
	{
	public:

		virtual ~Layer() = default;

		virtual void OnAttach() {}
		virtual void OnDetach() {}
		virtual void OnUpdate() {}

		virtual void OnImGuiRender() {}

		void OnEvent(Event& p_event);

	public:

		virtual void OnMouseButtonPressedEvent(MouseButtonPressedEvent& p_event) {}
		virtual void OnMouseButtonReleasedEvent(MouseButtonReleasedEvent& p_event) {}
		virtual void OnMouseMovedEvent(MouseMovedEvent& p_event) {}
		virtual void OnMouseScrolledEvent(MouseScrolledEvent& p_event) {}

		virtual void OnKeyPressedEvent(KeyPressedEvent& p_event) {}
		virtual void OnKeyReleasedEvent(KeyReleasedEvent& p_event) {}

		virtual void OnWindowCloseRequestedEvent(WindowCloseRequestedEvent& p_event) {}
		virtual void OnWindowResizedEvent(WindowResizedEvent& p_event) {}
		virtual void OnWindowMovedEvent(WindowMovedEvent& p_event) {}
		virtual void OnMouseLeftWindowEvent(MouseLeftWindowEvent& p_event) {}
		virtual void OnMouseEnteredWindowEvent(MouseEnteredWindowEvent& p_event) {}

	};

}

