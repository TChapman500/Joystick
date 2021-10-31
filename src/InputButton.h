#pragma once
#include "InputControl.h"


namespace TChapman500
{
	namespace JoystickAPI
	{
		class InputButton : public InputControl
		{
		public:
			InputButton(unsigned short usagePage, unsigned short usage);
			virtual ~InputButton();

			virtual void SetState(void *state) override;

			bool IsPressed = false;
			bool RisingEdge = false;
			bool FallingEdge = false;

		};
	}
}
