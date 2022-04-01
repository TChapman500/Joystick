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

			/// <summary>
			/// Whether the button is currently pressed.
			/// </summary>
			bool IsPressed = false;

			/// <summary>
			/// Whether the button was pressed this frame.
			/// </summary>
			bool RisingEdge = false;

			/// <summary>
			/// Whether the button was released this frame.
			/// </summary>
			bool FallingEdge = false;
		};
	}
}
