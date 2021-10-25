#pragma once
#include "WinInputDevice.h"
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
			DIDEVCAPS Capabilities;		// Initialized
			DIDATAFORMAT ReportFormat;
			int ReportSize;
			char *ReportData;


			struct dInputObject
			{
				DIDEVICEOBJECTINSTANCEA ControlInstance;
				DIPROPRANGE ControlRange;
			};
			std::vector<dInputObject *> AxisList;
			std::vector<dInputObject *> ButtonList;
			std::vector<dInputObject *> ForceFeedbackList;

			std::vector<valueProperties> ValueProperties;



			static BOOL DIEnumValueObjectsCallback(LPCDIDEVICEOBJECTINSTANCEA lpddoi, LPVOID pvRef);
			static BOOL DIEnumButtonObjectsCallback(LPCDIDEVICEOBJECTINSTANCEA lpddoi, LPVOID pvRef);
			static BOOL DIEnumFFBObjectsCallback(LPCDIDEVICEOBJECTINSTANCEA lpddoi, LPVOID pvRef);
		};
	}
}
