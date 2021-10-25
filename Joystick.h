#pragma once
#include "IInputDevice.h"
#include <vector>

namespace TChapman500
{
	namespace JoystickAPI
	{
		class IInputDevice;

		class InputAxis;
		class InputButton;
		class InputHAT;

		class Joystick
		{
		public:
			Joystick(IInputDevice *devInterface);
			virtual ~Joystick();

			virtual void ReadJoystickState();
			void AddCustomHAT(InputButton *upButton, InputButton *downButton, InputButton *rightButton, InputButton *leftButton);
			void GetInterfaceName(char *str);

			unsigned short GetUsagePage();
			unsigned short GetUsage();
			unsigned short GetVendorID();
			unsigned short GetProductID();

			void GetVendorName(wchar_t *str);
			void GetProduceName(wchar_t *str);
			void GetSerialNumber(wchar_t *str);


			std::vector<InputAxis *> AxisList;
			std::vector<InputButton *> ButtonList;
			std::vector<InputHAT *> HATList;
			
			std::vector<valueProperties> ValuePropertyList;
		private:
			unsigned _ValueCount;
			unsigned _RealButtonCount;	// Number of buttons on the device
			unsigned _RealHATCount = 0;	// Number of HAT switches on the device

			IInputDevice *DeviceInterface;

			void _SetHatState(unsigned index, void *state);
			void _SetAxisState(unsigned index, void *state);

			struct valueStateFunctions
			{
				unsigned Index;
				void (Joystick:: *Function)(unsigned, void *);
			};
			std::vector<valueStateFunctions> _SetValueFunctions;

			valueState *ValueStates;
			bool *ButtonStates;

			unsigned short UsagePage;
			unsigned short Usage;
			unsigned short VendorID;
			unsigned short ProductID;

			wchar_t VendorName[126];
			wchar_t ProductName[126];
			wchar_t SerialNumber[126];
		};
	}
}
