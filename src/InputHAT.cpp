#include "InputHAT.h"
#include "InputButton.h"

namespace TChapman500
{
	namespace JoystickAPI
	{
		InputHAT::InputHAT(unsigned short usagePage, unsigned short usage, InputButton **buttonList) : InputControl(usagePage, usage)
		{
			if (buttonList)
			{
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
			HATPosition = *(hatState *)state;
			
			bool ButtonStates[4] = { false, false, false, false };

			// Check which fake buttons should be set as pressed.
			if (HATPosition == hatState::Up || HATPosition == hatState::UpLeft || HATPosition == hatState::UpRight) ButtonStates[0] = true;
			if (HATPosition == hatState::Down || HATPosition == hatState::DownLeft || HATPosition == hatState::DownRight) ButtonStates[1] = true;
			if (HATPosition == hatState::Right || HATPosition == hatState::UpRight || HATPosition == hatState::DownRight) ButtonStates[2] = true;
			if (HATPosition == hatState::Left || HATPosition == hatState::UpLeft || HATPosition == hatState::DownLeft) ButtonStates[3] = true;

			// Set fake button states.
			UpButton->SetState(&ButtonStates[0]);
			DownButton->SetState(&ButtonStates[1]);
			RightButton->SetState(&ButtonStates[2]);
			LeftButton->SetState(&ButtonStates[3]);
		}
	}
}