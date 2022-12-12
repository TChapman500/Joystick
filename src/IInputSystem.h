#pragma once

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
};

}}


