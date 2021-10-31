#include "DirectInputDevice.h"

#include <Windows.h>
#include <hidpi.h>
using TChapman500::JoystickAPI::DirectInputDevice;

struct devObjectParam
{
	TChapman500::JoystickAPI::DirectInputDevice *Device;
};

unsigned currValue = 0;


namespace TChapman500
{
	namespace JoystickAPI
	{
		DirectInputDevice::DirectInputDevice(IDirectInputDevice8A *device, HANDLE devHandle) : WinInputDevice(devHandle)
		{
			Device = device;


			UINT dataSize = 0;
			GetRawInputDeviceInfoA(devHandle, RIDI_PREPARSEDDATA, nullptr, &dataSize);
			Preparsed = (_HIDP_PREPARSED_DATA *)new char[dataSize];
			GetRawInputDeviceInfoA(devHandle, RIDI_PREPARSEDDATA, Preparsed, &dataSize);

			// Device Capabilities
			HidP_GetCaps(Preparsed, &Capabilities);

			ReportSize = Capabilities.InputReportByteLength;
			ReportData = new char[ReportSize];


			// Button List
			std::vector<HIDP_BUTTON_CAPS> buttonList;
			buttonList.resize(Capabilities.NumberInputButtonCaps);
			HidP_GetButtonCaps(HidP_Input, buttonList.data(), &Capabilities.NumberInputButtonCaps, Preparsed);
			BtnUsagePage = buttonList[0].UsagePage;
			BtnUsageMax = buttonList[0].Range.UsageMax;
			BtnUsageMin = buttonList[0].Range.UsageMin;
			ButtonCount = BtnUsageMax - BtnUsageMin + 1;

			// Value List
			std::vector<HIDP_VALUE_CAPS> axisList;
			axisList.resize(Capabilities.NumberInputValueCaps);
			HidP_GetValueCaps(HidP_Input, axisList.data(), &Capabilities.NumberInputValueCaps, Preparsed);

			// Populate value list for later retrieval.
			for (unsigned i = 0; i < axisList.size(); i++)
			{
				valueProperties properties;
				properties.HasNullState = axisList[i].HasNull;
				properties.Bits = axisList[i].BitSize;
				properties.MinValue = axisList[i].LogicalMin;
				properties.MaxValue = axisList[i].LogicalMax;
				properties.UsagePage = axisList[i].UsagePage;
				properties.Usage = axisList[i].Range.UsageMin;

				// All of the values that we're interested in.
				if (axisList[i].Range.UsageMin > 0x20)
					ValueProperties.push_back(properties);
			}


			// Initialize Report Format (not using DirectInput for this)
			ReportFormat.dwSize = sizeof(DIDATAFORMAT);
			ReportFormat.dwNumObjs = 0;
			ReportFormat.dwObjSize = sizeof(DIOBJECTDATAFORMAT);
			ReportFormat.dwFlags = NULL;
			ReportFormat.dwDataSize = 0;
			ReportFormat.rgodf = nullptr;

			// Set the device format
			Device->SetDataFormat(&ReportFormat);

			// Create interface and aquire the device
			Device->Acquire();
		}

		DirectInputDevice::~DirectInputDevice()
		{
			delete[] Preparsed;
			delete[] ReportData;
			Device->Unacquire();
		}

		void DirectInputDevice::ReadInputStates(bool *bStates, valueState *vStates)
		{
			unsigned bytesRead = 0;
			//BOOLEAN result = HidD_GetInputReport(HIDHandle, ReportData, ReportSize);
			ReadFile(HIDHandle, ReportData, ReportSize, (DWORD *)&bytesRead, &OverlappedResult);

			// Clear button states
			for (unsigned i = 0; i < ButtonCount; i++)
				bStates[i] = false;

			// Buffer for storing button states
			USAGE *btnStates = new USAGE[ButtonCount];
			unsigned buttonCount = ButtonCount;

			// Get pressed buttons
			HidP_GetUsages(HidP_Input, BtnUsagePage, 0, btnStates, (unsigned long*)&buttonCount, Preparsed, ReportData, ReportSize);

			// Set pressed button outputs
			for (unsigned i = 0; i < buttonCount; i++)
			{
				if (btnStates[i] > 0) bStates[btnStates[i] - BtnUsageMin] = true;
			}
			delete[] btnStates;

			// Update all axis and HAT states.
			for (int i = 0; i < ValueProperties.size(); i++)
			{
				int value = 0;

				HidP_GetUsageValue(HidP_Input, ValueProperties[i].UsagePage, 0, ValueProperties[i].Usage, (unsigned long*)&value, Preparsed, ReportData, ReportSize);
				if (ValueProperties[i].Usage == 0x39)
				{
					value = value - ValueProperties[i].MinValue;
					if (value < 0) value = 8;
					vStates[i].RawValue = value;
					vStates[i].CenterRelative = 0.0f;
					vStates[i].EndRelative = 0.0f;
				}
				else
				{
					vStates[i].RawValue = value;
					GetFloatValues(ValueProperties[i].MinValue, ValueProperties[i].MaxValue, vStates[i].RawValue, &vStates[i].CenterRelative, &vStates[i].EndRelative);
				}
			}
		}

		unsigned DirectInputDevice::GetButtonCount() { return ButtonCount; }
		unsigned DirectInputDevice::GetValueCount() { return ValueProperties.size(); }

		void DirectInputDevice::GetValueProperties(unsigned index, valueProperties *properties)
		{
			if (index < ValueProperties.size()) memcpy(properties, &ValueProperties[index], sizeof(valueProperties));
		}

		void DirectInputDevice::GetInterfaceName(char *str) { strcpy_s(str, 32, "DirectInput"); }
	}
}

