#include "XInputDeviceEx.h"

namespace TChapman500
{
	namespace JoystickAPI
	{
		XInputDeviceEx::XInputDeviceEx(IDirectInputDevice8A *device, HANDLE devHandle) : DirectInputDevice(device, devHandle)
		{
		}

		XInputDeviceEx::~XInputDeviceEx()
		{
		}

		void XInputDeviceEx::ReadInputStates(bool *bStates, valueState *vStates)
		{
			// Call the base function, then modify the axis results from there.
			DirectInputDevice::ReadInputStates(bStates, vStates);

			vStates[0].RawValue &= 255;
			GetFloatValues(0, 255, vStates[0].RawValue, &vStates[0].CenterRelative, &vStates[0].EndRelative);
			vStates[1].RawValue &= 255;
			GetFloatValues(0, 255, vStates[1].RawValue, &vStates[1].CenterRelative, &vStates[1].EndRelative);
			vStates[2].RawValue &= 255;
			GetFloatValues(0, 255, vStates[2].RawValue, &vStates[2].CenterRelative, &vStates[2].EndRelative);
			vStates[3].RawValue &= 255;
			GetFloatValues(0, 255, vStates[3].RawValue, &vStates[3].CenterRelative, &vStates[3].EndRelative);
			vStates[4].RawValue >>= 8;
			GetFloatValues(0, 255, vStates[4].RawValue, &vStates[4].CenterRelative, &vStates[4].EndRelative);
		}

		unsigned XInputDeviceEx::GetButtonCount() { return 10; }
		unsigned XInputDeviceEx::GetValueCount() { return 6; }

		valueProperties ValuePropertiesEx[6] = {
			{ 1, 0x31, 8, false, 0, 255 },	// X
			{ 1, 0x30, 8, false, 0, 255 },	// Y
			{ 1, 0x34, 8, false, 0, 255 },	// Rx
			{ 1, 0x33, 8, false, 0, 255 },	// Ry
			{ 1, 0x32, 8, false, 0, 255 },	// Z
			{ 1, 0x39, 4, true,  0, 7 }	// PoV
		};
		void XInputDeviceEx::GetValueProperties(unsigned index, valueProperties *properties)
		{
			if (index < 6) memcpy(properties, &ValuePropertiesEx[index], sizeof(valueProperties));
		}
		void XInputDeviceEx::GetInterfaceName(char *str) { strcpy_s(str, 32, "XInput using DirectInput"); }
	}
}