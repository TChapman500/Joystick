#pragma once
#include "IInputSystem.h"
#include <Windows.h>
#include <vector>

namespace TChapman500 {
namespace Input {

class Joystick;
class IInput;
class IOutput;

namespace Windows {

class XInput;
class RawInputDevice;
class HID;
class WinKeyboard;
class WinMouse;
class WinDevice;


class WinInputSystem : public IInputSystem
{
public:
	WNDCLASSEXW test;

	struct init_params
	{
		bool UseKeyboards;
		bool UseMice;
		HWND MainHWnd;
		WNDPROC UseProc;
	};

	WinInputSystem();
	WinInputSystem(HWND mainHWnd);
	~WinInputSystem();

	void ResetDeviceStates();
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

	// Call this in the window procedure.
	bool WindowProc(UINT message, WPARAM wParam, LPARAM lParam);

private:
	static LRESULT CALLBACK _WindowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);


	bool _JoyMessageProc(UINT message, WPARAM wParam, LPARAM lParam);
	bool _KeyMouseMessageProc(UINT message, WPARAM wParam, LPARAM lParam);
	bool _DevConnectProc(UINT message, WPARAM wParam, LPARAM lParam);
	void _ProcessRiJoysticks(RAWINPUT *rawInput);

	typedef bool (WinInputSystem::*INPUTPROC)(UINT, WPARAM, LPARAM);
	INPUTPROC _InputProc;

	void _PopulateXInputDevices();
	void _PopulateRawInputDevices();
	void _PopulateRawInputDevices(HWND hWnd);

	void _SyncDevices();

	std::vector<WinDevice *> _UnsynchedRawInputDevices;
	std::vector<WinDevice *> _WinRawInputDevices;
	std::vector<RawInputDevice *> _UnsynchedRawInputInterfaces;
	std::vector<RawInputDevice *> _RawInputInterfaces;

	std::vector<WinDevice *> _UnsynchedXInputDevices;
	std::vector<WinDevice *> _WinXInputDevices;
	std::vector<XInput *> _UnsynchedXInputInterfaces;
	std::vector<XInput *> _XInputInterfaces;

	std::vector<IInput *> _InputInterfaces;
	std::vector<IOutput *> _OutputInterfaces;
	std::vector<Joystick *> _Joysticks;

	IInput *_IKeyboard = nullptr;
	IInput *_IMouse = nullptr;

	Keyboard *_Keyboard = nullptr;
	Mouse *_Mouse = nullptr;
	
	HWND _HWnd = nullptr;
	HDEVNOTIFY _HDevNotify = nullptr;
};

}}}

