#pragma once
#include "input.h"
#include "InputValue.h"

namespace TChapman500 {
namespace Input {

class InputButton;
class InputHAT : public InputValue
{
public:


	InputHAT(unsigned short usagePage, unsigned short usage, unsigned short bits, int min, int max, bool null, InputButton **buttonList);
	virtual ~InputHAT();

	virtual void SetState(void *state) override;

	hat_position HATPosition = hat_position::Centered;
public:
	InputButton *UpButton;
	InputButton *DownButton;
	InputButton *RightButton;
	InputButton *LeftButton;

};

}}
