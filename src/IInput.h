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

class IInput
{
public:
	IInput();
	virtual ~IInput();

	// Device Capabilities
	virtual unsigned GetButtonCount() = 0;
	virtual unsigned GetValueCount() = 0;

	// Get Device State
	virtual bool GetInputState(bool *buttons, unsigned *values) = 0;
	virtual bool GetIsConnected() = 0;

	virtual bool GetValueProperties(unsigned index, value_properties *properties) = 0;

	// Device Information
	virtual unsigned short GetUsagePage() = 0;
	virtual unsigned short GetUsage() = 0;
	virtual unsigned GetVendorID() = 0;
	virtual unsigned GetProductID() = 0;
	virtual unsigned GetVersionNumber() = 0;
	virtual const wchar_t *GetVendorName() = 0;
	virtual const wchar_t *GetProductName() = 0;
	virtual const wchar_t *GetInterfaceName() = 0;
};

}}
