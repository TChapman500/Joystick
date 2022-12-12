#include "IInputDevice.h"

namespace TChapman500
{
	namespace Input
	{
		void IInputDevice::GetFloatValues(int min, int max, int raw, float *center, float *end)
		{
			// Make sure everything is relative to zero.
			int absMax = max - min;
			int absRaw = raw - min;

			// End-relative is easy.
			*end = (float)absRaw / (float)absMax;

			// Center-relative.
			int halfValue = absMax / 2;

			// Odd number of states (1 center position)
			if (absMax % 2 == 0)
			{
				*center = (float)(absRaw - halfValue) / (float)halfValue;
			}
			// Even number of states (2 center positions, most likely path)
			else
			{
				int overHalf = halfValue + 1;
				if (absRaw >= overHalf) *center = (float)(absRaw - overHalf) / (float)halfValue;
				else *center = ((float)absRaw / (float)halfValue) - 1.0f;
			}
		}
	}
}
