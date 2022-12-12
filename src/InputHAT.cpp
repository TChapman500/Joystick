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
				HATPosition = hat_state::Centered;
			// HAT Switch is Not Centered
			else HATPosition = (hat_state)(Value - MinValue);
			
			bool buttonStates[4] = { false, false, false, false };

			// Check which fake buttons should be set as pressed.
			if (HATPosition == hat_state::Up || HATPosition == hat_state::UpLeft || HATPosition == hat_state::UpRight) buttonStates[0] = true;
			if (HATPosition == hat_state::Down || HATPosition == hat_state::DownLeft || HATPosition == hat_state::DownRight) buttonStates[1] = true;
			if (HATPosition == hat_state::Right || HATPosition == hat_state::UpRight || HATPosition == hat_state::DownRight) buttonStates[2] = true;
			if (HATPosition == hat_state::Left || HATPosition == hat_state::UpLeft || HATPosition == hat_state::DownLeft) buttonStates[3] = true;

			// Set fake button states.
			if (UpButton) UpButton->SetState(&buttonStates[0]);
			if (DownButton) DownButton->SetState(&buttonStates[1]);
			if (RightButton) RightButton->SetState(&buttonStates[2]);
			if (LeftButton) LeftButton->SetState(&buttonStates[3]);
		}
	}
}