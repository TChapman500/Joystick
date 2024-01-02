#pragma once
namespace TChapman500 {
namespace Input {

struct value_properties
{
	unsigned short UsagePage;
	unsigned short Usage;
	unsigned short Bits;
	bool HasNullState;
	int MinValue;
	int MaxValue;
};

struct value_state
{
	int RawValue;
	float CenterRelative;
	float EndRelative;
};

enum class hat_position
{
	None = -1,
	Up,
	UpRight,
	Right,
	DownRight,
	Down,
	DownLeft,
	Left,
	UpLeft,
	Centered
};

enum class key_code : unsigned
{
	None, Escape, F1, F2, F3, F4, F5, F6, F7, F8, F9, F10, F11, F12, PrintScreen, ScrollLock, Pause,
	A, B, C, D, E, F, G, H, I, J, K, L, M, N, O, P, Q, R, S, T, U, V, W, X, Y, Z,
	Grave, Alpha0, Alpha1, Alpha2, Alpha3, Alpha4, Alpha5, Alpha6, Alpha7, Alpha8, Alpha9, Minus, Equal, Backspace,
	Tab, LeftBracket, RightBracket, BackSlash, CapsLock, Semicolon, Quote, Enter,
	Shift, LeftShift, RightShift, Control, LeftControl, RightControl, Alt, LeftAlt, RightAlt, Super, LeftSuper, RightSuper,
	Comma, Period, Slash, Space, Insert, Delete, Home, End, PageUp, PageDown,
	UpArrow, DownArrow, LeftArrow, RightArrow,
	NumLock, Divide, Multiply, Subtract, Add, NumEnter, Decimal, Break, SysRq,
	Num0, Num1, Num2, Num3, Num4, Num5, Num6, Num7, Num8, Num9, Clear,
	Unknown
};



}}