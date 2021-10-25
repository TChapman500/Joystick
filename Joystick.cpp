#include "Joystick.h"
#include "InputButton.h"
#include "InputAxis.h"
#include "InputHAT.h"
#include "CustomHAT.h"

namespace TChapman500
{
	namespace JoystickAPI
	{
		Joystick::Joystick(IInputDevice *devInterface)
		{
			DeviceInterface = devInterface;

			// Get Joystick Information
			UsagePage = DeviceInterface->GetUsagePage();
			Usage = DeviceInterface->GetUsage();
			VendorID = DeviceInterface->GetVendorID();
			ProductID = DeviceInterface->GetProductID();
			DeviceInterface->GetVendorName(VendorName);
			DeviceInterface->GetProduceName(ProductName);
			DeviceInterface->GetSerialNumber(SerialNumber);

			// Initialize Buttons
			_RealButtonCount = DeviceInterface->GetButtonCount();
			ButtonStates = new bool[_RealButtonCount];
			for (unsigned i = 0; i < _RealButtonCount; i++)
				ButtonList.push_back(new InputButton(9, i));

			// Initialize values
			_ValueCount = DeviceInterface->GetValueCount();
			ValueStates = new valueState[_ValueCount];

			unsigned currAxis = 0;
			unsigned currHAT = 0;

			valueProperties valueProps;

			for (unsigned i = 0; i < _ValueCount; i++)
			{
				DeviceInterface->GetValueProperties(i, &valueProps);
				ValuePropertyList.push_back(valueProps);

				valueStateFunctions function;

				// This is a HAT Switch
				if (valueProps.Usage == 0x39)
				{
					_RealHATCount++;

					// Get the HAT Switch and it's buttons.
					InputButton **buttonList = new InputButton*[4];
					HATList.push_back(new InputHAT(valueProps.UsagePage, valueProps.Usage, buttonList));
					ButtonList.push_back(buttonList[0]);
					ButtonList.push_back(buttonList[1]);
					ButtonList.push_back(buttonList[2]);
					ButtonList.push_back(buttonList[3]);
					delete[] buttonList;

					function.Index = currHAT;
					function.Function = &Joystick::_SetHatState;
					_SetValueFunctions.push_back(function);
					currHAT++;
				}
				// This is an axis
				else
				{
					AxisList.push_back(new InputAxis(valueProps.UsagePage, valueProps.Usage, valueProps.Bits, valueProps.MinValue, valueProps.MaxValue));
					function.Index = currAxis;
					function.Function = &Joystick::_SetAxisState;
					_SetValueFunctions.push_back(function);
					currAxis++;
				}
			}
		}

		Joystick::~Joystick()
		{
			delete[] ButtonStates;
			delete[] ValueStates;
		}

		void Joystick::ReadJoystickState()
		{
			// Read Device States
			DeviceInterface->ReadInputStates(ButtonStates, ValueStates);

			// Update Button States
			for (unsigned i = 0; i < _RealButtonCount; i++)
				ButtonList[i]->SetState(&ButtonStates[i]);

			// Update Value States
			for (unsigned i = 0; i < _ValueCount; i++)
				(this->*_SetValueFunctions[i].Function)(_SetValueFunctions[i].Index, &ValueStates[i]);

			// Update Fake HAT States
			for (unsigned i = _RealHATCount; i < HATList.size(); i++)
				HATList[i]->SetState(nullptr);
		}

		void Joystick::AddCustomHAT(InputButton *upButton, InputButton *downButton, InputButton *rightButton, InputButton *leftButton)
		{
			HATList.push_back(new CustomHAT(upButton, downButton, rightButton, leftButton));
		}

		void Joystick::GetInterfaceName(char *str) { DeviceInterface->GetInterfaceName(str); }
		void Joystick::_SetHatState(unsigned index, void *state) { HATList[index]->SetState(state); }
		void Joystick::_SetAxisState(unsigned index, void *state) { AxisList[index]->SetState(state); }

		unsigned short Joystick::GetUsagePage() { return UsagePage; }
		unsigned short Joystick::GetUsage() { return Usage; }
		unsigned short Joystick::GetVendorID() { return VendorID; }
		unsigned short Joystick::GetProductID() { return ProductID; }
		void Joystick::GetVendorName(wchar_t *str) { wcscpy_s(str, 126, VendorName); }
		void Joystick::GetProduceName(wchar_t *str) { wcscpy_s(str, 126, ProductName); }
		void Joystick::GetSerialNumber(wchar_t *str) { wcscpy_s(str, 126, SerialNumber); }

	}
}
