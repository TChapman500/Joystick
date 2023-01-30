#pragma once
#include "input.h"

namespace TChapman500 {
namespace Input {

class Joystick;
class Keyboard;
class Mouse;

class IInputSystem
{
public:
	IInputSystem();
	virtual ~IInputSystem();

	virtual void UpdateJoystickStates() = 0;
	virtual unsigned GetJoystickCount() = 0;
	virtual Joystick *GetJoystick(unsigned index) = 0;

	virtual unsigned GetAxisCount(unsigned joystick) = 0;
	virtual int GetAxisValue(unsigned joystick, unsigned axis) = 0;
	virtual float GetAxisCenterRelative(unsigned joystick, unsigned axis) = 0;
	virtual float GetAxisEndRelative(unsigned joystick, unsigned axis) = 0;
	virtual bool GetAxisProperties(value_properties &properties, unsigned joystick, unsigned axis) = 0;

	virtual unsigned GetButtonCount(unsigned joystick) = 0;
	virtual bool GetButtonState(unsigned joystick, unsigned button) = 0;
	virtual bool GetButtonRisingEdge(unsigned joystick, unsigned button) = 0;
	virtual bool GetButtonFallingEdge(unsigned joystick, unsigned button) = 0;
	virtual unsigned short GetButtonUsage(unsigned joystick, unsigned button) = 0;

	virtual unsigned GetHATCount(unsigned joystick) = 0;
	virtual hat_position GetHATPosition(unsigned joystick, unsigned hat) = 0;

	virtual Mouse *GetMouse() = 0;
	virtual bool GetMousePosition(int &x, int &y) = 0;
	virtual bool GetMouseDelta(int &x, int &y) = 0;
	virtual bool GetMouseScroll(int &x, int &y) = 0;
	virtual bool GetMouseButton(unsigned button) = 0;
	virtual bool GetMouseRisingEdge(unsigned button) = 0;
	virtual bool GetMouseFallingEdge(unsigned button) = 0;

	virtual Keyboard *GetKeyboard() = 0;
	virtual bool GetKeyState(key_code key) = 0;
	virtual bool GetKeyRisingEdge(key_code key) = 0;
	virtual bool GetKeyFallingEdge(key_code key) = 0;
	virtual bool GetCapsLock() = 0;
	virtual bool GetNumLock() = 0;
	virtual bool GetScrollLock() = 0;
};

}}


