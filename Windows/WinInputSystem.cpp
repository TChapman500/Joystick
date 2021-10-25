#include "WinInputSystem.h"

#include "XInputDevice.h"
#include "XInputDeviceEx.h"
#include "Joystick.h"

#include <Windows.h>
#include <dinputd.h>
#include <hidsdi.h>
#include <hidpi.h>
#include <Xinput.h>

bool IsXInputDevice(HANDLE device)
{
	// Get information about the current device.
	unsigned size = sizeof(RID_DEVICE_INFO);
	RID_DEVICE_INFO devInfo;
	ZeroMemory(&devInfo, sizeof(RID_DEVICE_INFO));
	GetRawInputDeviceInfoA(device, RIDI_DEVICEINFO, &devInfo, &size);

	// Don't know how we got here!
	if (devInfo.dwType != 2) return false;
	if (devInfo.hid.usUsagePage != 1) return false;

	// If we make it past here, then the device COULD be an XInput device
	if (devInfo.hid.usUsage != 5) return false;

	// Preparsed data
	UINT dataSize = 0;
	GetRawInputDeviceInfoA(device, RIDI_PREPARSEDDATA, nullptr, &dataSize);
	_HIDP_PREPARSED_DATA *preparsed = (_HIDP_PREPARSED_DATA *)new char[dataSize];
	GetRawInputDeviceInfoA(device, RIDI_PREPARSEDDATA, preparsed, &dataSize);

	// Device Capabilities
	HIDP_CAPS caps;
	HidP_GetCaps(preparsed, &caps);

	// Verify via button and axis count.
	bool buttonCountMatch = caps.NumberInputButtonCaps == 10;
	bool valueCountMatch = caps.NumberInputValueCaps == 6;
	if (!(buttonCountMatch || valueCountMatch))
	{
		delete[] preparsed;
		return false;
	}

	// Verify via axis parameters.
	std::vector<HIDP_VALUE_CAPS> axisList;
	axisList.resize(caps.NumberInputValueCaps);
	HidP_GetValueCaps(HidP_Input, axisList.data(), &caps.NumberInputValueCaps, preparsed);

	auto verifyAxis = [](HIDP_VALUE_CAPS *axis, USAGE targetUsage) -> bool
	{
		if (axis->Range.UsageMin != targetUsage) return false;
		if (axis->BitSize != 16) return false;
		if (axis->LogicalMin != 0) return false;
		if (axis->PhysicalMin != 0) return false;
		if (axis->LogicalMax != -1) return false;
		if (axis->PhysicalMax != -1) return false;
		return true;
	};
	
	if (!verifyAxis(&axisList[0], 0x30))
	{
		delete[] preparsed;
		return false;
	}
	if (!verifyAxis(&axisList[1], 0x31))
	{
		delete[] preparsed;
		return false;
	}
	if (!verifyAxis(&axisList[2], 0x33))
	{
		delete[] preparsed;
		return false;
	}
	if (!verifyAxis(&axisList[3], 0x34))
	{
		delete[] preparsed;
		return false;
	}
	if (!verifyAxis(&axisList[4], 0x32))
	{
		delete[] preparsed;
		return false;
	}

	// Verify HAT Switch
	if (axisList[6].Range.UsageMin != 0x39) return false;

	delete[] preparsed;
	return true;
}



struct params
{
	TChapman500::JoystickAPI::WinInputSystem *Self;
	std::vector<RAWINPUTDEVICELIST> &ControllerList;
};

unsigned devIndex = 0;


