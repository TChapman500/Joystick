#include "InputButton.h"

namespace TChapman500
{
	namespace JoystickAPI
	{
		InputButton::InputButton(unsigned short usagePage, unsigned short usage) : InputControl(usagePage, usage) {}
		InputButton::~InputButton() {}

		void InputButton::SetState(void *state)
		{
			bool bState = *(bool *)state;
			if (bState != IsPressed)
			{
				IsPressed = bState;
				if (bState)
				{
					RisingEdge = true;
					FallingEdge = false;
				}
				else
				{
					RisingEdge = false;
					FallingEdge = true;
				}
			}
			else
			{
				RisingEdge = false;
				FallingEdge = false;
			}
		}
	}
}
