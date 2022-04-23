#include "pchheader.h"
#include "InputTracker.h"

#include "Horyzen/Logger.h"



namespace Horyzen {

	std::array<bool, HORYZEN_KEY_COUNT> InputTracker::s_Keys;
	std::array<bool, HORYZEN_MOUSE_BUTTON_COUNT> InputTracker::s_MouseButtons;

	f64 InputTracker::s_MouseX = -1.0;
	f64 InputTracker::s_MouseY = -1.0;

	f64 InputTracker::s_MouseDX = -1.0;
	f64 InputTracker::s_MouseDY = -1.0;

	f64 InputTracker::s_PrevMouseX = -1.0;
	f64 InputTracker::s_PrevMouseY = -1.0;

	bool InputTracker::s_MouseInWindow = false;
	bool InputTracker::s_MouseWasInWindow = false;

	
	bool InputTracker::IsKeyPressed(Key p_key) { return s_Keys[u64(p_key)]; }
	bool InputTracker::IsLeftShiftPressed() { return s_Keys[u64(Key::LeftShift)]; }
	bool InputTracker::IsRightShiftPressed() { return s_Keys[u64(Key::RightShift)]; }
	bool InputTracker::IsShiftPressed() { return s_Keys[u64(Key::LeftShift)] || s_Keys[u64(Key::RightShift)]; }
	bool InputTracker::IsLeftControlPressed() { return s_Keys[u64(Key::LeftControl)]; }
	bool InputTracker::IsRightControlPressed() { return s_Keys[u64(Key::RightControl)]; }
	bool InputTracker::IsControlPressed() { return s_Keys[u64(Key::LeftControl)] || s_Keys[u64(Key::RightControl)]; }
	bool InputTracker::IsLeftAltPressed() { return s_Keys[u64(Key::LeftAlt)]; }
	bool InputTracker::IsRightAltPressed() { return s_Keys[u64(Key::RightAlt)]; }
	bool InputTracker::IsAltPressed() { return s_Keys[u64(Key::LeftAlt)] || s_Keys[u64(Key::RightAlt)]; }
	bool InputTracker::IsSpacePressed() { return s_Keys[u64(Key::Space)]; }

	bool InputTracker::IsMouseButtonPressed(MouseButton p_button) { return s_MouseButtons[u64(p_button)]; }
	bool InputTracker::IsLeftMouseButtonPressed() { return s_MouseButtons[u64(MouseButton::B1)]; }
	bool InputTracker::IsRightMouseButtonPressed() { return s_MouseButtons[u64(MouseButton::B2)]; }

	bool InputTracker::IsMouseInWindow() { return s_MouseInWindow; }


	f64 InputTracker::GetMousePosX() { return s_MouseX; }
	f64 InputTracker::GetMousePosY() { return s_MouseY; }
	f64 InputTracker::GetPrevMousePosX() { return s_PrevMouseX; }
	f64 InputTracker::GetPrevMousePosY() { return s_PrevMouseY; }
	f64 InputTracker::GetMouseDX() { return s_MouseDX; }
	f64 InputTracker::GetMouseDY() { return s_MouseDY; }


	void InputTracker::SeyKeyStatus(Key p_key, bool p_status) { s_Keys[u64(p_key)] = p_status; }
	void InputTracker::PressKey(Key p_key) { SeyKeyStatus(p_key, true); }
	void InputTracker::ReleaseKey(Key p_key) { SeyKeyStatus(p_key, false); }

	void InputTracker::SetMouseDeltasToZero()
	{
		s_MouseDX = 0.0;
		s_MouseDY = 0.0;
	}

	void InputTracker::SeyMouseButtonStatus(MouseButton p_button, bool p_status) { s_MouseButtons[u64(p_button)] = p_status; }
	void InputTracker::PressMouseButton(MouseButton p_button) { SeyMouseButtonStatus(p_button, true); }
	void InputTracker::ReleaseMouseButton(MouseButton p_button) { SeyMouseButtonStatus(p_button, false); }

	void InputTracker::SetMousePosition(f64 p_xPos, f64 p_yPos)
	{
		s_PrevMouseX = s_MouseX;
		s_PrevMouseY = s_MouseY;

		s_MouseX = p_xPos;
		s_MouseY = p_yPos;

		if (s_MouseWasInWindow) {
			s_MouseDX = s_MouseX - s_PrevMouseX;
			s_MouseDY = s_MouseY - s_PrevMouseY;
		} else {
			s_MouseDX = 0.0;
			s_MouseDY = 0.0;
			s_MouseWasInWindow = true;
		}
	}

	void InputTracker::SetMouseInWindowFlag(bool p_flag) 
	{
		s_MouseWasInWindow = s_MouseInWindow;
		s_MouseInWindow = p_flag; 
	}

}