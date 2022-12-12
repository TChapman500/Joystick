#include "XInput.h"

namespace TChapman500 {
namespace Input {
namespace Windows {

XInput::XInput(LPWSTR hidFile, LPWSTR xInputFile) : HID(hidFile)
{
}

XInput::~XInput()
{
}

unsigned XInput::GetButtonCount() { return 10; }

unsigned XInput::GetValueCount() { return 7; }

bool XInput::GetInputState(bool *buttons, unsigned *values)
{
	if (!buttons || !values) return false;

	return true;
}

bool XInput::GetIsConnected()
{
	return false;
}

bool XInput::GetValueProperties(unsigned index, value_properties *properties)
{
	// Invalid parameters
	if (!properties || index > 6) return false;

	// HAT Switch
	if (index == 0)
	{
		properties->UsagePage = 1;
		properties->Usage = 0x39;
		properties->Bits = 4;
		properties->HasNullState = true;
		properties->MinValue = 0;
		properties->MaxValue = 7;
	}
	// Axis
	else
	{
		properties->UsagePage = 1;
		properties->Usage = index - 1;
		properties->Bits = 8;
		properties->HasNullState = false;
		properties->MinValue = 0;
		properties->MaxValue = 255;
	}

	// Successful Data Retrieval
	return true;
}

const wchar_t *XInput::GetInterfaceName() { return L"XInput"; }

}}}