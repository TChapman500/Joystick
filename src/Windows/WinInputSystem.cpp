#include "WinInputSystem.h"

#include "Joystick.h"

#include <Windows.h>
#include <dinputd.h>
#include <hidsdi.h>
#include <hidpi.h>
#include <setupapi.h>
#include <Xinput.h>
#include <wctype.h>
#include <vector>

#include <fstream>
#include <string>

#include "HID.h"

namespace TChapman500 {
namespace Input {
namespace Windows {

struct params
{
	TChapman500::Input::Windows::WinInputSystem *Self;
	std::vector<RAWINPUTDEVICELIST> &ControllerList;
};

WinInputSystem::WinInputSystem()
{
	std::vector<std::wstring> xInputHids;

	// Initialize HID
	GUID hidGuid;
	HidD_GetHidGuid(&hidGuid);
	HDEVINFO devInfo = SetupDiGetClassDevsW(&hidGuid, nullptr, nullptr, DIGCF_DEVICEINTERFACE | DIGCF_PRESENT);
	if (devInfo == INVALID_HANDLE_VALUE) return;

	std::vector<std::wstring> hidJoysticks;
	std::vector<HID *> hidInterfaces;

	std::vector<IInput *> tempInputInterfaces;
	std::vector<IOutput *> tempOutputInterfaces;
	std::vector<Joystick *> tempJoystickList;

	// Get all hid devices and filter-out all non-joysticks/gamepads
	SP_DEVICE_INTERFACE_DATA devInterface;
	devInterface.cbSize = sizeof(SP_DEVICE_INTERFACE_DATA);
	for (int i = 0; ; i++)
	{
		// Device Fetch Failed
		if (!SetupDiEnumDeviceInterfaces(devInfo, nullptr, &hidGuid, i, &devInterface))
		{
			// No more Devices
			DWORD error = GetLastError();
			if (error == ERROR_NO_MORE_ITEMS) break;
			continue;
		}

		// Get the Interface Data
		unsigned long requiredSize = 0;
		SetupDiGetDeviceInterfaceDetailW(devInfo, &devInterface, nullptr, 0, &requiredSize, nullptr);
		char *buffer = new char[requiredSize];

		// Get Interface Detail Data
		SP_DEVICE_INTERFACE_DETAIL_DATA *interfaceDetail;
		interfaceDetail = (SP_DEVICE_INTERFACE_DETAIL_DATA *)buffer;
		interfaceDetail->cbSize = sizeof(SP_DEVICE_INTERFACE_DETAIL_DATA);
		SetupDiGetDeviceInterfaceDetailW(devInfo, &devInterface, interfaceDetail, requiredSize, nullptr, nullptr);

		// Open the device file
		wchar_t *devicePath = interfaceDetail->DevicePath;
		HANDLE fileHandle = CreateFileW(devicePath, GENERIC_READ | GENERIC_WRITE, FILE_SHARE_READ | FILE_SHARE_WRITE, nullptr, OPEN_EXISTING, FILE_FLAG_OVERLAPPED, nullptr);
		if (!fileHandle || fileHandle == INVALID_HANDLE_VALUE)
		{
			DWORD error = GetLastError();
			delete[] buffer;
			continue;
		}

		// Get HID information
		_HIDP_PREPARSED_DATA *preparsed;
		_HIDP_CAPS caps;
		if (!HidD_GetPreparsedData(fileHandle, &preparsed))
		{
			DWORD error = GetLastError();
			delete[] buffer;
			CloseHandle(fileHandle);
			continue;
		}

		if (!HidP_GetCaps(preparsed, &caps))
		{
			delete[] buffer;
			CloseHandle(fileHandle);
			HidD_FreePreparsedData(preparsed);
			continue;
		}

		// Joystick or gamepad.
		if (caps.UsagePage == 1 && (caps.Usage == 4 || caps.Usage == 5))
		{
			// Filter-out XInput devices.  We'll get them through other means.
			std::wstring devString(devicePath);
			IInput *newInterface = nullptr;
			if (devString.find(L"ig_") != std::wstring::npos)
			{
				// Add string to list.
				xInputHids.push_back(std::wstring(devicePath));

				// TODO:  Test 5+ XInput devices to see if 5+ virtual HIDs are created by the XInput driver.
			}
			else
			{
				// Add device to HID interface list.
				HID *newDevice = new HID(devicePath);
				hidInterfaces.push_back(newDevice);

				// Create the new joystick.
				newInterface = newDevice;
				tempJoystickList.push_back(new Joystick(newDevice, nullptr));

				// Add string to list.
				hidJoysticks.push_back(std::wstring(devicePath));
			}

			// Just in case.
			if (newInterface) tempInputInterfaces.push_back(newInterface);
		}
		delete[] buffer;

		// Free-Up Memory
		CloseHandle(fileHandle);
		HidD_FreePreparsedData(preparsed);
	}
	SetupDiDestroyDeviceInfoList(devInfo);

	// Initialize DirectInput

	// Initialize Custom XInput



	// Copy interface list to master
	_JoystickListSize = tempJoystickList.size();
	_JoystickList = new Joystick*[_JoystickListSize];
	for (size_t i = 0; i < _JoystickListSize; i++) _JoystickList[i] = tempJoystickList[i];

	_InputInterfaceSize = tempInputInterfaces.size();
	_InputInterfaces = new IInput*[_InputInterfaceSize];
	for (size_t i = 0; i < _InputInterfaceSize; i++) _InputInterfaces[i] = tempInputInterfaces[i];

	_OutputInterfaceSize = 0;
	_OutputInterfaces = nullptr;
}

WinInputSystem::~WinInputSystem()
{
	for (size_t i = 0; i < _JoystickListSize; i++) delete _JoystickList[i];
	delete[] _JoystickList;

	for (size_t i = 0; i < _InputInterfaceSize; i++) delete _InputInterfaces[i];
	delete[] _InputInterfaces;

	if (_OutputInterfaceSize > 0)
	{
		for (size_t i = 0; i < _OutputInterfaceSize; i++) delete _OutputInterfaces[i];
		delete[] _OutputInterfaces;
	}

	//DirectInput->Release();
}

void WinInputSystem::UpdateJoystickStates()
{
	for (size_t i = 0; i < _JoystickListSize; i++)
		_JoystickList[i]->ReadJoystickState();
}

unsigned WinInputSystem::GetJoystickCount() { return (unsigned)_JoystickListSize; }

Joystick *WinInputSystem::GetJoystick(unsigned index)
{
	if (index >= _JoystickListSize) return nullptr;
	return _JoystickList[index];
}

}}}

