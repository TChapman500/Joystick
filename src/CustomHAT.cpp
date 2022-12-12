#include "CustomHAT.h"
#include "InputButton.h"

namespace TChapman500
{
	namespace Input
	{
		CustomHAT::CustomHAT(InputButton *upButton, InputButton *downButton, InputButton *rightButton, InputButton *leftButton) : InputHAT(0x01, 0x39, 4, 0, 7, true, nullptr)
		{
			UpButton = upButton;
			DownButton = downButton;
			RightButton = rightButton;
			LeftButton = leftButton;
		}

		CustomHAT::~CustomHAT() {}

		void CustomHAT::SetState(void *state)
		{
			if (UpButton->IsPressed)
			{
				if (RightButton->IsPressed) HATPosition = hat_state::UpRight;
				else if (LeftButton->IsPressed) HATPosition = hat_state::UpLeft;
				else HATPosition = hat_state::Up;
			}
			else if (DownButton->IsPressed)
			{
				if (RightButton->IsPressed) HATPosition = hat_state::DownRight;
				else if (LeftButton->IsPressed) HATPosition = hat_state::DownLeft;
				else HATPosition = hat_state::Down;
			}
			else if (RightButton->IsPressed) HATPosition = hat_state::Right;
			else if (LeftButton->IsPressed) HATPosition = hat_state::Left;
			else HATPosition = hat_state::Centered;

			// Just in case.
			Value = (int)HATPosition;
		}
	}
}