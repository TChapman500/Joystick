#include "InputAxis.h"
#include "IInputDevice.h"

namespace TChapman500
{
	namespace Input
	{
		InputAxis::InputAxis(unsigned short usagePage, unsigned short usage, unsigned short bits, int min, int max, bool null) : InputValue(usagePage, usage, bits, min, max, null)
		{
			CenterRelative = 0.0f;
			EndRelative = 0.0f;
		}

		InputAxis::~InputAxis()
		{
		}

		void InputAxis::SetState(void *state)
		{
			InputValue::SetState(state);

			// Make sure everything is relative to zero.
			int absMax = MaxValue - MinValue;
			int absRaw = Value - MinValue;

			// End-Relative is Easy
			EndRelative = (float)absRaw / (float)absMax;

			// Center-relative.
			int halfValue = absMax >> 1;

			// Odd number of states (1 center position)
			if (absMax % 2 == 0)
			{
				CenterRelative = (float)(absRaw - halfValue) / (float)halfValue;
			}
			// Even number of states (2 center positions, most likely path)
			else
			{
				int overHalf = halfValue + 1;
				if (absRaw >= overHalf) CenterRelative = (float)(absRaw - overHalf) / (float)halfValue;
				else CenterRelative = ((float)absRaw / (float)halfValue) - 1.0f;
			}

		}
	}
}
