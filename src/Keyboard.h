#pragma once
#include "input.h"

namespace TChapman500 {
namespace Input {

class IInput;
class InputKey;
class Keyboard
{
public:
	Keyboard(IInput *inputInterface);
	~Keyboard();

	InputKey *GetKey(key_code key);
	bool GetKeyState(key_code key);
	bool GetKeyRisingEdge(key_code key);
	bool GetKeyFallingEdge(key_code key);
	bool GetKeyRepeatPress(key_code key);

	bool GetScrollLock();
	bool GetCapsLock();
	bool GetNumLock();

	
	void ReadKeyboardState();

	InputKey *operator[] (key_code key);

private:
	IInput *InputInterface;
	InputKey **Keys;
	bool CapsLock, NumLock, ScrollLock;

	bool *_KeyStates;
	unsigned *_LockStates;
};

}}