#pragma once
#include "IInput.h"
#include <Windows.h>
#include <hidsdi.h>
#include <string>

namespace TChapman500 {
namespace Input {
namespace Windows {

class HID : public IInput
{
public:
	HID(LPWSTR devicePath);
	~HID();

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
	virtual const wchar_t *GetInterfaceName() override;

protected:
	// Helper Functions
	bool OpenFile();
	bool GetDeviceInfo(_HIDD_ATTRIBUTES *devInfo);

private:

	// Device Handle
	wchar_t *DevicePath;
	HANDLE FileHandle;
	OVERLAPPED OverlappedResult;

	_HIDP_PREPARSED_DATA *Preparsed;
	HIDP_CAPS Capabilities;

	// HID Reports
	char *_Report;
	size_t _ReportLength;

	// Buttons
	USAGE *_ButtonStates;
	size_t _ButtonStatesSize;
	unsigned _FirstButtonIndex;


	// Capabilities

	// General Info
	unsigned VendorID;
	unsigned ProductID;
	unsigned VersionNumber;

	struct data_index_info
	{
		bool IsButton;
		unsigned ControlIndex;
	};
	data_index_info *_DataIndices;
	HIDP_DATA *_HidData;
	size_t _DataIndicesSize;

	HIDP_VALUE_CAPS *_Values;
	size_t _ValuesSize;
};

}}}

