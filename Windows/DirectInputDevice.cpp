#include "DirectInputDevice.h"

#include <hidsdi.h>
#include <Windows.h>
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
		
		BOOL DirectInputDevice::DIEnumValueObjectsCallback(LPCDIDEVICEOBJECTINSTANCEA lpddoi, LPVOID pvRef)
		{
			// Cast pointer to correct type.
			TChapman500::JoystickAPI::DirectInputDevice *device = (TChapman500::JoystickAPI::DirectInputDevice *)pvRef;

			// Initialize axis control data.
			DirectInputDevice::dInputObject *newControl = new dInputObject;
			memcpy(&newControl->ControlInstance, lpddoi, sizeof(DIDEVICEOBJECTINSTANCEA));
			newControl->ControlRange.diph.dwHeaderSize = sizeof(DIPROPHEADER);
			newControl->ControlRange.diph.dwSize = sizeof(DIPROPRANGE);
			newControl->ControlRange.diph.dwObj = newControl->ControlInstance.dwType;
			newControl->ControlRange.diph.dwHow = DIPH_BYID;
			device->Device->GetProperty(DIPROP_LOGICALRANGE, (DIPROPHEADER *)&newControl->ControlRange);

			// Add Axis to the list
			device->AxisList.push_back(newControl);

			// Get axis properties
			valueProperties newProperties;
			ZeroMemory(&newProperties, sizeof(valueProperties));

			// Current value index
			unsigned valueIndex = device->ValueProperties.size();


			// HAT Switch
			if (newControl->ControlInstance.wUsage == 0x39)
			{
				newProperties.HasNullState = true;
				newProperties.MinValue = 0;
				newProperties.MaxValue = 7;
				newProperties.Bits = 4;
			}
			// Axis
			else
			{
				newProperties.HasNullState = false;
				newProperties.MinValue = newControl->ControlRange.lMin;
				newProperties.MaxValue = newControl->ControlRange.lMax;
				// TODO:  Get bit width

				// Get Preparsed Info
				UINT dataSize = 0;
				GetRawInputDeviceInfoA(device, RIDI_PREPARSEDDATA, nullptr, &dataSize);
				_HIDP_PREPARSED_DATA *preparsed = (_HIDP_PREPARSED_DATA *)new char[dataSize];
				GetRawInputDeviceInfoA(device, RIDI_PREPARSEDDATA, preparsed, &dataSize);

				HIDP_CAPS caps;

				// Get capabilities Info
				HidP_GetCaps(preparsed, &caps);
				std::vector<HIDP_BUTTON_CAPS> buttonList;
				buttonList.resize(caps.NumberInputButtonCaps);
				HidP_GetButtonCaps(HidP_Input, buttonList.data(), &caps.NumberInputButtonCaps, preparsed);

				// Get Axis List
				std::vector<HIDP_VALUE_CAPS> axisList;
				axisList.resize(caps.NumberInputValueCaps);
				HidP_GetValueCaps(HidP_Input, axisList.data(), &caps.NumberInputValueCaps, preparsed);
				delete[] preparsed;

				newProperties.Bits = axisList[currValue].BitSize;
			}
			newProperties.Usage = newControl->ControlInstance.wUsage;
			newProperties.UsagePage = newControl->ControlInstance.wUsagePage;
			
			// Add property data to list.
			device->ValueProperties.push_back(newProperties);
			return DIENUM_CONTINUE;
		}

		BOOL DirectInputDevice::DIEnumButtonObjectsCallback(LPCDIDEVICEOBJECTINSTANCEA lpddoi, LPVOID pvRef)
		{
			// Cast pointer to correct type.
			TChapman500::JoystickAPI::DirectInputDevice *device = (TChapman500::JoystickAPI::DirectInputDevice *)pvRef;

			// Initialize button control data.
			DirectInputDevice::dInputObject *newControl = new dInputObject;
			memcpy(&newControl->ControlInstance, lpddoi, sizeof(DIDEVICEOBJECTINSTANCEA));
			ZeroMemory(&newControl->ControlRange, sizeof(DIPROPRANGE));

			// Add Button to the list
			device->ButtonList.push_back(newControl);
			return DIENUM_CONTINUE;
		}


		DirectInputDevice::DirectInputDevice(IDirectInputDevice8A *device, HANDLE devHandle) : WinInputDevice(devHandle)
		{
			Device = device;

			currValue = 0;
			Device->GetCapabilities(&Capabilities);
			Device->EnumObjects(&DIEnumValueObjectsCallback, this, DIDFT_POV | DIDFT_ABSAXIS);
			Device->EnumObjects(&DIEnumValueObjectsCallback, this, DIDFT_BUTTON);

			// Allocate space for the input report.
			ReportSize = (AxisList.size() * 4) + ButtonList.size();
			if (ReportSize % 4 != 0)
			{
				unsigned extra = 4 - (ReportSize % 4);
				ReportSize += extra;
			}

			ReportData = new char[ReportSize];
			for (unsigned i = 0; i < ReportSize; i++) ReportData[i] = 0;

			// Initialize Report Format
			ReportFormat.dwSize = sizeof(DIDATAFORMAT);
			ReportFormat.dwNumObjs = AxisList.size() + ButtonList.size();
			ReportFormat.dwObjSize = sizeof(DIOBJECTDATAFORMAT);
			ReportFormat.dwFlags = DIDF_ABSAXIS | DIDFT_POV | DIDFT_BUTTON;
			ReportFormat.dwDataSize = (4 * AxisList.size()) + ButtonList.size();
			ReportFormat.rgodf = new DIOBJECTDATAFORMAT[ReportFormat.dwNumObjs];

			// All value objects (including HAT switch)
			for (int i = 0; i < AxisList.size(); i++)
			{
				DIOBJECTDATAFORMAT *formatEntry = &ReportFormat.rgodf[i];
				ZeroMemory(formatEntry, sizeof(DIOBJECTDATAFORMAT));
				formatEntry->dwOfs = 4 * i;
				formatEntry->dwType = DIDFT_ABSAXIS | DIDFT_POV | DIDFT_ANYINSTANCE;
			}

			// All buttons
			int baseAlign = 4 * AxisList.size();
			for (int i = AxisList.size(), j = 0; j < ButtonList.size(); i++, j++)
			{
				DIOBJECTDATAFORMAT *formatEntry = &ReportFormat.rgodf[i];
				ZeroMemory(formatEntry, sizeof(DIOBJECTDATAFORMAT));
				formatEntry->dwOfs = baseAlign + i;
				formatEntry->dwType = DIDFT_BUTTON | DIDFT_ANYINSTANCE;
			}

			// Set the device format
			Device->SetDataFormat(&ReportFormat);
		}

		DirectInputDevice::~DirectInputDevice()
		{
			delete[] ReportData;
			Device->Release();
		}

		void DirectInputDevice::ReadInputStates(bool *bStates, valueState *vStates)
		{
			Device->GetDeviceState(ReportSize, ReportData);

			// Report button states
			char *data = ReportData + (AxisList.size() * 4);
			for (unsigned i = 0; i < ButtonList.size(); i++)
				bStates[i] = data[i] & 1;

			// Report axis states
			int *axis = (int *)ReportData;
			for (unsigned i = 0; i < AxisList.size(); i++)
			{
				dInputObject *currAxis = AxisList[i];

				// HAT Switch
				if (currAxis->ControlInstance.wUsage == 0x39)
				{
					// Centered
					if (axis[i] < 0) vStates[i].RawValue = 8;
					// Not Centered
					else
					{
						int min = currAxis->ControlRange.lMin;
						int max = currAxis->ControlRange.lMax;
						int stateCount = max - min + 1;
						int raw = 36000 / stateCount;

						vStates[i].RawValue = axis[i] / raw;
					}
					vStates[i].CenterRelative = 0.0f;
					vStates[i].EndRelative = 0.0f;
				}
				// Axis
				else
				{
					// Set Raw Value
					vStates[i].RawValue = axis[i];
					GetFloatValues(currAxis->ControlRange.lMin, currAxis->ControlRange.lMax, vStates[i].RawValue, &vStates[i].CenterRelative, &vStates[i].EndRelative);
				}
			}
		}

		unsigned DirectInputDevice::GetButtonCount() { return ButtonList.size(); }
		unsigned DirectInputDevice::GetValueCount() { return AxisList.size(); }

		void DirectInputDevice::GetValueProperties(unsigned index, valueProperties *properties)
		{
			if (index < ValueProperties.size()) memcpy(properties, &ValueProperties[index], sizeof(valueProperties));
		}

		void DirectInputDevice::GetInterfaceName(char *str) { strcpy_s(str, 32, "DirectInput"); }
	}
}

