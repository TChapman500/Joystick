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
				if (RightButton->IsPressed) HATPosition = hat_position::UpRight;
				else if (LeftButton->IsPressed) HATPosition = hat_position::UpLeft;
				else HATPosition = hat_position::Up;
			}
			else if (DownButton->IsPressed)
			{
				if (RightButton->IsPressed) HATPosition = hat_position::DownRight;
				else if (LeftButton->IsPressed) HATPosition = hat_position::DownLeft;
				else HATPosition = hat_position::Down;
			}
			else if (RightButton->IsPressed) HATPosition = hat_position::Right;
			else if (LeftButton->IsPressed) HATPosition = hat_position::Left;
			else HATPosition = hat_position::Centered;

			// Just in case.
			Value = (int)HATPosition;
		}
	}
}