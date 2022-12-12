#pragma once

namespace TChapman500 {
namespace Input {

class InputControl
{
public:
	InputControl(unsigned short usagePage, unsigned short usage);
	virtual ~InputControl();

	virtual void SetState(void *state) = 0;
			
	unsigned short UsagePage;
	unsigned short Usage;
};

}}
