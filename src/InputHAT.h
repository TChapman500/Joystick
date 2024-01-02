#pragma once
#include "input.h"
#include "InputValue.h"

namespace TChapman500 {
namespace Input {

class InputButton;
class HATAxis;
class InputHAT : public InputValue
{
public:


	InputHAT(unsigned short usagePage, unsigned short usage, unsigned short bits, int min, int max, bool null, InputButton **buttonList, HATAxis **axisList);
	virtual ~InputHAT();

	virtual void SetState(void *state) override;

	hat_position Position = hat_position::Centered;
	hat_position ChangeTo = hat_position::None;
	hat_position ChangeFrom = hat_position::None;
public:
	InputButton *UpButton;
	InputButton *DownButton;
	InputButton *RightButton;
	InputButton *LeftButton;

	HATAxis *XAxis;
	HATAxis *YAxis;

};

}}
