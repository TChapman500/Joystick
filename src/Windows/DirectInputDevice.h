#pragma once
#include "WinInputDevice.h"
#include <hidsdi.h>
#include <dinput.h>
#include <vector>

namespace TChapman500
{
	namespace JoystickAPI
	{
		class DirectInputDevice : public WinInputDevice
		{
		public:
			DirectInputDevice(IDirectInputDevice8A *device, HANDLE devHandle);
			~DirectInputDevice();

			virtual void ReadInputStates(bool *bStates, valueState *vStates) override;

			virtual unsigned GetButtonCount() override;
			virtual unsigned GetValueCount() override;
			virtual void GetValueProperties(unsigned index, valueProperties *properties) override;


			virtual void GetInterfaceName(char *str) override;

		private:
			IDirectInputDevice8A *Device;	// Initialized
			HIDP_CAPS Capabilities;		// Initialized
			DIDATAFORMAT ReportFormat;
			_HIDP_PREPARSED_DATA *Preparsed;
			int ReportSize;
			char *ReportData;

			unsigned short BtnUsagePage;
			unsigned short BtnUsageMin;
			unsigned short BtnUsageMax;
			unsigned ButtonCount;
			unsigned OutButtonCount;

			struct dInputObject
			{

				DIDEVICEOBJECTINSTANCEA ControlInstance;
				DIPROPRANGE ControlRange;
			};
			std::vector<dInputObject *> ForceFeedbackList;

			std::vector<valueProperties> ValueProperties;

			OVERLAPPED OverlappedResult;
			USAGE *ButtonStates;

			static BOOL DIEnumFFBObjectsCallback(LPCDIDEVICEOBJECTINSTANCEA lpddoi, LPVOID pvRef);
		};
	}
}
