#pragma once
#include "IInputSystem.h"
#include <dinput.h>

namespace TChapman500 {
namespace Input {

class Joystick;
class IInput;
class IOutput;

namespace Windows {

class XInput;
class DirectInput;
class HID;
class WinKeyboard;
class WinMouse;

class WinInputSystem : public IInputSystem
{
public:
	WinInputSystem();
	~WinInputSystem();


	virtual void UpdateJoystickStates() override;
	virtual unsigned GetJoystickCount() override;
	virtual Joystick *GetJoystick(unsigned index) override;

	virtual unsigned GetAxisCount(unsigned joystick) override;
	virtual int GetAxisValue(unsigned joystick, unsigned axis) override;
	virtual float GetAxisCenterRelative(unsigned joystick, unsigned axis) override;
	virtual float GetAxisEndRelative(unsigned joystick, unsigned axis) override;
	virtual bool GetAxisProperties(value_properties &properties, unsigned joystick, unsigned axis) override;

	virtual unsigned GetButtonCount(unsigned joystick) override;
	virtual bool GetButtonState(unsigned joystick, unsigned button) override;
	virtual bool GetButtonRisingEdge(unsigned joystick, unsigned button) override;
	virtual bool GetButtonFallingEdge(unsigned joystick, unsigned button) override;
	virtual unsigned short GetButtonUsage(unsigned joystick, unsigned button) override;

	virtual unsigned GetHATCount(unsigned joystick) override;
	virtual hat_position GetHATPosition(unsigned joystick, unsigned hat) override;

	virtual Mouse *GetMouse() override;
	virtual bool GetMousePosition(int &x, int &y) override;
	virtual bool GetMouseDelta(int &x, int &y) override;
	virtual bool GetMouseScroll(int &x, int &y) override;
	virtual bool GetMouseButton(unsigned button) override;
	virtual bool GetMouseRisingEdge(unsigned button) override;
	virtual bool GetMouseFallingEdge(unsigned button) override;

	virtual Keyboard *GetKeyboard() override;
	virtual bool GetKeyState(key_code key) override;
	virtual bool GetKeyRisingEdge(key_code key) override;
	virtual bool GetKeyFallingEdge(key_code key) override;
	virtual bool GetCapsLock() override;
	virtual bool GetNumLock() override;
	virtual bool GetScrollLock() override;

	// Call this in the message loop.
	bool MessageProc(UINT message, WPARAM wParam, LPARAM lParam);

private:
	IInput **_InputInterfaces;
	size_t _InputInterfaceSize;

	IOutput **_OutputInterfaces;
	size_t _OutputInterfaceSize;

	Joystick **_JoystickList;
	size_t _JoystickListSize;

	IDirectInput8A *DirectInput = nullptr;

	WinKeyboard *_IKeyboard;
	WinMouse *_IMouse;

	Keyboard *_Keyboard;
	Mouse *_Mouse;
};

}}}

