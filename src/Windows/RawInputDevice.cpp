#include "RawInputDevice.h"
#include "WinDevice.h"
#include <new>

namespace TChapman500 {
namespace Input {
namespace Windows {

RawInputDevice::RawInputDevice(WinDevice *devInterface)
{
	memset(&_Capabilities, 0, sizeof(HIDP_CAPS));

	_DeviceInterface = devInterface;
	if (!devInterface) return;

	HANDLE fileHandle = _DeviceInterface->GetFileHandle();
	if (!fileHandle || !HidD_GetPreparsedData(fileHandle, &_Preparsed)) return;

	size_t dataIndicesSize = HidP_MaxDataListLength(HidP_Input, _Preparsed);
	_HidData.resize(dataIndicesSize);
	_DataIndices.resize(dataIndicesSize);

	// Device Capabilities
	NTSTATUS ntResult = HidP_GetCaps(_Preparsed, &_Capabilities);
	if (ntResult != HIDP_STATUS_SUCCESS)
	{
		HidD_FreePreparsedData(_Preparsed);
		_Preparsed = nullptr;
		return;
	}

	_ReportLength = _Capabilities.InputReportByteLength;
	_Report = new(std::nothrow) char[_ReportLength];

	bool error = false;

	// Get button capabilities
	HIDP_BUTTON_CAPS *buttonCaps = new HIDP_BUTTON_CAPS[_Capabilities.NumberInputButtonCaps];
	ntResult = HidP_GetButtonCaps(HidP_Input, buttonCaps, &_Capabilities.NumberInputButtonCaps, _Preparsed);
	if (ntResult == HIDP_STATUS_SUCCESS)
	{
		size_t buttonStateSize = (size_t)buttonCaps->Range.UsageMax - (size_t)buttonCaps->Range.UsageMin + 1;
		_ButtonStates.resize(buttonStateSize);
		_FirstButtonIndex = buttonCaps->Range.UsageMin;

		// Populate button data indices
		for (unsigned b = 0, i = buttonCaps->Range.DataIndexMin; i <= buttonCaps->Range.DataIndexMax; b++, i++)
		{
			_DataIndices[i].IsButton = true;
			_DataIndices[i].ControlIndex = b;	// Control index is an index on the button state list
		}
	}
	else error = true;
	delete[] buttonCaps;

	// Get value capabilities
	_Values.resize(_Capabilities.NumberInputValueCaps);
	ntResult = HidP_GetValueCaps(HidP_Input, _Values.data(), &_Capabilities.NumberInputValueCaps, _Preparsed);
	if (ntResult == HIDP_STATUS_SUCCESS)
	{
		// Populate value information
		for (unsigned i = 0; i < _Capabilities.NumberInputValueCaps; i++)
		{
			_DataIndices[_Values[i].NotRange.DataIndex].IsButton = false;
			_DataIndices[_Values[i].NotRange.DataIndex].ControlIndex = i;	// Control index is an index on the value state list
		}
	}
	else error = true;

	if (error)
	{
		HidD_FreePreparsedData(_Preparsed);
		_Preparsed = nullptr;
	}

	_HIDD_ATTRIBUTES devInfo;
	if (GetDeviceInfo(&devInfo))
	{
		_VendorID = devInfo.VendorID;
		_ProductID = devInfo.ProductID;
		_VersionNumber = devInfo.VersionNumber;
	}


	wchar_t *strResults = new(std::nothrow) wchar_t[126];
	if (strResults)
	{
		memset(strResults, 0, sizeof(wchar_t) * 126);
		BOOLEAN getResult = HidD_GetManufacturerString(fileHandle, strResults, sizeof(wchar_t) * 126);
		if (getResult)
		{
			_VendorName = new(std::nothrow) wchar_t[126];
			if (_VendorName) wcscpy_s(_VendorName, 126, strResults);
		}

		memset(strResults, 0, sizeof(wchar_t) * 126);
		getResult = HidD_GetProductString(fileHandle, strResults, sizeof(wchar_t) * 126);
		if (getResult)
		{
			_ProductName = new(std::nothrow) wchar_t[126];
			if (_ProductName) wcscpy_s(_ProductName, 126, strResults);
		}

		memset(strResults, 0, sizeof(wchar_t) * 126);
		getResult = HidD_GetSerialNumberString(fileHandle, strResults, sizeof(wchar_t) * 126);
		if (getResult)
		{
			_SerialNumber = new(std::nothrow) wchar_t[126];
			if (_SerialNumber) wcscpy_s(_SerialNumber, 126, strResults);
		}

		delete[] strResults;
	}
}

RawInputDevice::~RawInputDevice()
{
	if (_Preparsed) HidD_FreePreparsedData(_Preparsed);
	if (_Report) delete[] _Report;
	if (_VendorName) delete[] _VendorName;
	if (_ProductName) delete[] _ProductName;
	if (_SerialNumber) delete[] _SerialNumber;
}

unsigned RawInputDevice::GetButtonCount() { return _ButtonStates.size(); }

unsigned RawInputDevice::GetValueCount() { return _Values.size(); }

bool RawInputDevice::GetInputState(bool *buttons, unsigned *values)
{
	if (!buttons || !values) return false;

	// Clear button states
	for (size_t i = 0; i < _ButtonStates.size(); i++)
		buttons[i] = false;

	// Clear value states
	for (size_t i = 0; i < _Values.size(); i++)
		values[i] = 0;

	// This is a bit redundant of a read, but it works.
	unsigned long outputCount = (unsigned long)_DataIndices.size();
	NTSTATUS ntResult = HidP_GetData(HidP_Input, _HidData.data(), &outputCount, _Preparsed, _Report, (unsigned long)_ReportLength);
	if (ntResult != HIDP_STATUS_SUCCESS)
		return false;

	// Get pressed buttons and values
	for (size_t i = 0; i < outputCount; i++)
	{
		// Get the data index for this control.
		size_t d = _HidData[i].DataIndex;

		// Data at this control index is a button.
		if (_DataIndices[d].IsButton)
			buttons[_DataIndices[d].ControlIndex] = true;
		// Data at this control index is a value.
		else values[_DataIndices[d].ControlIndex] = _HidData[i].RawValue;
	}

	return true;
}

bool RawInputDevice::GetIsConnected() { return _DeviceInterface->GetFileHandle() != nullptr; }

bool RawInputDevice::GetValueProperties(unsigned index, value_properties *properties)
{
	// Invalid Parameters
	if (index >= _Values.size() || !properties) return false;

	// Copy properties for selected input value.
	properties->UsagePage = _Values[index].UsagePage;
	properties->Usage = _Values[index].NotRange.Usage;
	properties->Bits = _Values[index].BitSize;
	properties->HasNullState = _Values[index].HasNull;
	properties->MinValue = _Values[index].LogicalMin;
	properties->MaxValue = _Values[index].LogicalMax;
	return true;
}

unsigned short RawInputDevice::GetUsagePage() { return _Capabilities.UsagePage; }
unsigned short RawInputDevice::GetUsage() { return _Capabilities.Usage; }
unsigned RawInputDevice::GetVendorID() { return _VendorID; }
unsigned RawInputDevice::GetProductID() { return _ProductID; }
unsigned RawInputDevice::GetVersionNumber() { return _VersionNumber; }

const wchar_t *RawInputDevice::GetVendorName() { return _VendorName; }
const wchar_t *RawInputDevice::GetProductName() { return _ProductName; }
const wchar_t *RawInputDevice::GetSerialNumber() { return _SerialNumber; }
const wchar_t *RawInputDevice::GetInterfaceName() { return L"RawInput"; }

void RawInputDevice::SaveReport(char *report, size_t length)
{
	if (length != _ReportLength) return;
	memcpy_s(_Report, _ReportLength, report, length);
}

bool RawInputDevice::GetDeviceInfo(_HIDD_ATTRIBUTES *devInfo)
{
	if (!devInfo || !_DeviceInterface->GetFileHandle()) return false;
	if (HidD_GetAttributes(_DeviceInterface->GetFileHandle(), devInfo)) return true;
	return false;
}


}}}