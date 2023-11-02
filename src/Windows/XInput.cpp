#include "XInput.h"
#include "WinDevice.h"
#include "RawInputDevice.h"

namespace TChapman500 {
namespace Input {
namespace Windows {

XInput::XInput(WinDevice *deviceInterface, RawInputDevice *riDevice)
{
	_DeviceInterface = deviceInterface;
	_RawInputDevice = riDevice;
}

XInput::~XInput()
{
}

bool XInput::SetRawInputDevice(RawInputDevice *riDevice)
{
	if (_RawInputDevice || !riDevice) return false;
	_RawInputDevice = riDevice;
	return true;
}

unsigned XInput::GetButtonCount() { return 10; }

unsigned XInput::GetValueCount() { return 7; }

bool XInput::GetInputState(bool *buttons, unsigned *values)
{
	if (!buttons || !values) return false;


	BYTE in[3] = { 0x01, 0x01, 0x00 };
	BYTE out[29];
	DWORD size;
	HANDLE fileHandle = _DeviceInterface->GetFileHandle();
	if (!fileHandle)
	{
		for (int i = 0; i < 10; i++) buttons[i] = false;
		values[0] = 8;
		for (int i = 1; i < 7; i++) values[i] = 0;
		return false;
	}

	if (!DeviceIoControl(fileHandle, 0x8000e00c, in, sizeof(in), out, sizeof(out), &size, NULL) || size != sizeof(out)) return false;
	
	unsigned xiButtons = *(DWORD *)&out[11];

	// Up
	if (xiButtons & 1)
	{
		if (xiButtons & 4) values[0] = 7;	// And Left
		else if (xiButtons & 8) values[0] = 1;	// And Right
		else values[0] = 0;
	}
	// Down
	else if (xiButtons & 2)
	{
		if (xiButtons & 4) values[0] = 5;	// And Left
		else if (xiButtons & 8) values[0] = 3;	// And Right
		else values[0] = 4;
	}
	// Left
	else if (xiButtons & 4) values[0] = 6;
	// Right
	else if (xiButtons & 8) values[0] = 2;
	// Centered
	else values[0] = 8;

	buttons[0] = (xiButtons & 0x10) >= 1;
	buttons[1] = (xiButtons & 0x20) >= 1;
	buttons[2] = (xiButtons & 0x40) >= 1;
	buttons[3] = (xiButtons & 0x80) >= 1;
	buttons[4] = (xiButtons & 0x100) >= 1;
	buttons[5] = (xiButtons & 0x200) >= 1;
	buttons[6] = (xiButtons & 0x1000) >= 1;
	buttons[7] = (xiButtons & 0x2000) >= 1;
	buttons[8] = (xiButtons & 0x4000) >= 1;
	buttons[9] = (xiButtons & 0x8000) >= 1;

	for (int i = 1; i < 7; i++) values[i] = 0;

	// X
	unsigned short value = *(unsigned short *)&out[15];
	value += 32768;
	values[1] = value & 255;

	// Y
	value = *(unsigned short *)&out[17];
	value += 32768;
	values[2] = value & 255;

	// Z
	value = out[13];
	values[3] = value;

	// RX
	value = *(unsigned short *)&out[19];
	value += 32768;
	values[4] = value & 255;

	// RY
	value = *(unsigned short *)&out[21];
	value += 32768;
	values[5] = value & 255;

	// RZ
	value = out[14];
	values[6] = value;


	return true;
}

bool XInput::GetIsConnected() { return _DeviceInterface->GetFileHandle() != nullptr; }

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
		properties->Usage = (index - 1) + 0x30;
		properties->Bits = 8;
		properties->HasNullState = false;
		properties->MinValue = 0;
		properties->MaxValue = 255;
	}

	// Successful Data Retrieval
	return true;
}

unsigned short XInput::GetUsagePage() { return 1; }
unsigned short XInput::GetUsage() { return 5; }
unsigned XInput::GetVendorID() { return _RawInputDevice->GetVendorID(); }
unsigned XInput::GetProductID() { return _RawInputDevice->GetProductID(); }
unsigned XInput::GetVersionNumber() { return _RawInputDevice->GetVersionNumber(); }
const wchar_t *XInput::GetVendorName() { return _RawInputDevice->GetVendorName(); }
const wchar_t *XInput::GetProductName() { return _RawInputDevice->GetProductName(); }
const wchar_t *XInput::GetSerialNumber() { return _RawInputDevice->GetSerialNumber(); }
const wchar_t *XInput::GetInterfaceName() { return L"XInput"; }

}}}