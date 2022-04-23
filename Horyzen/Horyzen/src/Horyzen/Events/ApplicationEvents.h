#pragma once

#include "Event.h"

namespace Horyzen {

	class HORYZEN_OBJECT WindowCloseRequestedEvent : public Event
	{
	public:

		static EventType GetStaticType() { return EventType::WindowCloseRequested; }
		virtual EventType GetEventType() override { return EventType::WindowCloseRequested; }
		virtual i32 GetEventCategoryFlags() override
		{
			return i32(EventCategory::Application);
		}

	};


	class HORYZEN_OBJECT WindowResizedEvent : public Event
	{
	public:

		WindowResizedEvent(i32 p_width, i32 p_height)
			: m_Width(p_width), m_Height(p_height)
		{}

		static EventType GetStaticType() { return EventType::WindowResized; }
		virtual EventType GetEventType() override { return EventType::WindowResized; }
		virtual i32 GetEventCategoryFlags() override
		{
			return i32(EventCategory::Application);
		}

		i32 GetWidth() { return m_Width; }
		i32 GetHeight() { return m_Height; }

	private:

		i32 m_Width;
		i32 m_Height;

	};


	class HORYZEN_OBJECT WindowMovedEvent : public Event
	{
	public:

		WindowMovedEvent(i32 p_xPos, i32 p_yPos)
			: m_xPos(p_xPos), m_yPos(p_yPos)
		{}

		static EventType GetStaticType() { return EventType::WindowMoved; }
		virtual EventType GetEventType() override { return EventType::WindowMoved; }
		virtual i32 GetEventCategoryFlags() override
		{
			return i32(EventCategory::Application);
		}
		i32 GetX() { return m_xPos; }
		i32 GetY() { return m_yPos; }

	private:

		i32 m_xPos;
		i32 m_yPos;

	};


	class HORYZEN_OBJECT MouseLeftWindowEvent : public Event
	{
	public:

		static EventType GetStaticType() { return EventType::MouseLeftWindow; }
		virtual EventType GetEventType() override { return EventType::MouseLeftWindow; }
		virtual i32 GetEventCategoryFlags() override
		{
			return i32(EventCategory::Application);
		}

	};


	class HORYZEN_OBJECT MouseEnteredWindowEvent : public Event
	{
	public:

		static EventType GetStaticType() { return EventType::MouseEnteredWindow; }
		virtual EventType GetEventType() override { return EventType::MouseEnteredWindow; }
		virtual i32 GetEventCategoryFlags() override
		{
			return i32(EventCategory::Application);
		}

	};

}
