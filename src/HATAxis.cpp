#include "HATAxis.h"
#include <math.h>

namespace TChapman500 {
namespace Input {

HATAxis::HATAxis(unsigned short usagePage, unsigned short usage) : InputAxis(usagePage, usage, 8, 0, 255, false) {}

HATAxis::~HATAxis() {}

void HATAxis::SetState(void *state)
{
	float value = *(float *)state;

	float prevCenterRelative = CenterRelative;
	float prevEndRelative = EndRelative;
	CenterRelative = value;
	EndRelative = (value + 1.0f) * 0.5f;

	float toInt = ceilf(EndRelative * 255.0f);
	PrevValue = Value;
	Value = (int)toInt;

	CenterRelativeDelta = CenterRelative - prevCenterRelative;
	EndRelativeDelta = EndRelative - prevEndRelative;
}

}}