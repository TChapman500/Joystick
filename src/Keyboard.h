#pragma once
#include "input.h"

namespace TChapman500 {
namespace Input {

class IInput;
class InputButton;
class Keyboard
{
public:
	Keyboard(IInput *inputInterface);
	~Keyboard();

	InputButton *GetKey(key_code key);
	bool GetKeyState(key_code key);
	bool GetKeyRisingEdge(key_code key);
	bool GetKeyFallingEdge(key_code key);

	bool GetScrollLock();
	bool GetCapsLock();
	bool GetNumLock();

	
	void ReadKeyboardState();

private:
	IInput *InputInterface;
	InputButton **Keys;
	bool CapsLock, NumLock, ScrollLock;

	bool *_KeyStates;
	unsigned *_LockStates;
};

}}