#pragma once
#include "pchheader.h"

#include "Horyzen/Core.h"

#include "KeyAndMouseValues.h"

namespace Horyzen {

	class HORYZEN_OBJECT InputTracker
	{
	public:
		static bool IsKeyPressed(Key p_key);
		static bool IsLeftShiftPressed();
		static bool IsRightShiftPressed();
		static bool IsShiftPressed();
		static bool IsLeftControlPressed();
		static bool IsRightControlPressed();
		static bool IsControlPressed();
		static bool IsLeftAltPressed();
		static bool IsRightAltPressed();
		static bool IsAltPressed();
		static bool IsSpacePressed();

		static bool IsMouseButtonPressed(MouseButton p_button);
		static bool IsLeftMouseButtonPressed();
		static bool IsRightMouseButtonPressed();

		static bool IsMouseInWindow();

		static f64 GetMousePosX();
		static f64 GetMousePosY();

		static f64 GetPrevMousePosX();
		static f64 GetPrevMousePosY();

		static f64 GetMouseDX();
		static f64 GetMouseDY();

		static void SeyKeyStatus(Key p_key, bool p_status);
		static void PressKey(Key p_key);
		static void ReleaseKey(Key p_key);
		
		static void SetMouseDeltasToZero();

		static void SeyMouseButtonStatus(MouseButton p_button, bool p_status);
		static void PressMouseButton(MouseButton p_button);
		static void ReleaseMouseButton(MouseButton p_button);

		static void SetMousePosition(f64 p_xPos, f64 p_yPos);

		static void SetMouseInWindowFlag(bool p_flag);

	private:
		static std::array<bool, HORYZEN_KEY_COUNT> s_Keys;
		static std::array<bool, HORYZEN_MOUSE_BUTTON_COUNT> s_MouseButtons;

		static f64 s_MouseX;
		static f64 s_MouseY;

		static f64 s_MouseDX;
		static f64 s_MouseDY;

		static f64 s_PrevMouseX;
		static f64 s_PrevMouseY;

		static bool s_MouseInWindow;
		static bool s_MouseWasInWindow;
	};

}

