#pragma once

#include "pchheader.h"
#include "Horyzen/Core.h"
#include "Horyzen/Input/KeyAndMouseValues.h"



namespace Horyzen {

	enum class EventType
	{
		None = 0,
		MouseButtonPressed = 1,
		MouseButtonReleased = 2,
		MouseMoved = 3,
		MouseScrolled = 4,

		KeyPressed = 5,
		KeyReleased = 6,

		WindowCloseRequested = 7,
		WindowResized = 8,
		WindowMoved = 9,
		MouseLeftWindow = 10,
		MouseEnteredWindow = 11
	};

	enum class EventCategory : i32
	{
		None = 0,
		Input = 1,
		Mouse = (1 << 1),
		Keyboard = (1 << 2),
		Application = (1 << 3)
	};


	class HORYZEN_OBJECT Event
	{
	public:
		virtual EventType GetEventType() = 0;
		virtual i32 GetEventCategoryFlags() = 0;

	};


	class HORYZEN_OBJECT EventDispatcher
	{
	public:
		EventDispatcher(Event& p_event)
			: m_Event(p_event)
		{}

		template <typename EventT, typename FuncT>
		void Dispatch(const FuncT& p_func)
		{
			if (EventT::GetStaticType() == m_Event.GetEventType()) {
				p_func(dynamic_cast<EventT&>(m_Event));
			}
		}

	private:
		Event& m_Event;
	};

}