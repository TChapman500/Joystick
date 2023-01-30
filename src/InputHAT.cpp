#include "InputHAT.h"
#include "InputButton.h"

namespace TChapman500
{
	namespace Input
	{
		InputHAT::InputHAT(unsigned short usagePage, unsigned short usage, unsigned short bits, int min, int max, bool null, InputButton **buttonList) : InputValue(usagePage, usage, bits, min, max, null)
		{
			// The parent device takes ownership of these pointers.
			UpButton = nullptr;
			DownButton = nullptr;
			RightButton = nullptr;
			LeftButton = nullptr;

			if (buttonList)
			{
				// The parent device takes ownership of these pointers.
				UpButton = new InputButton(9, 60001);
				DownButton = new InputButton(9, 60002);
				RightButton = new InputButton(9, 60003);
				LeftButton = new InputButton(9, 60004);

				buttonList[0] = UpButton;
				buttonList[1] = DownButton;
				buttonList[2] = RightButton;
				buttonList[3] = LeftButton;
			}
		}

		InputHAT::~InputHAT()
		{
		}

		void InputHAT::SetState(void *state)
		{
			InputValue::SetState(state);

			// HAT Switch is Centered
			if (Value < MinValue || Value > MaxValue)
				HATPosition = hat_position::Centered;
			// HAT Switch is Not Centered
			else HATPosition = (hat_position)(Value - MinValue);
			
			bool buttonStates[4] = { false, false, false, false };

			// Check which fake buttons should be set as pressed.
			if (HATPosition == hat_position::Up || HATPosition == hat_position::UpLeft || HATPosition == hat_position::UpRight) buttonStates[0] = true;
			if (HATPosition == hat_position::Down || HATPosition == hat_position::DownLeft || HATPosition == hat_position::DownRight) buttonStates[1] = true;
			if (HATPosition == hat_position::Right || HATPosition == hat_position::UpRight || HATPosition == hat_position::DownRight) buttonStates[2] = true;
			if (HATPosition == hat_position::Left || HATPosition == hat_position::UpLeft || HATPosition == hat_position::DownLeft) buttonStates[3] = true;

			// Set fake button states.
			if (UpButton) UpButton->SetState(&buttonStates[0]);
			if (DownButton) DownButton->SetState(&buttonStates[1]);
			if (RightButton) RightButton->SetState(&buttonStates[2]);
			if (LeftButton) LeftButton->SetState(&buttonStates[3]);
		}
	}
}