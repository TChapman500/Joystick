#pragma once
#include "IInput.h"
#include <Windows.h>


namespace TChapman500 {
namespace Input {
namespace Windows {

class WinKeyboard : public IInput
{
public:
	WinKeyboard();
	virtual ~WinKeyboard();

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


	// Call these functions in the window proc function
	void RawInputMessage(RAWKEYBOARD *data);


private:
	// Modified inside message proc, sent when GetInputState() is called.
	bool *_Keys;
	unsigned *_Values;


};

}}}