#include "InputButton.h"

namespace TChapman500
{
	namespace JoystickAPI
	{
		InputButton::InputButton(unsigned short usagePage, unsigned short usage) : InputControl(usagePage, usage) {}
		InputButton::~InputButton() {}

		void InputButton::SetState(void *state)
		{
			// Clear edge flags
			RisingEdge = false;
			FallingEdge = false;

			// Return early if no state data is given.
			if (!state) return;

			// Process state data
			bool bState = *(bool *)state;
			if (bState != IsPressed)
			{
				// Set state and edge flag
				IsPressed = bState;
				if (bState) RisingEdge = true;
				else FallingEdge = true;
			}
		}
	}
}
