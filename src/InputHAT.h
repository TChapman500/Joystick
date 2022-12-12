#pragma once
#include "InputValue.h"

namespace TChapman500
{
	namespace Input
	{
		class InputButton;
		class InputHAT : public InputValue
		{
		public:
			enum class hat_state
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


			InputHAT(unsigned short usagePage, unsigned short usage, unsigned short bits, int min, int max, bool null, InputButton **buttonList);
			virtual ~InputHAT();

			virtual void SetState(void *state) override;

			hat_state HATPosition = hat_state::Centered;
		public:
			InputButton *UpButton;
			InputButton *DownButton;
			InputButton *RightButton;
			InputButton *LeftButton;

		};
	}
}
