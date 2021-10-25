#include "WinInputDevice.h"
#include <hidsdi.h>

namespace TChapman500
{
	namespace JoystickAPI
	{
		WinInputDevice::WinInputDevice(HANDLE devHandle)
		{
			DeviceHandle = devHandle;

			// Get information about the current device.
			unsigned size = sizeof(RID_DEVICE_INFO);
			RID_DEVICE_INFO devInfo;
			ZeroMemory(&devInfo, sizeof(RID_DEVICE_INFO));
			GetRawInputDeviceInfoA(DeviceHandle, RIDI_DEVICEINFO, &devInfo, &size);

			// Store device Information
			UsagePage = devInfo.hid.usUsagePage;
			Usage = devInfo.hid.usUsage;
			VendorID = (unsigned short)devInfo.hid.dwVendorId;
			ProductID = (unsigned short)devInfo.hid.dwProductId;

			// Get device name
			UINT dataSize = 0;
			GetRawInputDeviceInfoA(DeviceHandle, RIDI_DEVICENAME, nullptr, &dataSize);
			char *devName = new char[dataSize];
			GetRawInputDeviceInfoA(DeviceHandle, RIDI_DEVICENAME, devName, &dataSize);

			HIDHandle = CreateFileA(devName, GENERIC_READ | GENERIC_WRITE, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, OPEN_EXISTING, NULL, NULL);
			if (HIDHandle)
			{
				HidD_GetManufacturerString(HIDHandle, VendorName, sizeof(wchar_t) * 126);
				HidD_GetProductString(HIDHandle, ProductName, sizeof(wchar_t) * 126);
				HidD_GetSerialNumberString(HIDHandle, SerialNumber, sizeof(wchar_t) * 126);
			}
		}

		WinInputDevice::~WinInputDevice()
		{
			if (HIDHandle) CloseHandle(HIDHandle);
		}

		unsigned short WinInputDevice::GetUsagePage() { return UsagePage; }
		unsigned short WinInputDevice::GetUsage() { return Usage; }
		unsigned short WinInputDevice::GetVendorID() { return VendorID; }
		unsigned short WinInputDevice::GetProductID() { return ProductID; }
		void WinInputDevice::GetVendorName(wchar_t *str) { wcscpy_s(str, 126, VendorName); }
		void WinInputDevice::GetProduceName(wchar_t *str) { wcscpy_s(str, 126, ProductName); }
		void WinInputDevice::GetSerialNumber(wchar_t *str) { wcscpy_s(str, 126, SerialNumber); }
	}
}
