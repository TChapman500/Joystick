#pragma once

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
	void AddCustomHAT(InputButton *upButton, InputButton *downButton, InputButton *rightButton, InputButton *leftButton);

	unsigned short GetUsagePage();
	unsigned short GetUsage();
	unsigned short GetVendorID();
	unsigned short GetProductID();

	const wchar_t *GetInterfaceName();
	const wchar_t *GetVendorName();
	const wchar_t *GetProductName();

	InputButton *GetButton(unsigned index);
	InputAxis *GetAxis(unsigned index);
	InputHAT *GetHAT(unsigned index);

	unsigned GetButtonCount();
	unsigned GetAxisCount();
	unsigned GetHATCount();
	
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
