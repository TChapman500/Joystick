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

			float CenterRelative = 0.0f;
			float EndRelative = 0.0f;

			int RawValue = 0;
			
			int MinValue;
			int MaxValue;
			unsigned short BitSize;
		};
	}
}
