#pragma once


namespace TChapman500 {
namespace Input {

class InputButton;
class IInput;

class Mouse
{
public:
	Mouse(IInput *inputInterface);
	~Mouse();


	void ReadMouseState();

	void GetMouseScroll(int &x, int &y);
	void GetMousePosition(int &x, int &y);
	void GetMouseDelta(int &x, int &y);
	bool GetButtonState(unsigned index);
	bool GetButtonRisingEdge(unsigned index);
	bool GetButtonFallingEdge(unsigned index);
	InputButton *GetButton(unsigned index);

private:

	IInput *InputInterface;
	InputButton **Buttons;	// Always 5 buttons.



	int PositionX, PositionY;
	int DeltaX, DeltaY;
	int ScrollX, ScrollY;

	bool *_ButtonStates;
	int *_ValueStates;
};

}}
