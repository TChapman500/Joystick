#include "WinKeyboard.h"
#include "input.h"

namespace TChapman500 {
namespace Input {
namespace Windows {


// Maps the scan codes to the key_code enum
key_code ScanCodes[128];

key_code E0ScanCodes[128] = {
	key_code::Unknown,
	key_code::Unknown,
	key_code::Unknown,
	key_code::Unknown,
	key_code::Unknown,
	key_code::Unknown,
	key_code::Unknown,
	key_code::Unknown,
	key_code::Unknown,
	key_code::Unknown,
	key_code::Unknown,
	key_code::Unknown,
	key_code::Unknown,
	key_code::Unknown,
	key_code::Unknown,
	key_code::Unknown,
	key_code::Unknown,
	key_code::Unknown,
	key_code::Unknown,
	key_code::Unknown,
	key_code::Unknown,
	key_code::Unknown,
	key_code::Unknown,
	key_code::Unknown,
	key_code::Unknown,
	key_code::Unknown,
	key_code::Unknown,
	key_code::Unknown,
	key_code::NumEnter,
	key_code::RightControl,
	key_code::Unknown,
	key_code::Unknown,
	key_code::Unknown,
	key_code::Unknown,
	key_code::Unknown,
	key_code::Unknown,
	key_code::Unknown,
	key_code::Unknown,
	key_code::Unknown,
	key_code::Unknown,
	key_code::Unknown,
	key_code::Unknown,
	key_code::Unknown,
	key_code::Unknown,
	key_code::Unknown,
	key_code::Unknown,
	key_code::Unknown,
	key_code::Unknown,
	key_code::Unknown,
	key_code::Unknown,
	key_code::Unknown,
	key_code::Unknown,
	key_code::Unknown,
	key_code::Divide,
	key_code::Unknown,
	key_code::PrintScreen,
	key_code::RightAlt,
	key_code::Unknown,
	key_code::Unknown,
	key_code::Unknown,
	key_code::Unknown,
	key_code::Unknown,
	key_code::Unknown,
	key_code::Unknown,
	key_code::Unknown,
	key_code::Unknown,
	key_code::Unknown,
	key_code::Unknown,
	key_code::Unknown,
	key_code::Clear,
	key_code::Break,
	key_code::Home,
	key_code::UpArrow,
	key_code::PageUp,
	key_code::DownArrow,
	key_code::PageDown,
	key_code::Insert,
	key_code::Delete
};

key_code E1ScanCodes[128] = {
	key_code::Unknown,
	key_code::Unknown,
	key_code::Unknown,
	key_code::Unknown,
	key_code::Unknown,
	key_code::Unknown,
	key_code::Unknown,
	key_code::Unknown,
	key_code::Unknown,
	key_code::Unknown,
	key_code::Unknown,
	key_code::Unknown,
	key_code::Unknown,
	key_code::Unknown,
	key_code::Unknown,
	key_code::Unknown,
	key_code::Unknown,
	key_code::Unknown,
	key_code::Unknown,
	key_code::Unknown,
	key_code::Unknown,
	key_code::Unknown,
	key_code::Unknown,
	key_code::Unknown,
	key_code::Unknown,
	key_code::Unknown,
	key_code::Unknown,
	key_code::Unknown,
	key_code::Unknown,
	key_code::Pause,
};




WinKeyboard::WinKeyboard()
{
	// There should only be one instance of the keyboard.
	for (unsigned i = 0; i < 128; i++)
	{
		ScanCodes[i] = key_code::Unknown;
		E0ScanCodes[i] = key_code::Unknown;
		E1ScanCodes[i] = key_code::Unknown;
	}

	// scan codes without a prefix
	{
		ScanCodes[0x01] = key_code::Escape;
		ScanCodes[0x02] = key_code::Alpha1;
		ScanCodes[0x03] = key_code::Alpha2;
		ScanCodes[0x04] = key_code::Alpha3;
		ScanCodes[0x05] = key_code::Alpha4;
		ScanCodes[0x06] = key_code::Alpha5;
		ScanCodes[0x07] = key_code::Alpha6;
		ScanCodes[0x08] = key_code::Alpha7;
		ScanCodes[0x09] = key_code::Alpha8;
		ScanCodes[0x0A] = key_code::Alpha9;
		ScanCodes[0x0B] = key_code::Alpha0;
		ScanCodes[0x0C] = key_code::Minus;
		ScanCodes[0x0D] = key_code::Equal;
		ScanCodes[0x0E] = key_code::Backspace;
		ScanCodes[0x0F] = key_code::Tab;
		ScanCodes[0x10] = key_code::Q;
		ScanCodes[0x11] = key_code::W;
		ScanCodes[0x12] = key_code::E;
		ScanCodes[0x13] = key_code::R;
		ScanCodes[0x14] = key_code::T;
		ScanCodes[0x15] = key_code::Y;
		ScanCodes[0x16] = key_code::U;
		ScanCodes[0x17] = key_code::I;
		ScanCodes[0x18] = key_code::O;
		ScanCodes[0x19] = key_code::P;
		ScanCodes[0x1A] = key_code::LeftBracket;
		ScanCodes[0x1B] = key_code::RightBracket;
		ScanCodes[0x1C] = key_code::Enter;
		ScanCodes[0x1D] = key_code::LeftControl;
		ScanCodes[0x1E] = key_code::A;
		ScanCodes[0x1F] = key_code::S;
		ScanCodes[0x20] = key_code::D;
		ScanCodes[0x21] = key_code::F;
		ScanCodes[0x22] = key_code::G;
		ScanCodes[0x23] = key_code::H;
		ScanCodes[0x24] = key_code::J;
		ScanCodes[0x25] = key_code::K;
		ScanCodes[0x26] = key_code::L;
		ScanCodes[0x27] = key_code::Semicolon;
		ScanCodes[0x28] = key_code::Quote;
		ScanCodes[0x29] = key_code::Grave;
		ScanCodes[0x2A] = key_code::LeftShift;
		ScanCodes[0x2B] = key_code::BackSlash;
		ScanCodes[0x2C] = key_code::Z;
		ScanCodes[0x2D] = key_code::X;
		ScanCodes[0x2E] = key_code::C;
		ScanCodes[0x2F] = key_code::V;
		ScanCodes[0x30] = key_code::B;
		ScanCodes[0x31] = key_code::N;
		ScanCodes[0x32] = key_code::M;
		ScanCodes[0x33] = key_code::Comma;
		ScanCodes[0x34] = key_code::Period;
		ScanCodes[0x35] = key_code::Slash;
		ScanCodes[0x36] = key_code::RightShift;
		ScanCodes[0x37] = key_code::Multiply;
		ScanCodes[0x38] = key_code::LeftAlt;
		ScanCodes[0x39] = key_code::Space;
		ScanCodes[0x3A] = key_code::CapsLock;
		ScanCodes[0x3B] = key_code::F1;
		ScanCodes[0x3C] = key_code::F2;
		ScanCodes[0x3D] = key_code::F3;
		ScanCodes[0x3E] = key_code::F4;
		ScanCodes[0x3F] = key_code::F5;
		ScanCodes[0x40] = key_code::F6;
		ScanCodes[0x41] = key_code::F7;
		ScanCodes[0x42] = key_code::F8;
		ScanCodes[0x43] = key_code::F9;
		ScanCodes[0x44] = key_code::F10;
		ScanCodes[0x45] = key_code::NumLock;
		ScanCodes[0x46] = key_code::ScrollLock;
		ScanCodes[0x47] = key_code::Num7;
		ScanCodes[0x48] = key_code::Num8;
		ScanCodes[0x49] = key_code::Num9;
		ScanCodes[0x4A] = key_code::Subtract;
		ScanCodes[0x4B] = key_code::Num4;
		ScanCodes[0x4C] = key_code::Num5;
		ScanCodes[0x4D] = key_code::Num6;
		ScanCodes[0x4E] = key_code::Add;
		ScanCodes[0x4F] = key_code::Num1;
		ScanCodes[0x50] = key_code::Num2;
		ScanCodes[0x51] = key_code::Num3;
		ScanCodes[0x52] = key_code::Num0;
		ScanCodes[0x53] = key_code::Decimal;
		ScanCodes[0x54] = key_code::SysRq;
		ScanCodes[0x57] = key_code::F11;
		ScanCodes[0x58] = key_code::F12;
	}

	// scan codes with prefix 0xE0
	{
		E0ScanCodes[0x1C] = key_code::NumEnter;
		E0ScanCodes[0x1D] = key_code::RightControl;
		E0ScanCodes[0x2A] = key_code::PrintScreen;
		E0ScanCodes[0x35] = key_code::Divide;
		E0ScanCodes[0x37] = key_code::PrintScreen;
		E0ScanCodes[0x38] = key_code::RightAlt;
		E0ScanCodes[0x45] = key_code::Clear;		// Something + Num Lock
		E0ScanCodes[0x46] = key_code::Break;		// Ctrl + Pause
		E0ScanCodes[0x47] = key_code::Home;
		E0ScanCodes[0x48] = key_code::UpArrow;
		E0ScanCodes[0x49] = key_code::PageUp;
		E0ScanCodes[0x4B] = key_code::LeftArrow;
		E0ScanCodes[0x4D] = key_code::RightArrow;
		E0ScanCodes[0x4F] = key_code::End;
		E0ScanCodes[0x50] = key_code::DownArrow;
		E0ScanCodes[0x51] = key_code::PageDown;
		E0ScanCodes[0x52] = key_code::Insert;
		E0ScanCodes[0x53] = key_code::Delete;
		E0ScanCodes[0x5B] = key_code::LeftSuper;
		E0ScanCodes[0x5C] = key_code::RightSuper;
	}

	// scan codes with prefix 0xE1
	{
		E1ScanCodes[0x1D] = key_code::Pause;
	}

	_Keys = new bool[(unsigned)key_code::Unknown * 2];
	for (unsigned i = 0; i < (unsigned)key_code::Unknown * 2; i++) _Keys[i] = false;

	_Values = new unsigned[3];
	_Values[0] = 0;
	_Values[1] = 0;
	_Values[2] = 0;
}

WinKeyboard::~WinKeyboard()
{
	delete[] _Keys;
	delete[] _Values;
}

unsigned WinKeyboard::GetButtonCount()
{
	return (unsigned)key_code::Unknown;
}

unsigned WinKeyboard::GetValueCount() { return 3; }

bool WinKeyboard::GetInputState(bool *buttons, unsigned *values)
{
	if (!buttons || !values) return false;
	memcpy(buttons, _Keys, (size_t)key_code::Unknown * 2);
	memset(_Keys + (size_t)key_code::Unknown, 0, (size_t)key_code::Unknown);
	memcpy(values, _Values, 12);
	return true;
}

bool WinKeyboard::GetIsConnected() { return true; }

bool WinKeyboard::GetValueProperties(unsigned index, value_properties *properties)
{
	return false;
}

unsigned short WinKeyboard::GetUsagePage() { return 1; }
unsigned short WinKeyboard::GetUsage() { return 6; }

unsigned WinKeyboard::GetVendorID()
{
	return 0;
}

unsigned WinKeyboard::GetProductID()
{
	return 0;
}

unsigned WinKeyboard::GetVersionNumber()
{
	return 0;
}

const wchar_t *WinKeyboard::GetVendorName()
{
	return nullptr;
}

const wchar_t *WinKeyboard::GetProductName()
{
	return nullptr;
}

const wchar_t *WinKeyboard::GetSerialNumber()
{
	return nullptr;
}

const wchar_t *WinKeyboard::GetInterfaceName() { return L"RawInput Keyboard"; }

void WinKeyboard::RawInputMessage(RAWKEYBOARD *data)
{
	key_code key;

	// Process E0 Prefix
	if ((data->Flags & RI_KEY_E0) == RI_KEY_E0)
		key = E0ScanCodes[data->MakeCode];
	// Process E1 Prefix
	else if ((data->Flags & RI_KEY_E1) == RI_KEY_E1)
		key = E1ScanCodes[data->MakeCode];
	// Process null Prefix
	else key = ScanCodes[data->MakeCode];

	// Process make flag
	if ((data->Flags & RI_KEY_BREAK) == RI_KEY_BREAK)
	{
		_Keys[(unsigned)key] = false;
		_Keys[(unsigned)key + (unsigned)key_code::Unknown] = false;

		switch (key)
		{
		case key_code::LeftAlt:
		case key_code::RightAlt:
			if (!_Keys[(unsigned)key_code::LeftAlt] && !_Keys[(unsigned)key_code::RightAlt])
			{
				_Keys[(unsigned)key_code::Alt] = false;
				_Keys[(unsigned)key_code::Alt + (unsigned)key_code::Unknown] = false;
			}
			break;
		case key_code::LeftShift:
		case key_code::RightShift:
			if (!_Keys[(unsigned)key_code::LeftShift] && !_Keys[(unsigned)key_code::RightShift])
			{
				_Keys[(unsigned)key_code::Shift] = false;
				_Keys[(unsigned)key_code::Shift + (unsigned)key_code::Unknown] = false;
			}
			break;
		case key_code::LeftControl:
		case key_code::RightControl:
			if (!_Keys[(unsigned)key_code::LeftControl] && !_Keys[(unsigned)key_code::RightControl])
			{
				_Keys[(unsigned)key_code::Control] = false;
				_Keys[(unsigned)key_code::Control + (unsigned)key_code::Unknown] = false;
			}
			break;
		case key_code::LeftSuper:
		case key_code::RightSuper:
			if (!_Keys[(unsigned)key_code::LeftSuper] && !_Keys[(unsigned)key_code::RightSuper])
			{
				_Keys[(unsigned)key_code::Super] = false;
				_Keys[(unsigned)key_code::Super + (unsigned)key_code::Unknown] = false;
			}
			break;
		}
	}
	else
	{
		_Keys[(unsigned)key + (unsigned)key_code::Unknown] = true;
		_Keys[(unsigned)key] = true;

		short tKey;
		switch (key)
		{
		case key_code::CapsLock:
			tKey = GetKeyState(VK_CAPITAL);
			_Values[0] = tKey & 1;
			break;
		case key_code::NumLock:
			tKey = GetKeyState(VK_NUMLOCK);
			_Values[1] = tKey & 1;
			break;
		case key_code::ScrollLock:
			tKey = GetKeyState(VK_SCROLL);
			_Values[2] = tKey & 1;
			break;
		case key_code::LeftAlt:
		case key_code::RightAlt:
			_Keys[(unsigned)key_code::Alt] = true;
			_Keys[(unsigned)key_code::Alt + (unsigned)key_code::Unknown] = true;
			break;
		case key_code::LeftShift:
		case key_code::RightShift:
			_Keys[(unsigned)key_code::Shift] = true;
			_Keys[(unsigned)key_code::Shift + (unsigned)key_code::Unknown] = true;
			break;
		case key_code::LeftControl:
		case key_code::RightControl:
			_Keys[(unsigned)key_code::Control] = true;
			_Keys[(unsigned)key_code::Control + (unsigned)key_code::Unknown] = true;
			break;
		case key_code::LeftSuper:
		case key_code::RightSuper:
			_Keys[(unsigned)key_code::Super] = true;
			_Keys[(unsigned)key_code::Super + (unsigned)key_code::Unknown] = true;
			break;
		}
	}
}

}}}