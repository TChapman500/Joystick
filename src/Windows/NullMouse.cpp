#include "NullMouse.h"
#include <string.h>

namespace TChapman500 {
namespace Input {
namespace Windows {


NullMouse::NullMouse()
{
}

NullMouse::~NullMouse()
{
}

unsigned NullMouse::GetButtonCount()
{
	return 5;
}

unsigned NullMouse::GetValueCount()
{
	return 6;
}

bool NullMouse::GetInputState(bool *buttons, unsigned *values)
{
	if (!buttons || !values) return false;
	memset(buttons, 0, 5);
	memset(values, 0, 24);
	return true;
}

bool NullMouse::GetIsConnected()
{
	return false;
}

bool NullMouse::GetValueProperties(unsigned index, value_properties *properties)
{
	return false;
}

unsigned short NullMouse::GetUsagePage()
{
	return 1;
}

unsigned short NullMouse::GetUsage()
{
	return 2;
}

unsigned NullMouse::GetVendorID()
{
	return 0;
}

unsigned NullMouse::GetProductID()
{
	return 0;
}

unsigned NullMouse::GetVersionNumber()
{
	return 0;
}

const wchar_t *NullMouse::GetVendorName()
{
	return nullptr;
}

const wchar_t *NullMouse::GetProductName()
{
	return nullptr;
}

const wchar_t *NullMouse::GetSerialNumber()
{
	return nullptr;
}

const wchar_t *NullMouse::GetInterfaceName()
{
	return L"Null Mouse";
}


}}}