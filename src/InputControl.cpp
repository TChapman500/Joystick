#include "InputControl.h"

namespace TChapman500
{
	namespace Input
	{
		InputControl::InputControl(unsigned short usagePage, unsigned short usage)
		{
			UsagePage = usagePage;
			Usage = usage;
		}

		InputControl::~InputControl()
		{
		}
	}
}