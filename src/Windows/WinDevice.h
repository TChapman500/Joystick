#pragma once
#include <Windows.h>

namespace TChapman500 {
namespace Input {
namespace Windows {

class WinDevice
{
public:
	WinDevice(HANDLE riHandle, LPWSTR path, LPWSTR parentPath, LPWSTR childPath);
	~WinDevice();

	void SetRawInputHandle(HANDLE riHandle);
	void SetDeviceConnected(bool connected);
	void SetParentPath(LPWSTR parentPath);
	void SetChildPath(LPWSTR childPath);
	void SetPath(LPWSTR path);

	HANDLE GetRawInputHandle();
	HANDLE GetFileHandle();

	LPWSTR GetParentPath();
	LPWSTR GetChildPath();
	LPWSTR GetPath();

private:
	HANDLE _RawInputHandle = nullptr;		// Handle provided by Raw Input API.
	LPWSTR _DevicePath = nullptr;		// Path to this device.
	HANDLE _DeviceFileHandle = nullptr;	// For when the file is opened.
	OVERLAPPED _OverlappedResult;

	// If the parent of one "device" matches the child of another, they are the same physical device.
	LPWSTR _ParentPath = nullptr;		// Path to parent device (used by "ig_" devices)
	LPWSTR _ChildPath = nullptr;		// Path to child device (used by XInput devices)

};

}}}