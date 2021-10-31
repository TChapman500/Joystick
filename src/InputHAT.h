#pragma once
#include "InputControl.h"

namespace TChapman500
{
	namespace JoystickAPI
	{
		class InputButton;
		class InputHAT : public InputControl
		{
		public:
			enum class hatState
			{
				Up,
				UpRight,
				Right,
				DownRight,
				Down,
				DownLeft,
				Left,
				UpLeft,
				Centered
			};


			InputHAT(unsigned short usagePage, unsigned short usage, InputButton **buttonList);
			virtual ~InputHAT();

			virtual void SetState(void *state) override;

			hatState HATPosition = hatState::Centered;
		public:
			InputButton *UpButton = nullptr;
			InputButton *DownButton = nullptr;
			InputButton *RightButton = nullptr;
			InputButton *LeftButton = nullptr;

		};
	}
}
