#pragma once
#include "InputButton.h"

namespace TChapman500 {
namespace Input {

class InputKey : public InputButton
{
public:
	InputKey(unsigned short usagePage, unsigned short usage);
	virtual ~InputKey();

	virtual void SetState(void *state) override;
	void SetRepeat(bool repeat);

	bool RepeatPress = false;
};

}}
