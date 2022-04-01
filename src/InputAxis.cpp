#include "InputAxis.h"
#include "IInputDevice.h"

namespace TChapman500
{
	namespace JoystickAPI
	{
		InputAxis::InputAxis(unsigned short usagePage, unsigned short usage, unsigned short bits, int minValue, int maxValue) : InputControl(usagePage, usage)
		{
			BitSize = bits;
			MinValue = minValue;
			MaxValue = maxValue;
		}

		InputAxis::~InputAxis()
		{
		}

		void InputAxis::SetState(void *state)
		{
			// Return early if no state data is given.
			if (!state) return;

			// Process state data
			valueState *vState = (valueState *)state;
			RawValue = vState->RawValue;			// Integer value
			CenterRelative = vState->CenterRelative;	// Floating point value (from -1 to +1, 0 = center)
			EndRelative = vState->EndRelative;		// Floating point value (from 0 to +1, 0.5 = center)
		}
	}
}
