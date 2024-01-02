#pragma once
#include "InputControl.h"


namespace TChapman500 {
namespace Input {
class InputValue : public InputControl
{
public:
	InputValue(unsigned short usagePage, unsigned short usage, unsigned short bits, int min, int max, bool null);
	~InputValue();

	virtual void SetState(void *state) override;

	unsigned short Bits;
	int MinValue;
	int MaxValue;
	int Value;
	int PrevValue;
	bool HasNullState;


};

}}
