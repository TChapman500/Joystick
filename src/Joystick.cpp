#include "Joystick.h"
#include "InputButton.h"
#include "InputAxis.h"
#include "InputHAT.h"
#include "CustomHAT.h"
#include "IInput.h"
#include "IOutput.h"
#include <string>
#include <vector>

namespace TChapman500 {
namespace Input {

Joystick::Joystick(IInput *inputInterface, IOutput *outputInterface)
{
	InputInterface = inputInterface;
	OutputInterface = outputInterface;

	std::vector<InputButton *> tempButtonList;
	std::vector<InputAxis *> tempAxisList;
	std::vector<InputHAT *> tempHATList;

	// Initialize Buttons
	_RealButtonCount = inputInterface->GetButtonCount();
	_RawButtonStates = new bool[_RealButtonCount];
	for (size_t i = 0; i < _RealButtonCount; i++)
		tempButtonList.push_back(new InputButton(9, (unsigned short)(i + 1)));

	// Initialize values
	_ValueListSize = inputInterface->GetValueCount();
	_RawValueStates = new unsigned[_ValueListSize];
	_ValueList = new InputValue*[_ValueListSize];

	value_properties valueProps;
	for (size_t i = 0; i < _ValueListSize; i++)
	{
		InputInterface->GetValueProperties((unsigned)i, &valueProps);
		
		// This is a HAT Switch
		if (valueProps.UsagePage == 0x01 && valueProps.Usage == 0x39)
		{
			InputButton *buttonList[4];
			HATAxis *axisList[2];
			InputHAT *newHat = new InputHAT(valueProps.UsagePage, valueProps.Usage, valueProps.Bits, valueProps.MinValue, valueProps.MaxValue, valueProps.HasNullState, buttonList, axisList);
			_ValueList[i] = newHat;

			// Get the HAT Switch and it's buttons.
			tempHATList.push_back(newHat);
			tempButtonList.push_back(buttonList[0]);
			tempButtonList.push_back(buttonList[1]);
			tempButtonList.push_back(buttonList[2]);
			tempButtonList.push_back(buttonList[3]);

			tempAxisList.push_back((InputAxis *)axisList[0]);
			tempAxisList.push_back((InputAxis *)axisList[1]);

		}
		// This is an axis
		else
		{
			// Add new axis to the hidden axis list.
			InputAxis *newAxis = new InputAxis(valueProps.UsagePage, valueProps.Usage, valueProps.Bits, valueProps.MinValue & 0xFFFF, valueProps.MaxValue & 0xFFFF, valueProps.HasNullState);
			_ValueList[i] = newAxis;

			// We'll use this to determine which axis to expose to the user.
			switch (valueProps.UsagePage)
			{
			case 0x01:	// Generic Desktop
				if ((valueProps.Usage >= 0x30 && valueProps.Usage <= 0x38) || (valueProps.Usage >= 0x40 && valueProps.Usage <= 0x46) || (valueProps.Usage >= 0x49 && valueProps.Usage <= 0x4C))
					tempAxisList.push_back(newAxis);
				break;
			case 0x02:	// Simulation Controls
				if ((valueProps.Usage >= 0xB0 && valueProps.Usage <= 0xB2) || (valueProps.Usage >= 0xB5 && valueProps.Usage <= 0xB6) || valueProps.Usage == 0xBF || (valueProps.Usage >= 0xC3 && valueProps.Usage <= 0xD0))
					tempAxisList.push_back(newAxis);
				break;
			case 0x05:	// Game Controls
				if (valueProps.Usage >= 0x21 && valueProps.Usage <= 0x29)
					tempAxisList.push_back(newAxis);
				break;
			}
		}
	}

	// Populate final button list
	_ButtonListSize = tempButtonList.size();
	_ButtonList = new InputButton*[_ButtonListSize];
	for (size_t i = 0; i < _ButtonListSize; i++) _ButtonList[i] = tempButtonList[i];

	// Populate final axis list
	_AxisListSize = tempAxisList.size();
	_AxisList = new InputAxis*[_AxisListSize];
	for (size_t i = 0; i < _AxisListSize; i++) _AxisList[i] = tempAxisList[i];

	// Populate final hat list
	_HATListSize = tempHATList.size();
	_RealHATCount = _HATListSize;
	_HATList = new InputHAT*[_HATListSize];
	for (size_t i = 0; i < _HATListSize; i++) _HATList[i] = tempHATList[i];
}

Joystick::~Joystick()
{
	for (size_t i = 0; i < _ButtonListSize; i++) delete _ButtonList[i];
	delete[] _ButtonList;

	for (size_t i = 0; i < _AxisListSize; i++) delete _AxisList[i];
	delete[] _AxisList;

	for (size_t i = 0; i < _HATListSize; i++) delete _HATList[i];
	delete[] _HATList;

	delete[] _ValueList;
	delete[] _RawValueStates;
	delete[] _RawButtonStates;
}

void Joystick::ReadJoystickState()
{
	// Read Device States
	if (!InputInterface->GetInputState(_RawButtonStates, _RawValueStates))
	{
		int test = 0;
		return;
	}

	// Update Button States
	for (size_t i = 0; i < _RealButtonCount; i++)
		_ButtonList[i]->SetState(&_RawButtonStates[i]);

	// Update Value States
	for (size_t i = 0; i < _ValueListSize; i++)
		_ValueList[i]->SetState(&_RawValueStates[i]);

	// Update Fake HAT States
	for (size_t i = _RealHATCount; i < _HATListSize; i++)
		_HATList[i]->SetState(nullptr);
}

bool Joystick::AddCustomHAT(unsigned upButton, unsigned downButton, unsigned rightButton, unsigned leftButton)
{
	// Invalid Parameter check.
	if (upButton >= _RealButtonCount || downButton >= _RealButtonCount || rightButton >= _RealButtonCount || leftButton >= _RealButtonCount)
		return false;
	if (upButton == downButton || upButton == rightButton || upButton == leftButton || downButton == rightButton || downButton == leftButton || rightButton == leftButton)
		return false;

	// Create a new HAT List
	InputHAT **newList = new(std::nothrow) InputHAT * [_HATListSize + 1];
	if (!newList) return false;

	// Copy HATs to new list
	for (size_t i = 0; i < _HATListSize; i++)
		newList[i] = _HATList[i];

	// Create the new HAT from the desired buttons
	newList[_HATListSize] = new(std::nothrow) CustomHAT(_ButtonList[upButton], _ButtonList[downButton], _ButtonList[rightButton], _ButtonList[leftButton]);
	if (!newList[_HATListSize])
	{
		delete[] newList;
		return false;
	}

	// Delete and reassign old list
	delete[] _HATList;
	_HATList = newList;

	// Increment list size
	_HATListSize++;
	return true;
}

unsigned short Joystick::GetUsagePage() { return InputInterface->GetUsagePage(); }
unsigned short Joystick::GetUsage() { return InputInterface->GetUsage(); }
unsigned short Joystick::GetVendorID() { return InputInterface->GetVendorID(); }
unsigned short Joystick::GetProductID() { return InputInterface->GetProductID(); }
const wchar_t *Joystick::GetInterfaceName()
{
	std::wstring resultIn(InputInterface->GetInterfaceName());

	if (OutputInterface)
	{
		std::wstring resultOut(OutputInterface->GetInterfaceName());
		if (resultIn == resultOut)
		{
			size_t length = wcslen(resultIn.c_str()) + 1;
			wchar_t *result = new wchar_t[length];
			wcscpy_s(result, length, resultIn.c_str());
			return result;
		}
		else
		{
			std::wstring finalResult = resultIn + L"/" + resultOut;
			size_t length = wcslen(finalResult.c_str()) + 1;
			wchar_t *result = new wchar_t[length];
			wcscpy_s(result, length, finalResult.c_str());
			return result;
		}

	}
	else
	{
		size_t length = wcslen(resultIn.c_str()) + 1;
		wchar_t *result = new wchar_t[length];
		wcscpy_s(result, length, resultIn.c_str());
		return result;
	}
}
const wchar_t *Joystick::GetVendorName() { return InputInterface->GetVendorName(); }
const wchar_t *Joystick::GetProductName() { return InputInterface->GetProductName(); }

InputAxis *Joystick::GetAxis(unsigned index)
{
	if (index >= _AxisListSize) return nullptr;
	return _AxisList[index];
}

InputButton *Joystick::GetButton(unsigned index)
{
	if (index >= _ButtonListSize) return nullptr;
	return _ButtonList[index];
}

InputHAT *Joystick::GetHAT(unsigned index)
{
	if (index >= _HATListSize) return nullptr;
	return _HATList[index];
}

bool Joystick::GetButtonState(unsigned index)
{
	if (index >= _ButtonListSize) return false;
	return _ButtonList[index]->IsPressed;
}

bool Joystick::GetButtonRisingEdge(unsigned index)
{
	if (index >= _ButtonListSize) return false;
	return _ButtonList[index]->RisingEdge;
}

bool Joystick::GetButtonFallingEdge(unsigned index)
{
	if (index >= _ButtonListSize) return false;
	return _ButtonList[index]->FallingEdge;
}

unsigned short Joystick::GetButtonUsage(unsigned index)
{
	if (index >= _ButtonListSize) return false;
	return _ButtonList[index]->Usage;
}

int Joystick::GetAxisValue(unsigned index)
{
	if (index >= _AxisListSize) return 0;
	return _AxisList[index]->Value;
}

float Joystick::GetAxisCenterRelative(unsigned index)
{
	if (index >= _AxisListSize) return 0.0f;
	return _AxisList[index]->CenterRelative;
}

float Joystick::GetAxisEndRelative(unsigned index)
{
	if (index >= _AxisListSize) return 0.0f;
	return _AxisList[index]->EndRelative;
}

bool Joystick::GetAxisProperties(value_properties &properties, unsigned index)
{
	if (index >= _AxisListSize) return false;
	properties.Usage = _AxisList[index]->Usage;
	properties.UsagePage = _AxisList[index]->UsagePage;
	properties.Bits = _AxisList[index]->Bits;
	properties.MinValue = _AxisList[index]->MinValue;
	properties.MaxValue = _AxisList[index]->MaxValue;
	properties.HasNullState = _AxisList[index]->HasNullState;
	properties.Bits = _AxisList[index]->Bits;
	return true;
}

hat_position Joystick::GetHATPosition(unsigned index)
{
	if (index >= _HATListSize) return hat_position::Centered;
	return _HATList[index]->Position;
}

unsigned Joystick::GetButtonCount() { return (unsigned)_ButtonListSize; }
unsigned Joystick::GetAxisCount() { return (unsigned)_AxisListSize; }
unsigned Joystick::GetHATCount() { return (unsigned)_HATListSize; }

}}