namespace TChapman500
{
	namespace JoystickAPI
	{
		BOOL WinInputSystem::DIEnumDevicesCallback(LPCDIDEVICEINSTANCE lpddi, LPVOID pvRef)
		{
			// Initialize Device Instance
			params *pParams = (params *)pvRef;

			// Create device interface
			IDirectInputDevice8A *diDevice;
			pParams->Self->DirectInput->CreateDevice(lpddi->guidInstance, &diDevice, nullptr);

			// Set calibration mode to raw
			DIPROPDWORD calMode;
			ZeroMemory(&calMode, sizeof(DIPROPDWORD));
			calMode.diph.dwSize = sizeof(DIPROPDWORD);
			calMode.diph.dwHeaderSize = sizeof(DIPROPHEADER);
			calMode.diph.dwHow = DIPH_DEVICE;
			calMode.dwData = DIPROPCALIBRATIONMODE_RAW;
			diDevice->SetProperty(DIPROP_CALIBRATIONMODE, (DIPROPHEADER *)&calMode);

			// Check to see if we should create an XInputDeviceEx object
			// Get information about the current device.
			unsigned size = sizeof(RID_DEVICE_INFO);
			RID_DEVICE_INFO devInfo;
			ZeroMemory(&devInfo, sizeof(RID_DEVICE_INFO));
			GetRawInputDeviceInfoA(pParams->ControllerList[devIndex].hDevice, RIDI_DEVICEINFO, &devInfo, &size);

			// Possible XInputDevice
			if (devInfo.hid.usUsage == 5)
			{
				// Is an XInput Device
				if (IsXInputDevice(pParams->ControllerList[devIndex].hDevice))
					pParams->Self->DirectInputDeviceList.push_back(new XInputDeviceEx(diDevice, pParams->ControllerList[devIndex].hDevice));
				// Not an XInput Device
				else pParams->Self->DirectInputDeviceList.push_back(new DirectInputDevice(diDevice, pParams->ControllerList[devIndex].hDevice));
			}
			// Not an XInputDevice
			else
			{
				// Create interface and aquire the device
				pParams->Self->DirectInputDeviceList.push_back(new DirectInputDevice(diDevice, pParams->ControllerList[devIndex].hDevice));
			}


			// Create interface and aquire the device
			diDevice->Acquire();

			devIndex++;
			return DIENUM_CONTINUE;
		}


#pragma warning(push)
#pragma warning(disable:6385)
		WinInputSystem::WinInputSystem()
		{
			devIndex = 0;

			// Initialize Raw Input
			GetRawInputDeviceList(nullptr, &RAWInputCount, sizeof(RAWINPUTDEVICELIST));
			RAWINPUTDEVICELIST *devList = new RAWINPUTDEVICELIST[RAWInputCount];
			GetRawInputDeviceList(devList, &RAWInputCount, sizeof(RAWINPUTDEVICELIST));

			// Filter-out anything that is not a joystick or gamepad
			std::vector<RAWINPUTDEVICELIST> controllerList;
			for (unsigned i = 0; i < RAWInputCount; i++)
			{
				// Device is not a keyboard or mouse
				if (devList[i].dwType == RIM_TYPEHID)
				{
					// Get information about the current device.
					unsigned size = sizeof(RID_DEVICE_INFO);
					RID_DEVICE_INFO devInfo;
					ZeroMemory(&devInfo, sizeof(RID_DEVICE_INFO));
					GetRawInputDeviceInfoA(devList[i].hDevice, RIDI_DEVICEINFO, &devInfo, &size);

					// Device is a gamepad or joystick.
					if (devInfo.hid.usUsagePage == 1 && (devInfo.hid.usUsage == 4 || devInfo.hid.usUsage == 5))
						controllerList.push_back({ devList[i].hDevice, devList[i].dwType });
				}
			}
			delete[] devList;
			RAWInputCount = (unsigned)controllerList.size();


			// Temporary parameters for DirectInput
			std::vector<DirectInputDevice *> DeviceList;
			params Params = {
				this,
				controllerList
			};

			// Initialize DirectInput
			DirectInput8Create(GetModuleHandleA(nullptr), DIRECTINPUT_VERSION, IID_IDirectInput8A, (void **)&DirectInput, nullptr);
			DirectInput->EnumDevices(DI8DEVCLASS_GAMECTRL, (LPDIENUMDEVICESCALLBACKA)&DIEnumDevicesCallback, &Params, DIEDFL_ALLDEVICES);


			unsigned xInputDevCount = 0;
			// Determine which interface to use (XInput or DirectInput)
			for (unsigned i = 0; i < RAWInputCount; i++)
			{
				if (IsXInputDevice(controllerList[i].hDevice))
				{
					if (xInputDevCount < 4)
					{
						XInputDevice *newInterface = new XInputDevice(xInputDevCount, controllerList[i].hDevice);
						XInputDeviceList.push_back(newInterface);
						JoystickList.push_back(new Joystick(newInterface));
					}
					else JoystickList.push_back(new Joystick(DirectInputDeviceList[i]));
					xInputDevCount++;
				}
				else JoystickList.push_back(new Joystick(DirectInputDeviceList[i]));
			}
		}
#pragma warning(pop)

		WinInputSystem::~WinInputSystem()
		{
			for (unsigned i = 0; i < JoystickList.size(); i++)
				delete JoystickList[i];

			for (unsigned i = 0; i < XInputDeviceList.size(); i++)
				delete XInputDeviceList[i];

			for (unsigned i = 0; i < DirectInputDeviceList.size(); i++)
				delete DirectInputDeviceList[i];

			DirectInput->Release();
		}

		void WinInputSystem::UpdateJoystickStates()
		{
			for (unsigned i = 0; i < JoystickList.size(); i++)
				JoystickList[i]->ReadJoystickState();
		}

		unsigned WinInputSystem::GetJoystickCount() { return (unsigned)JoystickList.size(); }

		Joystick *WinInputSystem::GetJoystick(unsigned index)
		{
			if (index < JoystickList.size()) return JoystickList[index];
			return nullptr;
		}
	}
}
