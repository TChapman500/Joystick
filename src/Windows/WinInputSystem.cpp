#include "WinInputSystem.h"

// TODO:  Replace this with platform-specific implementations
#include "IOutput.h"


#include "Joystick.h"
#include "Mouse.h"
#include "Keyboard.h"
#include "InputButton.h"
#include "WinKeyboard.h"
#include "WinMouse.h"

#include <Windows.h>
#include <windowsx.h>
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
	// Create keyboard and mouse interface.
	_IKeyboard = new WinKeyboard();
	_IMouse = new WinMouse();

	// Create the keyboard and mouse.
	_Mouse = new Mouse(_IMouse);
	_Keyboard = new Keyboard(_IKeyboard);

	// Register keyboard and mouse for raw input.
	RAWINPUTDEVICE devices[2] = {
		{ 1, 2, NULL, nullptr },
		{ 1, 6, NULL, nullptr } };
	RegisterRawInputDevices(devices, 2, sizeof(RAWINPUTDEVICE));

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

	delete _IKeyboard;
	delete _IMouse;
	delete _Keyboard;
	delete _Mouse;


	//DirectInput->Release();
}

void WinInputSystem::UpdateJoystickStates()
{
	_Keyboard->ReadKeyboardState();
	for (size_t i = 0; i < _JoystickListSize; i++)
		_JoystickList[i]->ReadJoystickState();
}

unsigned WinInputSystem::GetJoystickCount() { return (unsigned)_JoystickListSize; }

Joystick *WinInputSystem::GetJoystick(unsigned index)
{
	if (index >= _JoystickListSize) return nullptr;
	return _JoystickList[index];
}

unsigned WinInputSystem::GetAxisCount(unsigned joystick)
{
	if (joystick >= _JoystickListSize) return 0;
	return _JoystickList[joystick]->GetAxisCount();
}

int WinInputSystem::GetAxisValue(unsigned joystick, unsigned axis)
{
	if (joystick >= _JoystickListSize) return 0;
	return _JoystickList[joystick]->GetAxisValue(axis);
}

float WinInputSystem::GetAxisCenterRelative(unsigned joystick, unsigned axis)
{
	if (joystick >= _JoystickListSize) return 0.0f;
	return _JoystickList[joystick]->GetAxisCenterRelative(axis);
}

float WinInputSystem::GetAxisEndRelative(unsigned joystick, unsigned axis)
{
	if (joystick >= _JoystickListSize) return 0.0f;
	return _JoystickList[joystick]->GetAxisEndRelative(axis);
}

bool WinInputSystem::GetAxisProperties(value_properties &properties, unsigned joystick, unsigned axis)
{
	if (joystick >= _JoystickListSize) return false;
	return _JoystickList[joystick]->GetAxisProperties(properties, axis);
}

unsigned WinInputSystem::GetButtonCount(unsigned joystick)
{
	if (joystick >= _JoystickListSize) return 0;
	return _JoystickList[joystick]->GetButtonCount();
}

bool WinInputSystem::GetButtonState(unsigned joystick, unsigned button)
{
	if (joystick >= _JoystickListSize) return false;
	return _JoystickList[joystick]->GetButtonState(button);
}

bool WinInputSystem::GetButtonRisingEdge(unsigned joystick, unsigned button)
{
	if (joystick >= _JoystickListSize) return false;
	return _JoystickList[joystick]->GetButtonRisingEdge(button);
}

bool WinInputSystem::GetButtonFallingEdge(unsigned joystick, unsigned button)
{
	if (joystick >= _JoystickListSize) return false;
	return _JoystickList[joystick]->GetButtonFallingEdge(button);
}

unsigned short WinInputSystem::GetButtonUsage(unsigned joystick, unsigned button)
{
	if (joystick >= _JoystickListSize) return 0;
	return _JoystickList[joystick]->GetButtonUsage(button);
}

unsigned WinInputSystem::GetHATCount(unsigned joystick)
{
	if (joystick >= _JoystickListSize) return 0;
	return _JoystickList[joystick]->GetHATCount();
}

hat_position WinInputSystem::GetHATPosition(unsigned joystick, unsigned hat)
{
	if (joystick >= _JoystickListSize) return hat_position::Centered;
	return _JoystickList[joystick]->GetHATPosition(hat);
}

Mouse *WinInputSystem::GetMouse() { return _Mouse; }

bool WinInputSystem::GetMousePosition(int &x, int &y)
{
	_Mouse->GetMousePosition(x, y);
	return true;
}

bool WinInputSystem::GetMouseDelta(int &x, int &y)
{
	_Mouse->GetMouseDelta(x, y);
	return true;
}

bool WinInputSystem::GetMouseScroll(int &x, int &y)
{
	_Mouse->GetMouseScroll(x, y);
	return true;
}

bool WinInputSystem::GetMouseButton(unsigned button) { return _Mouse->GetButton(button); }
bool WinInputSystem::GetMouseRisingEdge(unsigned button) { return _Mouse->GetButtonRisingEdge(button); }
bool WinInputSystem::GetMouseFallingEdge(unsigned button) { return _Mouse->GetButtonFallingEdge(button); }

Keyboard *WinInputSystem::GetKeyboard() { return _Keyboard; }
bool WinInputSystem::GetKeyState(key_code key) { return _Keyboard->GetKeyState(key); }
bool WinInputSystem::GetKeyRisingEdge(key_code key) { return _Keyboard->GetKeyRisingEdge(key); }
bool WinInputSystem::GetKeyFallingEdge(key_code key) { return _Keyboard->GetKeyFallingEdge(key); }
bool WinInputSystem::GetCapsLock() { return _Keyboard->GetCapsLock(); }
bool WinInputSystem::GetNumLock() { return _Keyboard->GetNumLock(); }
bool WinInputSystem::GetScrollLock() { return _Keyboard->GetScrollLock(); }


bool WinInputSystem::MessageProc(UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	case WM_INPUT_DEVICE_CHANGE:	// For device connect/disconnect.

		return true;
	case WM_INPUT:		// For raw keyboard and mouse input.
	{
		// Get the Input Data
		HRAWINPUT lRawInput = (HRAWINPUT)lParam;
		UINT rawInputSize = 0;
		GetRawInputData(lRawInput, RID_HEADER, nullptr, &rawInputSize, sizeof(RAWINPUTHEADER));
		RAWINPUT *rawInput = (RAWINPUT *)(new(std::nothrow) char[rawInputSize]);
		if (!rawInput) return true;
		GetRawInputData(lRawInput, RID_HEADER, rawInput, &rawInputSize, sizeof(RAWINPUTHEADER));

		// Process keyboard or mouse input.
		switch (rawInput->header.dwType)
		{
		case RIM_TYPEMOUSE:
			_IMouse->RawInputMessage(&rawInput->data.mouse);
			break;
		case RIM_TYPEKEYBOARD:
			_IKeyboard->RawInputMessage(&rawInput->data.keyboard);
			break;
		}

		// Clean-up
		char *rawInputBuffer = (char *)rawInput;
		delete[] rawInputBuffer;
		return true;
	}
	case WM_MOUSEMOVE:	// For absolute mouse position.
		_IMouse->MouseMoveMessage(lParam);
		return true;
	}
	return false;
}


}}}

