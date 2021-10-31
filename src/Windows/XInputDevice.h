#pragma once
#include "WinInputDevice.h"

namespace TChapman500
{
	namespace JoystickAPI
	{
		class XInputDevice : public WinInputDevice
		{
		public:
			XInputDevice(unsigned index, HANDLE handle);
			~XInputDevice();
			
			virtual void ReadInputStates(bool *bStates, valueState *vStates) override;

			virtual unsigned GetButtonCount() override;
			virtual unsigned GetValueCount() override;
			virtual void GetValueProperties(unsigned index, valueProperties *properties) override;

			virtual void GetInterfaceName(char *str) override;
		private:
			unsigned DeviceIndex;
		};
	}
}
