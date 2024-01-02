#include "InputValue.h"

namespace TChapman500 {
namespace Input {
InputValue::InputValue(unsigned short usagePage, unsigned short usage, unsigned short bits, int min, int max, bool null) : InputControl(usagePage, usage)
{
	Bits = bits;
	MinValue = min;
	MaxValue = max;
	HasNullState = null;
	Value = 0;
	PrevValue = 0;
}

InputValue::~InputValue() {}

void InputValue::SetState(void *state)
{
	PrevValue = Value;
	Value = *(int *)state;
}

}}