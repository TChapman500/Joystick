#pragma once
#include "InputHAT.h"

namespace TChapman500
{
	namespace Input
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
