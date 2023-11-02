#include "NullKeyboard.h"
#include "input.h"
#include <string.h>

namespace TChapman500 {
namespace Input {
namespace Windows {


NullKeyboard::NullKeyboard() {}
NullKeyboard::~NullKeyboard() {}
unsigned NullKeyboard::GetButtonCount() { return (unsigned)key_code::Unknown; }
unsigned NullKeyboard::GetValueCount() { return 3; }

bool NullKeyboard::GetInputState(bool *buttons, unsigned *values)
{
	if (!buttons || !values) return false;
	memset(buttons, 0, (size_t)key_code::Unknown);
	memset(values, 0, 12);
	return true;
}

bool NullKeyboard::GetIsConnected() { return true; }

bool NullKeyboard::GetValueProperties(unsigned index, value_properties *properties)
{
	return false;
}

unsigned short NullKeyboard::GetUsagePage()
{
	return 1;
}

unsigned short NullKeyboard::GetUsage()
{
	return 6;
}

unsigned NullKeyboard::GetVendorID()
{
	return 0;
}

unsigned NullKeyboard::GetProductID()
{
	return 0;
}

unsigned NullKeyboard::GetVersionNumber()
{
	return 0;
}

const wchar_t *NullKeyboard::GetVendorName()
{
	return nullptr;
}

const wchar_t *NullKeyboard::GetProductName()
{
	return nullptr;
}

const wchar_t *NullKeyboard::GetSerialNumber()
{
	return nullptr;
}

const wchar_t *NullKeyboard::GetInterfaceName() { return L"Null Keyboard"; }


}}}