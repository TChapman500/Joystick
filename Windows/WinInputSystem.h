#pragma once
#include "IInputSystem.h"
#include <vector>
#include <dinput.h>

namespace TChapman500
{
	namespace JoystickAPI
	{
		class Joystick;
		class XInputDevice;
		class DirectInputDevice;

		class WinInputSystem : public IInputSystem
		{
		public:
			WinInputSystem();
			~WinInputSystem();


			virtual void UpdateJoystickStates() override;
			virtual unsigned GetJoystickCount() override;
			virtual Joystick *GetJoystick(unsigned index) override;
		private:
			unsigned XInputCount = 0;
			unsigned RAWInputCount = 0;
			unsigned DirectInputCount = 0;

			std::vector<XInputDevice *> XInputDeviceList;			// Max of 4 entries
			std::vector<DirectInputDevice *> DirectInputDeviceList;		// Variable number of entries
			std::vector<Joystick *> JoystickList;		// All devices found

			IDirectInput8A *DirectInput;
			static BOOL DIEnumDevicesCallback(LPCDIDEVICEINSTANCE lpddi, LPVOID pvRef);

		};
	}
}

