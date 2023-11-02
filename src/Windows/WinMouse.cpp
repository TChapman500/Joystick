#include "WinMouse.h"
#include <windowsx.h>

namespace TChapman500 {
namespace Input {
namespace Windows {


WinMouse::WinMouse()
{
	_Buttons = new bool[5];
	_Values = new int[6];
}

WinMouse::~WinMouse()
{
	delete[] _Buttons;
	delete[] _Values;
}

unsigned WinMouse::GetButtonCount()
{
	return 5;
}

unsigned WinMouse::GetValueCount()
{
	return 6;
}

bool WinMouse::GetInputState(bool *buttons, unsigned *values)
{
	if (!buttons || !values) return false;
	memcpy(buttons, _Buttons, 5);
	memcpy(values, _Values, 24);
	return true;
}

bool WinMouse::GetIsConnected()
{
	return true;
}

bool WinMouse::GetValueProperties(unsigned index, value_properties *properties)
{
	return false;
}

unsigned short WinMouse::GetUsagePage() { return 1; }
unsigned short WinMouse::GetUsage() { return 2; }

unsigned WinMouse::GetVendorID()
{
	return 0;
}

unsigned WinMouse::GetProductID()
{
	return 0;
}

unsigned WinMouse::GetVersionNumber()
{
	return 0;
}

const wchar_t *WinMouse::GetVendorName()
{
	return nullptr;
}

const wchar_t *WinMouse::GetProductName()
{
	return nullptr;
}

const wchar_t *WinMouse::GetSerialNumber()
{
	return nullptr;
}

const wchar_t *WinMouse::GetInterfaceName() { return L"RawInput Mouse"; }

void WinMouse::RawInputMessage(RAWMOUSE *data)
{
	// Relative Mouse Movement
	_Values[2] = data->lLastX;
	_Values[3] = data->lLastY;

	// Update Mouse Buttons.
	if ((data->usFlags & RI_MOUSE_LEFT_BUTTON_DOWN) == RI_MOUSE_LEFT_BUTTON_DOWN) _Buttons[0] = true;
	if ((data->usFlags & RI_MOUSE_LEFT_BUTTON_UP) == RI_MOUSE_LEFT_BUTTON_UP) _Buttons[0] = false;
	if ((data->usFlags & RI_MOUSE_RIGHT_BUTTON_DOWN) == RI_MOUSE_RIGHT_BUTTON_DOWN) _Buttons[1] = true;
	if ((data->usFlags & RI_MOUSE_RIGHT_BUTTON_UP) == RI_MOUSE_RIGHT_BUTTON_UP) _Buttons[1] = false;
	if ((data->usFlags & RI_MOUSE_MIDDLE_BUTTON_DOWN) == RI_MOUSE_MIDDLE_BUTTON_DOWN) _Buttons[2] = true;
	if ((data->usFlags & RI_MOUSE_MIDDLE_BUTTON_UP) == RI_MOUSE_MIDDLE_BUTTON_UP) _Buttons[2] = false;
	if ((data->usFlags & RI_MOUSE_BUTTON_4_DOWN) == RI_MOUSE_BUTTON_4_DOWN) _Buttons[3] = true;
	if ((data->usFlags & RI_MOUSE_BUTTON_4_UP) == RI_MOUSE_BUTTON_4_UP) _Buttons[3] = false;
	if ((data->usFlags & RI_MOUSE_BUTTON_5_DOWN) == RI_MOUSE_BUTTON_5_DOWN) _Buttons[4] = true;
	if ((data->usFlags & RI_MOUSE_BUTTON_5_UP) == RI_MOUSE_BUTTON_5_UP) _Buttons[4] = false;

	// Update scroll wheel.
	if ((data->usFlags & RI_MOUSE_WHEEL) == RI_MOUSE_WHEEL)
	{
		short delta = *(short*)&data->usButtonData;
		_Values[4] = 0;
		_Values[5] = delta;
	}
	else if ((data->usFlags & RI_MOUSE_HWHEEL) == RI_MOUSE_HWHEEL)
	{
		short delta = *(short *)&data->usButtonData;
		_Values[4] = delta;
		_Values[5] = 0;
	}
}

void WinMouse::MouseMoveMessage(LPARAM lParam)
{
	_Values[0] = GET_X_LPARAM(lParam);
	_Values[1] = GET_Y_LPARAM(lParam);
}

void WinMouse::ClearDeltas()
{
	_Values[2] = 0;
	_Values[3] = 0;
}

}}}
