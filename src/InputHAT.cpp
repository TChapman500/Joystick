#include "InputHAT.h"
#include "InputButton.h"
#include "HATAxis.h"
#include <math.h>

namespace TChapman500 {
namespace Input {

InputHAT::InputHAT(unsigned short usagePage, unsigned short usage, unsigned short bits, int min, int max, bool null, InputButton **buttonList, HATAxis **axisList) : InputValue(usagePage, usage, bits, min, max, null)
{
	// The parent device takes ownership of these pointers.
	UpButton = nullptr;
	DownButton = nullptr;
	RightButton = nullptr;
	LeftButton = nullptr;

	XAxis = nullptr;
	YAxis = nullptr;

	if (buttonList)
	{
		// The parent device takes ownership of these pointers.
		UpButton = new InputButton(9, 60001);
		DownButton = new InputButton(9, 60002);
		RightButton = new InputButton(9, 60003);
		LeftButton = new InputButton(9, 60004);

		buttonList[0] = UpButton;
		buttonList[1] = DownButton;
		buttonList[2] = RightButton;
		buttonList[3] = LeftButton;
	}

	if (axisList)
	{
		// UsagePage, Usage
		XAxis = new HATAxis(1, 60001);
		YAxis = new HATAxis(1, 60002);

		axisList[0] = XAxis;
		axisList[1] = YAxis;
	}
}

InputHAT::~InputHAT()
{
}

void InputHAT::SetState(void *state)
{
	InputValue::SetState(state);

	// HAT Switch is Centered
	if (Value < MinValue || Value > MaxValue)
	{
		if (Position != hat_position::Centered)
		{
			ChangeFrom = Position;
			ChangeTo = hat_position::Centered;
			Position = hat_position::Centered;
		}
		else
		{
			ChangeFrom = hat_position::None;
			ChangeTo = hat_position::None;
		}
	}
	// HAT Switch is Not Centered
	else
	{
		hat_position position = (hat_position)(Value - MinValue);
		if (position != Position)
		{
			ChangeFrom = Position;
			ChangeTo = position;
			Position = position;
		}
		else
		{
			ChangeFrom = hat_position::None;
			ChangeTo = hat_position::None;
		}
	}

	bool buttonStates[4] = { false, false, false, false };

	float xValue = 0.0f;
	float yValue = 0.0f;
	const float sqrtHalf = (float)sqrt(0.5);

	switch (Position)
	{
	case hat_position::Up:
		buttonStates[0] = true;
		yValue = 1.0f;
		break;
	case hat_position::UpRight:
		buttonStates[0] = true;
		buttonStates[2] = true;
		xValue = sqrtHalf;
		yValue = sqrtHalf;
		break;
	case hat_position::Right:
		buttonStates[2] = true;
		xValue = 1.0f;
		break;
	case hat_position::DownRight:
		buttonStates[1] = true;
		buttonStates[2] = true;
		xValue = sqrtHalf;
		yValue = -sqrtHalf;
		break;
	case hat_position::Down:
		buttonStates[1] = true;
		yValue = -1.0f;
		break;
	case hat_position::DownLeft:
		buttonStates[1] = true;
		buttonStates[3] = true;
		xValue = -sqrtHalf;
		yValue = -sqrtHalf;
		break;
	case hat_position::Left:
		buttonStates[3] = true;
		xValue = -1.0f;
		break;
	case hat_position::UpLeft:
		buttonStates[0] = true;
		buttonStates[3] = true;
		xValue = -sqrtHalf;
		yValue = sqrtHalf;
		break;
	}

	// Set fake button states.
	if (UpButton) UpButton->SetState(&buttonStates[0]);
	if (DownButton) DownButton->SetState(&buttonStates[1]);
	if (RightButton) RightButton->SetState(&buttonStates[2]);
	if (LeftButton) LeftButton->SetState(&buttonStates[3]);

	if (XAxis) XAxis->SetState(&xValue);
	if (YAxis) YAxis->SetState(&yValue);
}

}}