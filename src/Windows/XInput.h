#pragma once
#include "HID.h"
#include "IOutput.h"

namespace TChapman500 {
namespace Input {
namespace Windows {

class RawInputDevice;
class WinDevice;

class XInput : public IInput, IOutput
{
public:
	XInput(WinDevice *deviceInterface, RawInputDevice *riDevice);
	~XInput();

	bool SetRawInputDevice(RawInputDevice *riDevice);

	// Device Capabilities
	virtual unsigned GetButtonCount() override;
	virtual unsigned GetValueCount() override;

	// Get Device State
	virtual bool GetInputState(bool *buttons, unsigned *values) override;
	virtual bool GetIsConnected() override;

	virtual bool GetValueProperties(unsigned index, value_properties *properties) override;

	// Device Information
	virtual unsigned short GetUsagePage() override;
	virtual unsigned short GetUsage() override;
	virtual unsigned GetVendorID() override;
	virtual unsigned GetProductID() override;
	virtual unsigned GetVersionNumber() override;
	virtual const wchar_t *GetVendorName() override;
	virtual const wchar_t *GetProductName() override;
	virtual const wchar_t *GetSerialNumber() override;
	virtual const wchar_t *GetInterfaceName() override;

private:
	WinDevice *_DeviceInterface = nullptr;
	RawInputDevice *_RawInputDevice = nullptr;

	// Device Handle
	LPWSTR DevicePath = nullptr;
	HANDLE FileHandle = nullptr;

};

}}}

