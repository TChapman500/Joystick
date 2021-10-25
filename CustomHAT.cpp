#include "CustomHAT.h"
#include "InputButton.h"

namespace TChapman500
{
	namespace JoystickAPI
	{
		CustomHAT::CustomHAT(InputButton *upButton, InputButton *downButton, InputButton *rightButton, InputButton *leftButton) : InputHAT(1, 0x39, nullptr)
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
				if (RightButton->IsPressed) HATPosition = hatState::UpRight;
				else if (LeftButton->IsPressed) HATPosition = hatState::UpLeft;
				else HATPosition = hatState::Up;
			}
			else if (DownButton->IsPressed)
			{
				if (RightButton->IsPressed) HATPosition = hatState::DownRight;
				else if (LeftButton->IsPressed) HATPosition = hatState::DownLeft;
				else HATPosition = hatState::Down;
			}
			else if (RightButton->IsPressed) HATPosition = hatState::Right;
			else if (LeftButton->IsPressed) HATPosition = hatState::Left;
			else HATPosition = hatState::Centered;
		}
	}
}