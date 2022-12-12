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

class WinInputSystem : public IInputSystem
{
public:
	WinInputSystem();
	~WinInputSystem();


	virtual void UpdateJoystickStates() override;
	virtual unsigned GetJoystickCount() override;
	virtual Joystick *GetJoystick(unsigned index) override;

	// Call these in the message loop.
	LRESULT OnInputDeviceChanged(WPARAM wParam, LPARAM lParam);
	LRESULT OnInput(WPARAM wParam, LPARAM lParam);
	LRESULT OnMouseMove(WPARAM wParam, LPARAM lParam);



private:
	IInput **_InputInterfaces;
	size_t _InputInterfaceSize;

	IOutput **_OutputInterfaces;
	size_t _OutputInterfaceSize;

	Joystick **_JoystickList;
	size_t _JoystickListSize;

	IDirectInput8A *DirectInput = nullptr;
};

}}}

