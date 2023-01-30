#include "Mouse.h"
#include "InputButton.h"
#include "IInput.h"

namespace TChapman500 {
namespace Input {

Mouse::Mouse(IInput *inputInterface)
{
	InputInterface = inputInterface;
	PositionX = 0;
	PositionY = 0;
	DeltaX = 0;
	DeltaY = 0;
	ScrollX = 0;
	ScrollY = 0;

	Buttons = new InputButton * [5];
	Buttons[0] = new InputButton(9, 1);	// Left Button
	Buttons[1] = new InputButton(9, 2);	// Right Button
	Buttons[2] = new InputButton(9, 3);	// Middle Button
	Buttons[3] = new InputButton(9, 4);	// X1 Button
	Buttons[4] = new InputButton(9, 5);	// X2 Button

	_ButtonStates = new bool[5];
	_ValueStates = new int[6];
}

Mouse::~Mouse()
{
	delete Buttons[0];
	delete Buttons[1];
	delete Buttons[2];
	delete Buttons[3];
	delete Buttons[4];
	delete[] Buttons;

	delete[] _ButtonStates;
	delete[] _ValueStates;
}

void Mouse::ReadMouseState()
{
	InputInterface->GetInputState(_ButtonStates, (unsigned *)_ValueStates);
	for (unsigned i = 0; i < 5; i++) Buttons[i]->SetState(&_ButtonStates[i]);
	PositionX = _ValueStates[0];
	PositionY = _ValueStates[1];
	DeltaX = _ValueStates[2];
	DeltaY = _ValueStates[3];
	ScrollX = _ValueStates[4];
	ScrollY = _ValueStates[5];
}

void Mouse::GetMouseScroll(int &x, int &y)
{
	x = ScrollX;
	y = ScrollY;
}

void Mouse::GetMousePosition(int &x, int &y)
{
	x = PositionX;
	y = PositionY;
}

void Mouse::GetMouseDelta(int &x, int &y)
{
	x = DeltaX;
	y = DeltaY;
}

bool Mouse::GetButtonState(unsigned index)
{
	if (index > 4) return false;
	return Buttons[index]->IsPressed;
}

bool Mouse::GetButtonRisingEdge(unsigned index)
{
	if (index > 4) return false;
	return Buttons[index]->RisingEdge;
}

bool Mouse::GetButtonFallingEdge(unsigned index)
{
	if (index > 4) return false;
	return Buttons[index]->FallingEdge;
}

InputButton *Mouse::GetButton(unsigned index)
{
	if (index > 4) return nullptr;
	return Buttons[index];
}

}}