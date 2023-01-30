#pragma once
#include "input.h"

namespace TChapman500 {
namespace Input {

class IInput;
class IOutput;

class InputValue;
class InputAxis;
class InputButton;
class InputHAT;

class Joystick
{
public:
	Joystick(IInput *inputInterface, IOutput *outputInterface);
	virtual ~Joystick();

	virtual void ReadJoystickState();
	bool AddCustomHAT(unsigned upButton, unsigned downButton, unsigned rightButton, unsigned leftButton);

	unsigned short GetUsagePage();
	unsigned short GetUsage();
	unsigned short GetVendorID();
	unsigned short GetProductID();

	const wchar_t *GetInterfaceName();
	const wchar_t *GetVendorName();
	const wchar_t *GetProductName();


	unsigned GetButtonCount();
	InputButton *GetButton(unsigned index);
	bool GetButtonState(unsigned index);
	bool GetButtonRisingEdge(unsigned index);
	bool GetButtonFallingEdge(unsigned index);
	unsigned short GetButtonUsage(unsigned index);

	unsigned GetAxisCount();
	int GetAxisValue(unsigned index);
	InputAxis *GetAxis(unsigned index);
	float GetAxisCenterRelative(unsigned index);
	float GetAxisEndRelative(unsigned index);
	bool GetAxisProperties(value_properties &properties, unsigned index);

	unsigned GetHATCount();
	InputHAT *GetHAT(unsigned index);
	hat_position GetHATPosition(unsigned index);
	
private:
	// Input and output interfaces
	IInput* InputInterface;
	IOutput* OutputInterface;

	// All real values on the device
	InputValue **_ValueList;
	size_t _ValueListSize;

	// All axes on the device
	InputAxis **_AxisList;
	size_t _AxisListSize;

	// All buttons on the device
	InputButton **_ButtonList;
	size_t _ButtonListSize;
	size_t _RealButtonCount;

	// All HATs on the device
	InputHAT **_HATList;
	size_t _HATListSize;
	size_t _RealHATCount;

	unsigned *_RawValueStates;
	bool *_RawButtonStates;
};

}}
