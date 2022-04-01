#pragma once
#include "InputControl.h"

namespace TChapman500
{
	namespace JoystickAPI
	{
		class InputAxis : public InputControl
		{
		public:
			InputAxis(unsigned short usagePage, unsigned short usage, unsigned short bits, int minValue, int maxValue);
			~InputAxis();

			void SetState(void *state) override;

			/// <summary>
			/// A floating point value with a range from -1.0 to +1.0.  This is intended to
			/// be used for controls where the center gives no activation of the control
			/// while each end gives full activation of the control but in the opposite
			/// direction from the other end.  Eg:  Steering, rotation controls, etc.
			/// </summary>
			float CenterRelative = 0.0f;

			/// <summary>
			/// A floating point value with a range from 0.0 to 1.0. This is intended to be
			/// used for controls where one end gives no activation of the control while the
			/// other end gives full activation of the control.  Eg:  Throttle, brakes, etc.
			/// </summary>
			float EndRelative = 0.0f;

			/// <summary>
			/// The raw value retrieved from the device.  The direct use of this value is
			/// not recommended.  Use the CenterRelative and EndRelative values instead.
			/// </summary>
			int RawValue = 0;
			
			int MinValue;
			int MaxValue;
			unsigned short BitSize;
		};
	}
}
