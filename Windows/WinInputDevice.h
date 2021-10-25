#pragma once
#include "IInputDevice.h"
#include <Windows.h>

namespace TChapman500
{
	namespace JoystickAPI
	{
		class WinInputDevice : public IInputDevice
		{
		public:
			WinInputDevice(HANDLE devHandle);
			virtual ~WinInputDevice();

			virtual unsigned short GetUsagePage() override;
			virtual unsigned short GetUsage() override;
			virtual unsigned short GetVendorID() override;
			virtual unsigned short GetProductID() override;

			virtual void GetVendorName(wchar_t *str) override;
			virtual void GetProduceName(wchar_t *str) override;
			virtual void GetSerialNumber(wchar_t *str) override;


		protected:
			HANDLE DeviceHandle;
			HANDLE HIDHandle;

			unsigned short UsagePage;
			unsigned short Usage;
			unsigned short VendorID;
			unsigned short ProductID;

			wchar_t VendorName[126];
			wchar_t ProductName[126];
			wchar_t SerialNumber[126];
		};
	}
}

