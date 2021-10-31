#include "InputControl.h"

namespace TChapman500
{
	namespace JoystickAPI
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