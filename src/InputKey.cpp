#include "InputKey.h"

namespace TChapman500 {
namespace Input {

InputKey::InputKey(unsigned short usagePage, unsigned short usage) : InputButton(usagePage, usage)
{
}

InputKey::~InputKey()
{
}

void InputKey::SetState(void *state)
{
	unsigned char keyState = *(unsigned char *)state;
	bool pressed = keyState & 1;
	bool repeat = (keyState >> 1) & 1;

    	InputButton::SetState(&pressed);
	RepeatPress = repeat;
}

void InputKey::SetRepeat(bool repeat)
{
	RepeatPress = repeat;
}

}}