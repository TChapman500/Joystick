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
			valueState *vState = (valueState *)state;
			RawValue = vState->RawValue;
			CenterRelative = vState->CenterRelative;
			EndRelative = vState->EndRelative;
		}
	}
}
