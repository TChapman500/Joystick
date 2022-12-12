#pragma once
#include "InputValue.h"

namespace TChapman500
{
	namespace Input
	{
		class InputAxis : public InputValue
		{
		public:
			InputAxis(unsigned short usagePage, unsigned short usage, unsigned short bits, int min, int max, bool null);
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

		};
	}
}
