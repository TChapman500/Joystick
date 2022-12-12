#pragma once
#include "HID.h"
#include "IOutput.h"

namespace TChapman500 {
namespace Input {
namespace Windows {

class XInput : public HID, IOutput
{
public:
	XInput(LPWSTR hidFile, LPWSTR xInputFile);
	~XInput();

	// Device Capabilities
	virtual unsigned GetButtonCount() override;
	virtual unsigned GetValueCount() override;

	// Get Device State
	virtual bool GetInputState(bool *buttons, unsigned *values) override;
	virtual bool GetIsConnected() override;

	virtual bool GetValueProperties(unsigned index, value_properties *properties) override;

	// Device Information
	virtual const wchar_t *GetInterfaceName() override;

private:
	// Device Handle
	LPWSTR DevicePath;
	HANDLE FileHandle;

};

}}}

