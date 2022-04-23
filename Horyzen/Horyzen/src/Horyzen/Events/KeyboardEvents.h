#pragma once

#include "Event.h"

namespace Horyzen {

	class HORYZEN_OBJECT KeyboardEvent : public Event
	{
	public:
		KeyboardEvent(Key p_keyCode)
			: m_KeyCode(p_keyCode)
		{}

		Key GetKeyCode() { return m_KeyCode; }

	private:
		Key m_KeyCode;
	};


	class HORYZEN_OBJECT KeyPressedEvent : public KeyboardEvent
	{
	public:
		KeyPressedEvent(Key p_keyCode)
			: KeyboardEvent(p_keyCode)
		{}

		static EventType GetStaticType() { return EventType::KeyPressed; }
		virtual EventType GetEventType() override { return EventType::KeyPressed; }
		virtual i32 GetEventCategoryFlags() override
		{
			return i32(EventCategory::Keyboard) | i32(EventCategory::Input);
		}
	};


	class HORYZEN_OBJECT KeyReleasedEvent : public KeyboardEvent
	{
	public:
		KeyReleasedEvent(Key p_keyCode)
			: KeyboardEvent(p_keyCode)
		{}

		static EventType GetStaticType() { return EventType::KeyReleased; }
		virtual EventType GetEventType() override { return EventType::KeyReleased; }
		virtual i32 GetEventCategoryFlags() override
		{
			return i32(EventCategory::Keyboard) | i32(EventCategory::Input);
		}
	};

}
