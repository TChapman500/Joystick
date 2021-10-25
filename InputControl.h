#pragma once

namespace TChapman500
{
	namespace JoystickAPI
	{
		class InputControl
		{
		public:
			InputControl(unsigned short usagePage, unsigned short usage);
			virtual ~InputControl();

			virtual void SetState(void *state) = 0;
			
		private:
			unsigned short UsagePage;
			unsigned short Usage;
		};
	}
}
