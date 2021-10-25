#pragma once
#include "DirectInputDevice.h"


namespace TChapman500
{
	namespace JoystickAPI
	{
		class XInputDeviceEx : public DirectInputDevice
		{
		public:
			XInputDeviceEx(IDirectInputDevice8A *device, HANDLE devHandle);
			~XInputDeviceEx();

			virtual void ReadInputStates(bool *bStates, valueState *vStates) override;

			virtual unsigned GetButtonCount() override;
			virtual unsigned GetValueCount() override;
			virtual void GetValueProperties(unsigned index, valueProperties *properties) override;

			virtual void GetInterfaceName(char *str) override;
		};
	}
}

