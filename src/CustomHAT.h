#pragma once
#include "InputHAT.h"

namespace TChapman500
{
	namespace JoystickAPI
	{
		class CustomHAT : public InputHAT
		{
		public:
			CustomHAT(InputButton *upButton, InputButton *downButton, InputButton *rightButton, InputButton *leftButton);
			~CustomHAT();

			virtual void SetState(void *state) override;
		};
	}
}
