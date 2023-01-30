#pragma once
#include "IInput.h"
#include <Windows.h>

namespace TChapman500 {
namespace Input {
namespace Windows {

class WinMouse : public IInput
{
public:
	WinMouse();
	virtual ~WinMouse();

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


	// Call these functions in the window proc function
	void RawInputMessage(RAWMOUSE *data);
	void MouseMoveMessage(LPARAM lParam);

private:
	bool *_Buttons;
	int *_Values;

};

}}}
