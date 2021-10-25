#include "XInputDevice.h"
#include <Xinput.h>

namespace TChapman500
{
	namespace JoystickAPI
	{
		XInputDevice::XInputDevice(unsigned index, HANDLE handle) : WinInputDevice(handle)
		{
			DeviceIndex = index;
		}

		XInputDevice::~XInputDevice()
		{
		}

		void XInputDevice::ReadInputStates(bool *bStates, valueState *vStates)
		{
			// Get Device State
			XINPUT_STATE devState;
			XInputGetState(DeviceIndex, &devState);

			// We're going to assume that this has been properly allocated.
			for (unsigned i = 0; i < 10; i++)
				bStates[i] = (devState.Gamepad.wButtons >> (4 + i)) & 1;

			// Left Stick
			vStates[0].RawValue = (devState.Gamepad.sThumbLX + 0x8000) & 255;
			GetFloatValues(0, 255, vStates[0].RawValue, &vStates[0].CenterRelative, &vStates[0].EndRelative);
			vStates[1].RawValue = (devState.Gamepad.sThumbLY + 0x8000) & 255;
			GetFloatValues(0, 255, vStates[1].RawValue, &vStates[1].CenterRelative, &vStates[1].EndRelative);

			// Left Trigger
			vStates[2].RawValue = devState.Gamepad.bLeftTrigger;
			GetFloatValues(0, 255, vStates[2].RawValue, &vStates[2].CenterRelative, &vStates[2].EndRelative);

			// Right Stick
			vStates[3].RawValue = (devState.Gamepad.sThumbRX + 0x8000) & 255;
			GetFloatValues(0, 255, vStates[3].RawValue, &vStates[3].CenterRelative, &vStates[3].EndRelative);
			vStates[4].RawValue = (devState.Gamepad.sThumbRY + 0x8000) & 255;
			GetFloatValues(0, 255, vStates[4].RawValue, &vStates[4].CenterRelative, &vStates[4].EndRelative);

			// Right Trigger
			vStates[5].RawValue = devState.Gamepad.bLeftTrigger;
			GetFloatValues(0, 255, vStates[5].RawValue, &vStates[5].CenterRelative, &vStates[5].EndRelative);

			// DPad UP
			if (devState.Gamepad.wButtons & 1)
			{
				// UP and LEFT
				if (devState.Gamepad.wButtons & 4) vStates[6].RawValue = 7;
				// UP and RIGHT
				else if (devState.Gamepad.wButtons & 8) vStates[6].RawValue = 1;
				// Straight UP
				else vStates[6].RawValue = 0;
			}
			// DPad DOWN
			else if (devState.Gamepad.wButtons & 2)
			{
				// DOWN and LEFT
				if (devState.Gamepad.wButtons & 4) vStates[6].RawValue = 5;
				// DOWN and RIGHT
				else if (devState.Gamepad.wButtons & 8) vStates[6].RawValue = 3;
				// Straight DOWN
				else vStates[6].RawValue = 4;
			}
			// DPad LEFT
			else if (devState.Gamepad.wButtons & 4) vStates[6].RawValue = 6;
			// DPad RIGHT
			else if (devState.Gamepad.wButtons & 8) vStates[6].RawValue = 2;
			// DPad CENTERED
			else vStates[6].RawValue = 8;
			vStates[6].CenterRelative = 0.0f;
			vStates[6].EndRelative = 0.0f;

		}

		unsigned XInputDevice::GetButtonCount() { return 10; }
		unsigned XInputDevice::GetValueCount() { return 7; }


		valueProperties ValueProperties[7] = {
			{ 1, 0x30, 8, false, 0, 255 },	// X
			{ 1, 0x31, 8, false, 0, 255 },	// Y
			{ 1, 0x32, 8, false, 0, 255 },	// Z
			{ 1, 0x33, 8, false, 0, 255 },	// Rx
			{ 1, 0x34, 8, false, 0, 255 },	// Ry
			{ 1, 0x35, 8, false, 0, 255 },	// Rz
			{ 1, 0x39, 4, true,  0, 7 }	// PoV
		};
		void XInputDevice::GetValueProperties(unsigned index, valueProperties *properties)
		{
			if (index < 7) memcpy(properties, &ValueProperties[index], sizeof(valueProperties));
		}

		void XInputDevice::GetInterfaceName(char *str) { strcpy_s(str, 32, "XInput"); }
	}
}
