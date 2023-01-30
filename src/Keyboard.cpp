#include "Keyboard.h"
#include "InputButton.h"
#include "IInput.h"
#include <memory.h>

namespace TChapman500 {
namespace Input {

Keyboard::Keyboard(IInput *inputInterface)
{
	InputInterface = inputInterface;

	CapsLock = false;
	NumLock = false;
	ScrollLock = false;

	Keys = new InputButton * [(unsigned)key_code::Unknown];
	Keys[0] = new InputButton(7, 0x00);
	Keys[1] = new InputButton(7, 0x29);

	// Function Keys
	Keys[2] = new InputButton(7, 0x3A);
	Keys[3] = new InputButton(7, 0x3B);
	Keys[4] = new InputButton(7, 0x3C);
	Keys[5] = new InputButton(7, 0x3D);
	Keys[6] = new InputButton(7, 0x3E);
	Keys[7] = new InputButton(7, 0x3F);
	Keys[8] = new InputButton(7, 0x40);
	Keys[9] = new InputButton(7, 0x41);
	Keys[10] = new InputButton(7, 0x42);
	Keys[11] = new InputButton(7, 0x43);
	Keys[12] = new InputButton(7, 0x44);
	Keys[13] = new InputButton(7, 0x45);


	Keys[14] = new InputButton(7, 0x46);
	Keys[15] = new InputButton(7, 0x47);
	Keys[16] = new InputButton(7, 0x48);

	// Letters
	Keys[17] = new InputButton(7, 0x04);
	Keys[18] = new InputButton(7, 0x05);
	Keys[19] = new InputButton(7, 0x06);
	Keys[20] = new InputButton(7, 0x07);
	Keys[21] = new InputButton(7, 0x08);
	Keys[22] = new InputButton(7, 0x09);
	Keys[23] = new InputButton(7, 0x0A);
	Keys[24] = new InputButton(7, 0x0B);
	Keys[25] = new InputButton(7, 0x0C);
	Keys[26] = new InputButton(7, 0x0D);
	Keys[27] = new InputButton(7, 0x0E);
	Keys[28] = new InputButton(7, 0x0F);
	Keys[29] = new InputButton(7, 0x10);
	Keys[30] = new InputButton(7, 0x11);
	Keys[31] = new InputButton(7, 0x12);
	Keys[32] = new InputButton(7, 0x13);
	Keys[33] = new InputButton(7, 0x14);
	Keys[34] = new InputButton(7, 0x15);
	Keys[35] = new InputButton(7, 0x16);
	Keys[36] = new InputButton(7, 0x17);
	Keys[37] = new InputButton(7, 0x18);
	Keys[38] = new InputButton(7, 0x19);
	Keys[39] = new InputButton(7, 0x1A);
	Keys[40] = new InputButton(7, 0x1B);
	Keys[41] = new InputButton(7, 0x1C);
	Keys[42] = new InputButton(7, 0x1D);

	Keys[43] = new InputButton(7, 0x35);

	// Numbers
	Keys[44] = new InputButton(7, 0x27);
	Keys[45] = new InputButton(7, 0x1E);
	Keys[46] = new InputButton(7, 0x1F);
	Keys[47] = new InputButton(7, 0x20);
	Keys[48] = new InputButton(7, 0x21);
	Keys[49] = new InputButton(7, 0x22);
	Keys[50] = new InputButton(7, 0x23);
	Keys[51] = new InputButton(7, 0x24);
	Keys[52] = new InputButton(7, 0x25);
	Keys[53] = new InputButton(7, 0x26);

	Keys[54] = new InputButton(7, 0x56);
	Keys[55] = new InputButton(7, 0x57);
	Keys[56] = new InputButton(7, 0x2A);


	Keys[57] = new InputButton(7, 0x2B);
	Keys[58] = new InputButton(7, 0x2F);
	Keys[59] = new InputButton(7, 0x30);
	Keys[60] = new InputButton(7, 0x31);

	Keys[61] = new InputButton(7, 0x39);
	Keys[62] = new InputButton(7, 0x33);
	Keys[63] = new InputButton(7, 0x34);
	Keys[64] = new InputButton(7, 0x28);

	// Shift
	Keys[65] = new InputButton(7, 0xE1);
	Keys[66] = new InputButton(7, 0xE1);
	Keys[67] = new InputButton(7, 0xE5);

	// Control
	Keys[68] = new InputButton(7, 0xE0);
	Keys[69] = new InputButton(7, 0xE0);
	Keys[70] = new InputButton(7, 0xE4);

	// Alt
	Keys[71] = new InputButton(7, 0xE2);
	Keys[72] = new InputButton(7, 0xE2);
	Keys[73] = new InputButton(7, 0xE6);

	// Super (Windows)
	Keys[74] = new InputButton(7, 0xE3);
	Keys[75] = new InputButton(7, 0xE3);
	Keys[76] = new InputButton(7, 0xE7);

	Keys[77] = new InputButton(7, 0x36);
	Keys[78] = new InputButton(7, 0x37);
	Keys[79] = new InputButton(7, 0x38);
	Keys[80] = new InputButton(7, 0x2C);

	Keys[81] = new InputButton(7, 0x49);
	Keys[82] = new InputButton(7, 0x4C);
	Keys[83] = new InputButton(7, 0x4A);
	Keys[84] = new InputButton(7, 0x4D);
	Keys[85] = new InputButton(7, 0x4B);
	Keys[86] = new InputButton(7, 0x4E);
	Keys[87] = new InputButton(7, 0x52);
	Keys[88] = new InputButton(7, 0x51);
	Keys[89] = new InputButton(7, 0x50);
	Keys[90] = new InputButton(7, 0x4F);


	Keys[91] = new InputButton(7, 0x53);
	Keys[92] = new InputButton(7, 0x54);
	Keys[93] = new InputButton(7, 0x55);
	Keys[94] = new InputButton(7, 0x56);
	Keys[95] = new InputButton(7, 0x57);
	Keys[96] = new InputButton(7, 0x58);
	Keys[97] = new InputButton(7, 0x63);
	Keys[98] = new InputButton(7, 0x48);
	Keys[99] = new InputButton(7, 0x46);

	// Number Pad Numbers
	Keys[100] = new InputButton(7, 0x62);
	Keys[101] = new InputButton(7, 0x59);
	Keys[102] = new InputButton(7, 0x5A);
	Keys[103] = new InputButton(7, 0x5B);
	Keys[104] = new InputButton(7, 0x5C);
	Keys[105] = new InputButton(7, 0x5D);
	Keys[106] = new InputButton(7, 0x5E);
	Keys[107] = new InputButton(7, 0x5F);
	Keys[108] = new InputButton(7, 0x60);
	Keys[109] = new InputButton(7, 0x61);
	Keys[110] = new InputButton(7, 0x53);

	_KeyStates = new bool[(unsigned)key_code::Unknown];
	_LockStates = new unsigned[3];
}

Keyboard::~Keyboard()
{
	for (unsigned i = (unsigned)key_code::None; i < (unsigned)key_code::Unknown; i++) delete Keys[i];
	delete[] Keys;
	delete[] _KeyStates;
	delete[] _LockStates;
}

InputButton *Keyboard::GetKey(key_code key)
{
	if (key >= key_code::Unknown) return nullptr;
	return Keys[(unsigned)key];
}

bool Keyboard::GetKeyState(key_code key)
{
	if (key >= key_code::Unknown) return false;
	return Keys[(unsigned)key]->IsPressed;
}

bool Keyboard::GetKeyRisingEdge(key_code key)
{
	if (key >= key_code::Unknown) return false;
	return Keys[(unsigned)key]->RisingEdge;
}

bool Keyboard::GetKeyFallingEdge(key_code key)
{
	if (key >= key_code::Unknown) return false;
	return Keys[(unsigned)key]->FallingEdge;
}

bool Keyboard::GetScrollLock() { return ScrollLock; }
bool Keyboard::GetCapsLock() { return CapsLock; }
bool Keyboard::GetNumLock() { return NumLock; }

void Keyboard::ReadKeyboardState()
{
	InputInterface->GetInputState(_KeyStates, _LockStates);

	// Skip null key.
	for (unsigned i = 1; i < (unsigned)key_code::Unknown; i++)
		Keys[(unsigned)i]->SetState(&_KeyStates[i]);

	// Fetch Lock States
	CapsLock = _LockStates[0] == 1;
	NumLock = _LockStates[1] == 1;
	ScrollLock = _LockStates[2] == 1;
}

}}