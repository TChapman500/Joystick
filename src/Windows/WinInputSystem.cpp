#include "WinInputSystem.h"

// TODO:  Replace this with platform-specific implementations
#include "IOutput.h"

#include "NullKeyboard.h"
#include "NullMouse.h"
#include "Joystick.h"
#include "Mouse.h"
#include "Keyboard.h"
#include "InputButton.h"
#include "WinKeyboard.h"
#include "WinMouse.h"
#include "WinDevice.h"
#include "RawInputDevice.h"
#include "XInput.h"

#include <Windows.h>
#include <windowsx.h>
#include <dinputd.h>
#include <hidsdi.h>
#include <hidpi.h>
#include <setupapi.h>
#include <Xinput.h>
#include <wctype.h>
#include <vector>
#include <initguid.h>
#include <cfgmgr32.h>
#include <Devpkey.h>
#include <dbt.h>

#include <fstream>
#include <string>

namespace TChapman500 {
namespace Input {
namespace Windows {

struct params
{
	TChapman500::Input::Windows::WinInputSystem *Self;
	std::vector<RAWINPUTDEVICELIST> &ControllerList;
};

static const GUID xbox_guid = { 0xec87f1e3, 0xc13b, 0x4100, { 0xb5, 0xf7, 0x8b, 0x84, 0xd5, 0x42, 0x60, 0xcb } };



void WinInputSystem::_PopulateXInputDevices()
{
	HDEVINFO devInfo = SetupDiGetClassDevsW(&xbox_guid, nullptr, nullptr, DIGCF_DEVICEINTERFACE | DIGCF_PRESENT);
	if (devInfo == INVALID_HANDLE_VALUE) return;

	// Iterate through all of the devices.
	for (int i = 0; ; i++)
	{
		// Attempt to get the next device.
		SP_DEVICE_INTERFACE_DATA devInterface;
		devInterface.cbSize = sizeof(SP_DEVICE_INTERFACE_DATA);
		if (!SetupDiEnumDeviceInterfaces(devInfo, nullptr, &xbox_guid, i, &devInterface))
		{
			// No more Devices
			DWORD error = GetLastError();
			if (error == ERROR_NO_MORE_ITEMS) break;
			continue;
		}

		// See if we can get the interface detail.
		DWORD requiredSize = 0;
		SetupDiGetDeviceInterfaceDetailW(devInfo, &devInterface, nullptr, 0, &requiredSize, nullptr);
		PSP_DEVICE_INTERFACE_DETAIL_DATA_W devDetail = (PSP_DEVICE_INTERFACE_DETAIL_DATA_W)LocalAlloc(LMEM_FIXED, requiredSize);
		if (devDetail == nullptr) continue;
		devDetail->cbSize = sizeof(SP_DEVICE_INTERFACE_DETAIL_DATA_W);

		// Get the interface detail.
		SP_DEVINFO_DATA devInfoData;
		devInfoData.cbSize = sizeof(SP_DEVINFO_DATA);
		SetupDiGetDeviceInterfaceDetailW(devInfo, &devInterface, devDetail, requiredSize, &requiredSize, &devInfoData);

		// Get the child device.
		DEVPROPTYPE propType;
		DWORD propBufferSize;
		SetupDiGetDevicePropertyW(devInfo, &devInfoData, &DEVPKEY_Device_Children, &propType, nullptr, 0, &propBufferSize, NULL);
		LPWSTR propBuffer = new(std::nothrow) WCHAR[propBufferSize >> 1];
		if (!propBuffer)
		{
			LocalFree(devDetail);
			continue;
		}
		SetupDiGetDevicePropertyW(devInfo, &devInfoData, &DEVPKEY_Device_Children, &propType, (PBYTE)propBuffer, propBufferSize, nullptr, NULL);
		std::wstring childPath(propBuffer);
		delete[] propBuffer;
		for (size_t j = 0; j < childPath.length(); j++) childPath[j] = towlower(childPath[j]);

		std::wstring devPath(devDetail->DevicePath);
		for (size_t j = 0; j < devPath.length(); j++) devPath[j] = towlower(devPath[j]);

		// Add the device to the list.
		WinDevice *device = new(std::nothrow) WinDevice(nullptr, (wchar_t*)devPath.c_str(), nullptr, (wchar_t *)childPath.c_str());
		if (!device) continue;
		_WinXInputDevices.push_back(device);
		_UnsynchedXInputDevices.push_back(device);

		XInput *xDevice = new(std::nothrow) XInput(device, nullptr);
		if (!xDevice)
		{
			LocalFree(devDetail);
			continue;
		}
		_XInputInterfaces.push_back(xDevice);
		_UnsynchedXInputInterfaces.push_back(xDevice);

		// Clean-up this iteration.
		LocalFree(devDetail);
	}
	SetupDiDestroyDeviceInfoList(devInfo);
}

void WinInputSystem::_PopulateRawInputDevices()
{
	_InputProc = &WinInputSystem::_JoyMessageProc;

	// Create keyboard and mouse interface.
	_IKeyboard = new(std::nothrow) NullKeyboard();
	_IMouse = new(std::nothrow) NullMouse();

	// Create the keyboard and mouse.
	_Mouse = new(std::nothrow) Mouse(_IMouse);
	_Keyboard = new(std::nothrow) Keyboard(_IKeyboard);

	// Create the hidden window to handle joystick input.
	WNDCLASSEXW hWndClass;
	memset(&hWndClass, 0, sizeof(WNDCLASSEXW));
	hWndClass.cbSize = sizeof(WNDCLASSEXW);
	hWndClass.lpfnWndProc = &WinInputSystem::_WindowProc;
	hWndClass.lpszClassName = L"Hidden Window";
	hWndClass.hInstance = GetModuleHandleW(nullptr);
	RegisterClassExW(&hWndClass);

	// Hidden window to handle input.
	_HWnd = CreateWindowExW(NULL, L"Hidden Window", L"Hidden Window", WS_POPUP, 0, 0, 50, 50, nullptr, nullptr, hWndClass.hInstance, nullptr);
	SetWindowLongPtrW(_HWnd, GWLP_USERDATA, (LONG_PTR)this);

	// Register Game Controllers for Raw Input Messages
	RAWINPUTDEVICE devices[2] = {
		{ 1, 4, RIDEV_DEVNOTIFY, _HWnd },
		{ 1, 5, RIDEV_DEVNOTIFY, _HWnd } };
	RegisterRawInputDevices(devices, 2, sizeof(RAWINPUTDEVICE));

	DEV_BROADCAST_DEVICEINTERFACE_W db =
	{
	    sizeof(DEV_BROADCAST_DEVICEINTERFACE_W),
	    DBT_DEVTYP_DEVICEINTERFACE,
	    NULL,
	    xbox_guid,
	};
	_HDevNotify = RegisterDeviceNotificationW(_HWnd, &db, DEVICE_NOTIFY_WINDOW_HANDLE);
}

void WinInputSystem::_PopulateRawInputDevices(HWND hWnd)
{
	if (hWnd)
	{
		_InputProc = &WinInputSystem::_KeyMouseMessageProc;

		// Create keyboard and mouse interface.
		_IKeyboard = new(std::nothrow) WinKeyboard();
		_IMouse = new(std::nothrow) WinMouse();

		// Create the keyboard and mouse.
		_Mouse = new(std::nothrow) Mouse(_IMouse);
		_Keyboard = new(std::nothrow) Keyboard(_IKeyboard);

		// Register Keyboard, Mouse and Game Controllers for Raw Input Messages
		RAWINPUTDEVICE devices[4] = {
			{ 1, 2, NULL, nullptr },
			{ 1, 4, RIDEV_DEVNOTIFY, hWnd },
			{ 1, 5, RIDEV_DEVNOTIFY, hWnd },
			{ 1, 6, NULL, nullptr } };
		RegisterRawInputDevices(devices, 4, sizeof(RAWINPUTDEVICE));

		DEV_BROADCAST_DEVICEINTERFACE_W db =
		{
		    sizeof(DEV_BROADCAST_DEVICEINTERFACE_W),
		    DBT_DEVTYP_DEVICEINTERFACE,
		    NULL,
		    xbox_guid,
		};
		_HDevNotify = RegisterDeviceNotificationW(hWnd, &db, DEVICE_NOTIFY_WINDOW_HANDLE);
	}
	else _PopulateRawInputDevices();
}

void WinInputSystem::_SyncDevices()
{
	LPWSTR xChild = nullptr;
	LPWSTR riParent = nullptr;

	// Get all XInput Devices
	while (!_UnsynchedXInputDevices.empty())
	{
		xChild = _WinXInputDevices[0]->GetChildPath();
		for (size_t i = 0; i < _UnsynchedRawInputDevices.size();)
		{
			riParent = _UnsynchedRawInputDevices[i]->GetParentPath();
			if (wcscmp(xChild, riParent) == 0)
			{
				XInput *xInterface = _UnsynchedXInputInterfaces[0];
				RawInputDevice *riInterface = _UnsynchedRawInputInterfaces[i];
				xInterface->SetRawInputDevice(riInterface);
				_UnsynchedXInputDevices.erase(_UnsynchedXInputDevices.begin());
				_UnsynchedXInputInterfaces.erase(_UnsynchedXInputInterfaces.begin());
				_UnsynchedRawInputDevices.erase(_UnsynchedRawInputDevices.begin() + i);
				_UnsynchedRawInputInterfaces.erase(_UnsynchedRawInputInterfaces.begin() + i);
				Joystick *newJoystick = new(std::nothrow) Joystick(xInterface, nullptr);
				if (!newJoystick) break;
				_Joysticks.push_back(newJoystick);
			}
			i++;
		}
	}

	// Get all non-XInput Devices
	while (!_UnsynchedRawInputDevices.empty())
	{
		RawInputDevice *riInterface = _UnsynchedRawInputInterfaces[0];
		_UnsynchedRawInputDevices.erase(_UnsynchedRawInputDevices.begin());
		_UnsynchedRawInputInterfaces.erase(_UnsynchedRawInputInterfaces.begin());
		Joystick *newJoystick = new(std::nothrow) Joystick(riInterface, nullptr);
		if (!newJoystick) continue;
		_Joysticks.push_back(newJoystick);
	}
}

WinInputSystem::WinInputSystem()
{
	_PopulateXInputDevices();
	_PopulateRawInputDevices();
}

WinInputSystem::WinInputSystem(HWND mainHWnd)
{
	_PopulateXInputDevices();
	_PopulateRawInputDevices(mainHWnd);
}

WinInputSystem::~WinInputSystem()
{
	RAWINPUTDEVICE devices[4] = {
		{ 1, 2, RIDEV_REMOVE, nullptr },
		{ 1, 4, RIDEV_REMOVE, nullptr },
		{ 1, 5, RIDEV_REMOVE, nullptr },
		{ 1, 6, RIDEV_REMOVE, nullptr } };
	RegisterRawInputDevices(devices, 4, sizeof(RAWINPUTDEVICE));

	UnregisterDeviceNotification(_HDevNotify);

	for (size_t i = 0; i < _WinXInputDevices.size(); i++) delete _WinXInputDevices[i];
	for (size_t i = 0; i < _WinRawInputDevices.size(); i++) delete _WinRawInputDevices[i];
	for (size_t i = 0; i < _Joysticks.size(); i++) delete _Joysticks[i];
	for (size_t i = 0; i < _InputInterfaces.size(); i++) delete _InputInterfaces[i];
	for (size_t i = 0; i < _OutputInterfaces.size(); i++) delete _OutputInterfaces[i];

	if (_HWnd) DestroyWindow(_HWnd);

	if (_IKeyboard) delete _IKeyboard;
	if (_IMouse) delete _IMouse;
	if (_Keyboard) delete _Keyboard;
	if (_Mouse) delete _Mouse;


	//DirectInput->Release();
}

void WinInputSystem::ResetDeviceStates()
{
	_Keyboard->ReadKeyboardState();
	_Mouse->ReadMouseState();
}

void WinInputSystem::UpdateJoystickStates()
{
	_Keyboard->ReadKeyboardState();
	_Mouse->ReadMouseState();
	for (size_t i = 0; i < _Joysticks.size(); i++)
		_Joysticks[i]->ReadJoystickState();
}

unsigned WinInputSystem::GetJoystickCount() { return (unsigned)_Joysticks.size(); }

Joystick *WinInputSystem::GetJoystick(unsigned index)
{
	if (index >= _Joysticks.size()) return nullptr;
	return _Joysticks[index];
}

unsigned WinInputSystem::GetAxisCount(unsigned joystick)
{
	if (joystick >= _Joysticks.size()) return 0;
	return _Joysticks[joystick]->GetAxisCount();
}

int WinInputSystem::GetAxisValue(unsigned joystick, unsigned axis)
{
	if (joystick >= _Joysticks.size()) return 0;
	return _Joysticks[joystick]->GetAxisValue(axis);
}

float WinInputSystem::GetAxisCenterRelative(unsigned joystick, unsigned axis)
{
	if (joystick >= _Joysticks.size()) return 0.0f;
	return _Joysticks[joystick]->GetAxisCenterRelative(axis);
}

float WinInputSystem::GetAxisEndRelative(unsigned joystick, unsigned axis)
{
	if (joystick >= _Joysticks.size()) return 0.0f;
	return _Joysticks[joystick]->GetAxisEndRelative(axis);
}

bool WinInputSystem::GetAxisProperties(value_properties &properties, unsigned joystick, unsigned axis)
{
	if (joystick >= _Joysticks.size()) return false;
	return _Joysticks[joystick]->GetAxisProperties(properties, axis);
}

unsigned WinInputSystem::GetButtonCount(unsigned joystick)
{
	if (joystick >= _Joysticks.size()) return 0;
	return _Joysticks[joystick]->GetButtonCount();
}

bool WinInputSystem::GetButtonState(unsigned joystick, unsigned button)
{
	if (joystick >= _Joysticks.size()) return false;
	return _Joysticks[joystick]->GetButtonState(button);
}

bool WinInputSystem::GetButtonRisingEdge(unsigned joystick, unsigned button)
{
	if (joystick >= _Joysticks.size()) return false;
	return _Joysticks[joystick]->GetButtonRisingEdge(button);
}

bool WinInputSystem::GetButtonFallingEdge(unsigned joystick, unsigned button)
{
	if (joystick >= _Joysticks.size()) return false;
	return _Joysticks[joystick]->GetButtonFallingEdge(button);
}

unsigned short WinInputSystem::GetButtonUsage(unsigned joystick, unsigned button)
{
	if (joystick >= _Joysticks.size()) return 0;
	return _Joysticks[joystick]->GetButtonUsage(button);
}

unsigned WinInputSystem::GetHATCount(unsigned joystick)
{
	if (joystick >= _Joysticks.size()) return 0;
	return _Joysticks[joystick]->GetHATCount();
}

hat_position WinInputSystem::GetHATPosition(unsigned joystick, unsigned hat)
{
	if (joystick >= _Joysticks.size()) return hat_position::Centered;
	return _Joysticks[joystick]->GetHATPosition(hat);
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


bool WinInputSystem::WindowProc(UINT message, WPARAM wParam, LPARAM lParam) { return (this->*_InputProc)(message, wParam, lParam); }

LRESULT WinInputSystem::_WindowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	WinInputSystem *pThis = (WinInputSystem*)GetWindowLongPtrW(hWnd, GWLP_USERDATA);
	if (!pThis) return DefWindowProcW(hWnd, message, wParam, lParam);
	if ((pThis->*pThis->_InputProc)(message, wParam, lParam)) return 0;
	return DefWindowProcW(hWnd, message, wParam, lParam);
}

bool WinInputSystem::_JoyMessageProc(UINT message, WPARAM wParam, LPARAM lParam)
{
	if (_DevConnectProc(message, wParam, lParam)) return true;
	if (message != WM_INPUT) return false;

		// Get the Input Data
	HRAWINPUT hRawInput = (HRAWINPUT)lParam;
	UINT riSize = 0;
	GetRawInputData(hRawInput, RID_INPUT, nullptr, &riSize, sizeof(RAWINPUTHEADER));
	RAWINPUT *rawInput = (RAWINPUT *)new(std::nothrow) char[riSize];
	if (!rawInput) return true;
	GetRawInputData(hRawInput, RID_INPUT, rawInput, &riSize, sizeof(RAWINPUTHEADER));

	// Process input data.
	_ProcessRiJoysticks(rawInput);

	// Clean-up
	delete[](char *)rawInput;
	return true;
}

bool WinInputSystem::_KeyMouseMessageProc(UINT message, WPARAM wParam, LPARAM lParam)
{
	if (_DevConnectProc(message, wParam, lParam)) return true;

	switch (message)
	{
	case WM_INPUT:		// For raw keyboard and mouse input.
	{
		// Get the Input Data
		HRAWINPUT hRawInput = (HRAWINPUT)lParam;
		UINT riSize = 0;
		GetRawInputData(hRawInput, RID_INPUT, nullptr, &riSize, sizeof(RAWINPUTHEADER));
		RAWINPUT *rawInput = (RAWINPUT*)new(std::nothrow) char[riSize];
		if (!rawInput) return true;
		GetRawInputData(hRawInput, RID_INPUT, rawInput, &riSize, sizeof(RAWINPUTHEADER));

		// Process keyboard or mouse input.
		switch (rawInput->header.dwType)
		{
		case RIM_TYPEMOUSE:
			((WinMouse*)_IMouse)->RawInputMessage(&rawInput->data.mouse);
			break;
		case RIM_TYPEKEYBOARD:
			((WinKeyboard*)_IKeyboard)->RawInputMessage(&rawInput->data.keyboard);
			break;
		case RIM_TYPEHID:
			_ProcessRiJoysticks(rawInput);
			break;
		}

		// Clean-up
		delete[] (char*)rawInput;
		return true;
	}
	case WM_MOUSEMOVE:	// For absolute mouse position.
		((WinMouse *)_IMouse)->MouseMoveMessage(lParam);
		return true;
	}
	return false;
}

bool WinInputSystem::_DevConnectProc(UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	case WM_DEVICECHANGE:		// For XInput device connect/disconnect.
	{
		DEV_BROADCAST_HDR *hdr = (DEV_BROADCAST_HDR *)lParam;
		if (!hdr || hdr->dbch_devicetype != DBT_DEVTYP_DEVICEINTERFACE) return false;
		DEV_BROADCAST_DEVICEINTERFACE_W *dif = (DEV_BROADCAST_DEVICEINTERFACE_W *)hdr;

		std::wstring path(dif->dbcc_name);
		for (size_t i = 0; i < path.length(); i++) path[i] = towlower(path[i]);

		switch (wParam)
		{
		case DBT_DEVICEARRIVAL:
		{
			// New Device?
			std::wstring instPath = path;
			size_t lastSlash = instPath.find_last_of(L'\\');
			instPath = &instPath[lastSlash + 1];
			size_t lastPound = instPath.find_last_of(L'#');
			instPath.resize(lastPound);
			for (size_t i = 0; i < instPath.length(); i++)
			{
				if (instPath[i] == L'#')
					instPath[i] = L'\\';
			}

			// Get Parent Path
			DEVPROPTYPE propType;
			wchar_t *childPath = nullptr;
			ULONG parentLength = 0;
			DEVINST currDevice;
			CM_Locate_DevNodeW(&currDevice, (DEVINSTID_W)instPath.c_str(), CM_LOCATE_DEVNODE_NORMAL);
			CM_Get_DevNode_PropertyW(currDevice, &DEVPKEY_Device_Children, &propType, nullptr, &parentLength, NULL);
			childPath = new(std::nothrow) wchar_t[parentLength >> 1];
			if (!childPath) return true;
			CM_Get_DevNode_PropertyW(currDevice, &DEVPKEY_Device_Children, &propType, (BYTE *)childPath, &parentLength, NULL);
			std::wstring childInst(childPath);
			delete[] childPath;
			for (size_t i = 0; i < childInst.length(); i++) childInst[i] = towlower(childInst[i]);

			// Scan for existing devices.
			for (size_t i = 0; i < _WinXInputDevices.size(); i++)
			{
				if (_WinXInputDevices[i]->GetPath() == path)
				{
					_WinXInputDevices[i]->SetDeviceConnected(true);
					return true;
				}
			}

			// Create the device.
			WinDevice *newDevice = new(std::nothrow) WinDevice(nullptr, (wchar_t *)path.c_str(), nullptr, (wchar_t *)childInst.c_str());
			if (!newDevice) return true;
			_WinXInputDevices.push_back(newDevice);
			_UnsynchedXInputDevices.push_back(newDevice);

			// Create the interface.
			XInput *newXInputDevice = new(std::nothrow) XInput(newDevice, nullptr);
			if (!newXInputDevice) return true;
			_XInputInterfaces.push_back(newXInputDevice);
			_UnsynchedXInputInterfaces.push_back(newXInputDevice);

			return true;
		}
		case DBT_DEVICEREMOVECOMPLETE:
			// Scan for existing devices.
			for (size_t i = 0; i < _WinXInputDevices.size(); i++)
			{
				if (_WinXInputDevices[i]->GetPath() == path)
				{
					_WinXInputDevices[i]->SetDeviceConnected(false);
					return true;
				}
			}
			break;
		}
		return true;
	}
	case WM_INPUT_DEVICE_CHANGE:	// For RawInput device connect/disconnect.
		switch (wParam)
		{
		case GIDC_ARRIVAL:
		{
			HANDLE riDevice = (HANDLE)lParam;

			wchar_t *pathBuffer = nullptr;
			UINT pathLength = 0;

			// Get Device Path
			GetRawInputDeviceInfoW(riDevice, RIDI_DEVICENAME, nullptr, &pathLength);
			pathBuffer = new(std::nothrow) wchar_t[pathLength];
			if (!pathBuffer) return true;
			memset(pathBuffer, 0, sizeof(wchar_t) * pathLength);
			GetRawInputDeviceInfoW(riDevice, RIDI_DEVICENAME, pathBuffer, &pathLength);

			// Get Instance Path
			std::wstring path(pathBuffer);
			delete[] pathBuffer;
			for (size_t i = 0; i < path.length(); i++) path[i] = towlower(path[i]);

			std::wstring instPath = path;
			size_t lastSlash = instPath.find_last_of(L'\\');
			instPath = &instPath[lastSlash + 1];
			size_t lastPound = instPath.find_last_of(L'#');
			instPath.resize(lastPound);
			for (size_t i = 0; i < instPath.length(); i++)
			{
				if (instPath[i] == L'#')
					instPath[i] = L'\\';
			}

			// Get Parent Path
			DEVPROPTYPE propType;
			wchar_t *parentPath = nullptr;
			ULONG parentLength = 0;
			DEVINST currDevice;
			CM_Locate_DevNodeW(&currDevice, (DEVINSTID_W)instPath.c_str(), CM_LOCATE_DEVNODE_NORMAL);
			CM_Get_DevNode_PropertyW(currDevice, &DEVPKEY_Device_Parent, &propType, nullptr, &parentLength, NULL);
			parentPath = new(std::nothrow) wchar_t[parentLength >> 1];
			if (!parentPath) return true;
			CM_Get_DevNode_PropertyW(currDevice, &DEVPKEY_Device_Parent, &propType, (BYTE*)parentPath, &parentLength, NULL);
			std::wstring parentInst(parentPath);
			delete[] parentPath;
			for (size_t i = 0; i < parentInst.length(); i++) parentInst[i] = towlower(parentInst[i]);

			//*
			size_t isXInputDevice = path.find(L"ig_");
			if (isXInputDevice == std::wstring::npos)
			{
				// Scan for existing device using only the path.
				for (size_t i = 0; i < _WinRawInputDevices.size(); i++)
				{
					if (_WinRawInputDevices[i]->GetFileHandle() != nullptr) continue;

					// Device plugged back into the same port.
					if (_WinRawInputDevices[i]->GetPath() == path)
					{
						_WinRawInputDevices[i]->SetRawInputHandle(riDevice);
						_WinRawInputDevices[i]->SetDeviceConnected(true);
						return true;
					}
				}

				// Scan for existing device using device IDs.
				for (size_t i = 0; i < _WinRawInputDevices.size(); i++)
				{
					if (_WinRawInputDevices[i]->GetFileHandle() != nullptr) continue;

					unsigned vendorID = _RawInputInterfaces[i]->GetVendorID();
					unsigned productID = _RawInputInterfaces[i]->GetProductID();
					unsigned versionNumber = _RawInputInterfaces[i]->GetVersionNumber();
					wchar_t *serialNumber = (wchar_t *)_RawInputInterfaces[i]->GetSerialNumber();

					HANDLE devHandle = CreateFileW(path.c_str(), GENERIC_READ | GENERIC_WRITE, FILE_SHARE_READ | FILE_SHARE_WRITE, nullptr, OPEN_EXISTING, FILE_FLAG_OVERLAPPED, nullptr);
					if (!devHandle) continue;
					_HIDD_ATTRIBUTES devInfo;
					if (!HidD_GetAttributes(devHandle, &devInfo)) continue;

					// Found a potential match.
					if (vendorID == devInfo.VendorID && productID == devInfo.ProductID && versionNumber == devInfo.VersionNumber)
					{
						// No serial number string, assume the match.
						if (serialNumber == nullptr)
						{
							CloseHandle(devHandle);

							_WinRawInputDevices[i]->SetRawInputHandle(riDevice);
							_WinRawInputDevices[i]->SetDeviceConnected(true);
							_WinRawInputDevices[i]->SetParentPath((wchar_t *)parentInst.c_str());
							_WinRawInputDevices[i]->SetPath((wchar_t *)path.c_str());
							return true;
						}

						std::wstring serial(serialNumber);

						wchar_t *strResults = new(std::nothrow) wchar_t[126];
						if (strResults)
						{
							memset(strResults, 0, sizeof(wchar_t) * 126);
							BOOLEAN getResult = HidD_GetSerialNumberString(devHandle, strResults, sizeof(wchar_t) * 126);
							// Everything matches!
							if (getResult && serialNumber == strResults)
							{
								delete[] strResults;
								CloseHandle(devHandle);

								_WinRawInputDevices[i]->SetRawInputHandle(riDevice);
								_WinRawInputDevices[i]->SetDeviceConnected(true);
								_WinRawInputDevices[i]->SetParentPath((wchar_t *)parentInst.c_str());
								_WinRawInputDevices[i]->SetPath((wchar_t *)path.c_str());
								return true;
							}
							delete[] strResults;
						}
					}

					CloseHandle(devHandle);
				}
			}
			else
			{
				// Scan for existing device.
				for (size_t i = 0; i < _WinRawInputDevices.size(); i++)
				{
					if (_WinRawInputDevices[i]->GetPath() == path)
					{
						_WinRawInputDevices[i]->SetRawInputHandle(riDevice);
						_WinRawInputDevices[i]->SetDeviceConnected(true);
						return true;
					}
				}
			}
			//*/


			// Create the WinDevice and RawInputDevice.
			WinDevice *newDevice = new(std::nothrow) WinDevice(riDevice, (wchar_t *)path.c_str(), (wchar_t *)parentInst.c_str(), nullptr);
			if (!newDevice) return true;
			_WinRawInputDevices.push_back(newDevice);
			_UnsynchedRawInputDevices.push_back(newDevice);

			// Create the interface.
			RawInputDevice *newRiDevice = new(std::nothrow) RawInputDevice(newDevice);
			if (!newRiDevice) return true;
			_RawInputInterfaces.push_back(newRiDevice);
			_UnsynchedRawInputInterfaces.push_back(newRiDevice);

			_SyncDevices();
			break;
		}
		case GIDC_REMOVAL:
		{
			HRAWINPUT hRawInput = (HRAWINPUT)lParam;
			for (size_t i = 0; i < _WinRawInputDevices.size(); i++)
			{
				if (_WinRawInputDevices[i]->GetRawInputHandle() == hRawInput)
				{
					_WinRawInputDevices[i]->SetDeviceConnected(false);
					return true;
				}
			}
			break;
		}
		}
		return true;
	}
	return false;
}

void WinInputSystem::_ProcessRiJoysticks(RAWINPUT *rawInput)
{
	for (size_t i = 0; i < _WinRawInputDevices.size(); i++)
	{
		if (_WinRawInputDevices[i]->GetRawInputHandle() == rawInput->header.hDevice)
		{
			_RawInputInterfaces[i]->SaveReport((char *)rawInput->data.hid.bRawData, (size_t)rawInput->data.hid.dwSizeHid * rawInput->data.hid.dwCount);
			break;
		}
	}
}


}}}

