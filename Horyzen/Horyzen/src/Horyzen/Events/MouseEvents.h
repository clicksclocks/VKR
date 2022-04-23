#pragma once

#include "Event.h"
#include "Horyzen/Core.h"

namespace Horyzen {

	class HORYZEN_OBJECT MouseMovedEvent : public Event
	{
	public:
		MouseMovedEvent(f64 p_xPos, f64 p_yPos)
			: m_xPos(p_xPos), m_yPos(p_yPos)
		{}

		f64 GetX() { return m_xPos; }
		f64 GetY() { return m_yPos; }

		static EventType GetStaticType() { return EventType::MouseMoved; }
		virtual EventType GetEventType() override { return EventType::MouseMoved; }
		virtual i32 GetEventCategoryFlags() override
		{
			return i32(EventCategory::Mouse) | i32(EventCategory::Input);
		}

	private:
		f64 m_xPos;
		f64 m_yPos;
	};


	class HORYZEN_OBJECT MouseScrolledEvent : public Event
	{
	public:
		MouseScrolledEvent(f64 p_xOffset, f64 p_yOffset)
			: m_xOffset(p_xOffset), m_yOffset(p_yOffset)
		{}

		f64 GetXOffset() { return m_xOffset; }
		f64 GetYOffset() { return m_yOffset; }

		static EventType GetStaticType() { return EventType::MouseScrolled; }
		virtual EventType GetEventType() override { return EventType::MouseScrolled; }
		virtual i32 GetEventCategoryFlags() override
		{
			return i32(EventCategory::Mouse) | i32(EventCategory::Input);
		}

	private:
		f64 m_xOffset;
		f64 m_yOffset;
	};


	class HORYZEN_OBJECT MouseButtonEvent : public Event
	{
	public:
		MouseButtonEvent(MouseButton p_button)
			: m_Button(p_button)
		{}
		MouseButton GetMouseButtonCode() { return m_Button; }

	private:
		MouseButton m_Button;
	};


	class HORYZEN_OBJECT MouseButtonPressedEvent : public MouseButtonEvent
	{
	public:
		MouseButtonPressedEvent(MouseButton p_button)
			: MouseButtonEvent(p_button)
		{}

		static EventType GetStaticType() { return EventType::MouseButtonPressed; }
		virtual EventType GetEventType() override { return EventType::MouseButtonPressed; }
		virtual i32 GetEventCategoryFlags() override
		{
			return i32(EventCategory::Mouse) | i32(EventCategory::Input);
		}
	};


	class HORYZEN_OBJECT MouseButtonReleasedEvent : public MouseButtonEvent
	{
	public:
		MouseButtonReleasedEvent(MouseButton p_button)
			: MouseButtonEvent(p_button)
		{}

		static EventType GetStaticType() { return EventType::MouseButtonReleased; }
		virtual EventType GetEventType() override { return EventType::MouseButtonReleased; }
		virtual i32 GetEventCategoryFlags() override
		{
			return i32(EventCategory::Mouse) | i32(EventCategory::Input);
		}
	};

}