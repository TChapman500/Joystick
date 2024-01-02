#include "CustomHAT.h"
#include "InputButton.h"

namespace TChapman500
{
	namespace Input
	{
		CustomHAT::CustomHAT(InputButton *upButton, InputButton *downButton, InputButton *rightButton, InputButton *leftButton) : InputHAT(0x01, 0x39, 4, 0, 7, true, nullptr, nullptr)
		{
			UpButton = upButton;
			DownButton = downButton;
			RightButton = rightButton;
			LeftButton = leftButton;
		}

		CustomHAT::~CustomHAT() {}

		void CustomHAT::SetState(void *state)
		{
			hat_position currState = Position;

			if (UpButton->IsPressed)
			{
				if (RightButton->IsPressed) Position = hat_position::UpRight;
				else if (LeftButton->IsPressed) Position = hat_position::UpLeft;
				else Position = hat_position::Up;
			}
			else if (DownButton->IsPressed)
			{
				if (RightButton->IsPressed) Position = hat_position::DownRight;
				else if (LeftButton->IsPressed) Position = hat_position::DownLeft;
				else Position = hat_position::Down;
			}
			else if (RightButton->IsPressed) Position = hat_position::Right;
			else if (LeftButton->IsPressed) Position = hat_position::Left;
			else Position = hat_position::Centered;

			if (Position != currState)
			{
				ChangeFrom = currState;
				ChangeTo = Position;
			}
			else
			{
				ChangeFrom = hat_position::None;
				ChangeTo = hat_position::None;
			}

			// Just in case.
			Value = (int)Position;
		}
	}
}