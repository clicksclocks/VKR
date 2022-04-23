#include "pchheader.h"
#include "Layer.h"


namespace Horyzen {

	void Layer::OnEvent(Event& p_event)
	{
		Horyzen::EventDispatcher dispatcher(p_event);
		dispatcher.Dispatch<MouseButtonPressedEvent>(std::bind(&Layer::OnMouseButtonPressedEvent, this, std::placeholders::_1));
		dispatcher.Dispatch<MouseButtonReleasedEvent>(std::bind(&Layer::OnMouseButtonReleasedEvent, this, std::placeholders::_1));
		dispatcher.Dispatch<MouseMovedEvent>(std::bind(&Layer::OnMouseMovedEvent, this, std::placeholders::_1));
		dispatcher.Dispatch<MouseScrolledEvent>(std::bind(&Layer::OnMouseScrolledEvent, this, std::placeholders::_1));

		dispatcher.Dispatch<KeyPressedEvent>(std::bind(&Layer::OnKeyPressedEvent, this, std::placeholders::_1));
		dispatcher.Dispatch<KeyReleasedEvent>(std::bind(&Layer::OnKeyReleasedEvent, this, std::placeholders::_1));

		dispatcher.Dispatch<WindowCloseRequestedEvent>(std::bind(&Layer::OnWindowCloseRequestedEvent, this, std::placeholders::_1));
		dispatcher.Dispatch<WindowResizedEvent>(std::bind(&Layer::OnWindowResizedEvent, this, std::placeholders::_1));
		dispatcher.Dispatch<WindowMovedEvent>(std::bind(&Layer::OnWindowMovedEvent, this, std::placeholders::_1));
		dispatcher.Dispatch<MouseLeftWindowEvent>(std::bind(&Layer::OnMouseLeftWindowEvent, this, std::placeholders::_1));
		dispatcher.Dispatch<MouseEnteredWindowEvent>(std::bind(&Layer::OnMouseEnteredWindowEvent, this, std::placeholders::_1));
	}

}
