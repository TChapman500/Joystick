#include "HID.h"
#include <hidsdi.h>
#include <hidpi.h>

namespace TChapman500 {
namespace Input {
namespace Windows {

HID::HID(LPWSTR devicePath)
{
	// Initialize Variables
	ZeroMemory(&OverlappedResult, sizeof(OVERLAPPED));

	DevicePath = devicePath;

	size_t pathLength = wcslen(devicePath) + 1;
	DevicePath = new wchar_t[pathLength];
	wcscpy_s(DevicePath, pathLength, devicePath);


	FileHandle = nullptr;
	Preparsed = nullptr;
	ZeroMemory(&Capabilities, sizeof(HIDP_CAPS));

	// Open the file
	if (!OpenFile()) return;

	if (!HidD_GetPreparsedData(FileHandle, &Preparsed))
	{
		CloseHandle(FileHandle);
		FileHandle = nullptr;
		return;
	}

	_DataIndicesSize = HidP_MaxDataListLength(HidP_Input, Preparsed);
	_HidData = new HIDP_DATA[_DataIndicesSize];
	_DataIndices = new data_index_info[_DataIndicesSize];

	// Device Capabilities
	NTSTATUS ntResult = HidP_GetCaps(Preparsed, &Capabilities);
	if (ntResult != HIDP_STATUS_SUCCESS)
	{
		CloseHandle(FileHandle);
		FileHandle = nullptr;
		HidD_FreePreparsedData(Preparsed);
		Preparsed = nullptr;
		return;
	}
	_ValuesSize = Capabilities.NumberInputValueCaps;
	unsigned short valueCount = (unsigned short)_ValuesSize;

	bool error = false;

	// Initialize Report

	_ReportLength = Capabilities.InputReportByteLength;
	_Report = new char[_ReportLength];

	// Get button capabilities
	HIDP_BUTTON_CAPS *buttonCaps = new HIDP_BUTTON_CAPS[Capabilities.NumberInputButtonCaps];
	ntResult = HidP_GetButtonCaps(HidP_Input, buttonCaps, &Capabilities.NumberInputButtonCaps, Preparsed);
	if (ntResult == HIDP_STATUS_SUCCESS)
	{
		_ButtonStatesSize = (size_t)buttonCaps->Range.UsageMax - (size_t)buttonCaps->Range.UsageMin + 1;
		_ButtonStates = new USAGE[_ButtonStatesSize];
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
	_Values = new HIDP_VALUE_CAPS[Capabilities.NumberInputValueCaps];
	ntResult = HidP_GetValueCaps(HidP_Input, _Values, &valueCount, Preparsed);
	if (ntResult == HIDP_STATUS_SUCCESS)
	{
		// Populate value information
		for (unsigned i = 0; i < Capabilities.NumberInputValueCaps; i++)
		{
			_DataIndices[_Values[i].NotRange.DataIndex].IsButton = false;
			_DataIndices[_Values[i].NotRange.DataIndex].ControlIndex = i;	// Control index is an index on the value state list
		}
	}
	else error = true;

	if (error)
	{
		CloseHandle(FileHandle);
		FileHandle = nullptr;
		HidD_FreePreparsedData(Preparsed);
		Preparsed = nullptr;
	}

	_HIDD_ATTRIBUTES devInfo;
	if (GetDeviceInfo(&devInfo))
	{
		VendorID = devInfo.VendorID;
		ProductID = devInfo.ProductID;
		VersionNumber = devInfo.VersionNumber;
	}
}

HID::~HID()
{
	delete[] DevicePath;
	delete[] _Report;
	delete[] _ButtonStates;
	delete[] _HidData;
	delete[] _DataIndices;
	delete[] _Values;

	if (FileHandle) CloseHandle(FileHandle);
	if (Preparsed) HidD_FreePreparsedData(Preparsed);
}

unsigned HID::GetButtonCount() { return (unsigned)_ButtonStatesSize; }
unsigned HID::GetValueCount() { return (unsigned)_ValuesSize; }

#define OLD_SCHOOL_REPORT_GET

bool HID::GetInputState(bool *buttons, unsigned *values)
{
	// Invalid function parameters or report handle
	if (!buttons || !values) return false;

	// This is just to make sure we can get data from the device.
	if (!OpenFile()) return false;

	// Fetch input report
	unsigned bytesRead = 0;
	ZeroMemory(&OverlappedResult, sizeof(OVERLAPPED));
	ReadFile(FileHandle, _Report, (unsigned)_ReportLength, (DWORD *)&bytesRead, &OverlappedResult);

#ifdef OLD_SCHOOL_REPORT_GET
	// Get pressed buttons
	unsigned long outButtonCount = (unsigned long)_ButtonStatesSize;
	NTSTATUS ntResult = HidP_GetUsages(HidP_Input, HID_USAGE_PAGE_BUTTON, 0, _ButtonStates, &outButtonCount, Preparsed, _Report, (unsigned)_ReportLength);
	if (ntResult != HIDP_STATUS_SUCCESS) return false;
#endif

	// Clear button states
	for (size_t i = 0; i < _ButtonStatesSize; i++)
		buttons[i] = false;

	// Clear value states
	for (size_t i = 0; i < _ValuesSize; i++)
		values[i] = 0;

#ifndef OLD_SCHOOL_REPORT_GET
	// This is a bit redundant of a read, but it works.
	unsigned long outputCount = HidData.size();
	NTSTATUS ntResult = HidP_GetData(HidP_Input, HidData.data(), &outputCount, Preparsed, _Report, _ReportLength);
	if (ntResult != HIDP_STATUS_SUCCESS)
		return false;

	// Get pressed buttons and values
	for (size_t i = 0, d = HidData[i].DataIndex; i < outputCount; i++)
	{
		// Data at this control index is a button.
		if (DataIndices[d].IsButton)
			buttons[DataIndices[d].ControlIndex] = true;
		// Data at this control index is a value.
		else values[DataIndices[d].ControlIndex] = HidData[i].RawValue;
	}
#endif

#ifdef OLD_SCHOOL_REPORT_GET
	// Set pressed buttons
	for (unsigned long i = 0; i < outButtonCount; i++)
	{
		if (_ButtonStates[i] > 0)
			buttons[_ButtonStates[i] - _FirstButtonIndex] = true;
	}

	// Get values
	for (unsigned i = 0; i < _ValuesSize; i++)
	{
		// Attempt to get values.
		ntResult = HidP_GetUsageValue(HidP_Input, _Values[i].UsagePage, 0, _Values[i].NotRange.Usage, (ULONG *)&values[i], Preparsed, _Report, (unsigned)_ReportLength);
		if (ntResult != HIDP_STATUS_SUCCESS) return false;
	}
#endif

	// Input state retrieved successfully
	return true;
}

bool HID::GetIsConnected()
{
	// Can't open the file.
	if (!OpenFile()) return false;

	// File is opened.
	unsigned bytesRead = 0;
	BOOL result = ReadFile(FileHandle, _Report, (unsigned)_ReportLength, (DWORD *)&bytesRead, &OverlappedResult);

	// File read still failed.
	if (!result) return false;
	// File read succeeded.
	return true;
}

bool HID::GetValueProperties(unsigned index, value_properties *properties)
{
	// Invalid Parameters
	if (index >= _ValuesSize || !properties) return false;

	// Copy properties for selected input value.
	properties->UsagePage = _Values[index].UsagePage;
	properties->Usage = _Values[index].NotRange.Usage;
	properties->Bits = _Values[index].BitSize;
	properties->HasNullState = _Values[index].HasNull;
	properties->MinValue = _Values[index].LogicalMin;
	properties->MaxValue = _Values[index].LogicalMax;
	return true;
}

unsigned short HID::GetUsagePage() { return Capabilities.UsagePage; }
unsigned short HID::GetUsage() { return Capabilities.Usage; }
unsigned HID::GetVendorID() { return VendorID; }
unsigned HID::GetProductID() { return ProductID; }
unsigned HID::GetVersionNumber() { return VersionNumber; }

const wchar_t *HID::GetVendorName()
{
	if (OpenFile())
	{
		// Allocate space for the buffer
		wchar_t *result = new wchar_t[126];

		// Get manufacturer name
		BOOLEAN getResult = HidD_GetManufacturerString(FileHandle, result, sizeof(wchar_t) * 126);
		if (!getResult)
		{
			delete[] result;
			return nullptr;
		}
		return result;
	}
	return nullptr;
}

const wchar_t *HID::GetProductName()
{
	if (OpenFile())
	{
		// Allocate space for the buffer
		wchar_t *result = new wchar_t[126];

		// Get product name
		BOOLEAN getResult = HidD_GetProductString(FileHandle, result, sizeof(wchar_t) * 126);
		if (!getResult)
		{
			delete[] result;
			return nullptr;
		}
		return result;
	}
	return nullptr;
}

const wchar_t *HID::GetInterfaceName() { return L"HID"; }

bool HID::OpenFile()
{
	// No need to do anything else.
	if (FileHandle && FileHandle != INVALID_HANDLE_VALUE) return true;

	// Open the file.
	FileHandle = CreateFileW(DevicePath, GENERIC_READ | GENERIC_WRITE, FILE_SHARE_READ | FILE_SHARE_WRITE, nullptr, OPEN_EXISTING, FILE_FLAG_OVERLAPPED, nullptr);

	// Attempt Failed
	if (FileHandle == INVALID_HANDLE_VALUE)
	{
		FileHandle = nullptr;
		return false;
	}
	// Attempt Successful
	return true;
}

bool HID::GetDeviceInfo(_HIDD_ATTRIBUTES *devInfo)
{
	if (!devInfo || !OpenFile()) return false;
	if (HidD_GetAttributes(FileHandle, devInfo)) return true;
	return false;
}

}}}