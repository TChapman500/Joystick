#include "WinDevice.h"
#include <new>

namespace TChapman500 {
namespace Input {
namespace Windows {

WinDevice::WinDevice(HANDLE riHandle, LPWSTR path, LPWSTR parentPath, LPWSTR childPath)
{
	_RawInputHandle = riHandle;

	if (path)
	{
		size_t length = wcslen(path) + 1;
		_DevicePath = new(std::nothrow) WCHAR[length];
		if (_DevicePath) wcscpy_s(_DevicePath, length, path);
	}

	if (parentPath)
	{
		size_t length = wcslen(parentPath) + 1;
		_ParentPath = new(std::nothrow) WCHAR[length];
		if (_ParentPath) wcscpy_s(_ParentPath, length, parentPath);
	}

	if (childPath)
	{
		size_t length = wcslen(childPath) + 1;
		_ChildPath = new(std::nothrow) WCHAR[length];
		if (_ChildPath) wcscpy_s(_ChildPath, length, childPath);
	}

	SetDeviceConnected(true);
}

WinDevice::~WinDevice()
{
	if (_DevicePath) delete[] _DevicePath;
	if (_ParentPath) delete[] _ParentPath;
	if (_ChildPath) delete[] _ChildPath;
	if (_DeviceFileHandle) CloseHandle(_DeviceFileHandle);

}

void WinDevice::SetRawInputHandle(HANDLE riHandle) { _RawInputHandle = riHandle; }

void WinDevice::SetDeviceConnected(bool connected)
{
	if (connected)
	{
		_DeviceFileHandle = CreateFileW(_DevicePath, GENERIC_READ | GENERIC_WRITE, FILE_SHARE_READ | FILE_SHARE_WRITE, nullptr, OPEN_EXISTING, FILE_FLAG_OVERLAPPED, nullptr);
	}
	else
	{
		CloseHandle(_DeviceFileHandle);
		_DeviceFileHandle = nullptr;
	}
}

void WinDevice::SetParentPath(LPWSTR parentPath)
{
	if (_ParentPath) delete[] _ParentPath;
	if (parentPath)
	{
		size_t length = wcslen(parentPath) + 1;
		_ParentPath = new(std::nothrow) WCHAR[length];
		if (_ParentPath) wcscpy_s(_ParentPath, length, parentPath);
	}
	else _ParentPath = nullptr;
}

void WinDevice::SetChildPath(LPWSTR childPath)
{
	if (_ChildPath) delete[] _ChildPath;
	if (childPath)
	{
		size_t length = wcslen(childPath) + 1;
		_ChildPath = new(std::nothrow) WCHAR[length];
		if (_ChildPath) wcscpy_s(_ChildPath, length, childPath);
	}
	else _ChildPath = nullptr;
}

void WinDevice::SetPath(LPWSTR path)
{
	if (_DevicePath) delete[] _DevicePath;
	if (path)
	{
		size_t length = wcslen(path) + 1;
		_DevicePath = new(std::nothrow) WCHAR[length];
		if (_DevicePath) wcscpy_s(_DevicePath, length, path);
	}
	else _DevicePath = nullptr;
}

HANDLE WinDevice::GetRawInputHandle() { return _RawInputHandle; }
HANDLE WinDevice::GetFileHandle() { return _DeviceFileHandle; }
LPWSTR WinDevice::GetParentPath() { return _ParentPath; }
LPWSTR WinDevice::GetChildPath() { return _ChildPath; }
LPWSTR WinDevice::GetPath() { return _DevicePath; }

}
}}