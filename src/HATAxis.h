#pragma once
#include "InputAxis.h"


namespace TChapman500 {
namespace Input {

class HATAxis : public InputAxis
{
public:
	HATAxis(unsigned short usagePage, unsigned short usage);
	~HATAxis();

	virtual void SetState(void *state) override;
};

}}
