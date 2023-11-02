#pragma once
#include "IInput.h"
#include <Windows.h>
#include <hidsdi.h>
#include <vector>

namespace TChapman500 {
namespace Input {
namespace Windows {

class WinDevice;
class RawInputDevice : public IInput
{
public:
	RawInputDevice(WinDevice *devInterface);
	~RawInputDevice();

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

	void SaveReport(char *report, size_t length);

protected:
	bool GetDeviceInfo(_HIDD_ATTRIBUTES *devInfo);

private:

	wchar_t *_VendorName = nullptr;
	wchar_t *_ProductName = nullptr;
	wchar_t *_InterfaceName = nullptr;
	wchar_t *_SerialNumber = nullptr;

	WinDevice *_DeviceInterface;

	_HIDP_PREPARSED_DATA *_Preparsed = nullptr;
	HIDP_CAPS _Capabilities;

	// HID Reports
	char *_Report = nullptr;
	size_t _ReportLength;

	// Buttons
	std::vector<USAGE> _ButtonStates;
	size_t _FirstButtonIndex;

	// General Info
	unsigned _VendorID;
	unsigned _ProductID;
	unsigned _VersionNumber;

	struct data_index_info
	{
		bool IsButton;
		unsigned ControlIndex;
	};
	std::vector<data_index_info> _DataIndices;
	std::vector<HIDP_DATA> _HidData;
	std::vector<HIDP_VALUE_CAPS> _Values;
};

}}}