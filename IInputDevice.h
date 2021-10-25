#pragma once


namespace TChapman500
{
	namespace JoystickAPI
	{
		struct valueProperties
		{
			unsigned short UsagePage;
			unsigned short Usage;
			unsigned short Bits;
			bool HasNullState;
			int MinValue;
			int MaxValue;
		};

		struct valueState
		{
			int RawValue;
			float CenterRelative;
			float EndRelative;
		};

		class IInputDevice
		{
		public:
			IInputDevice();
			virtual ~IInputDevice();

			void GetFloatValues(int min, int max, int raw, float *center, float *end);

			virtual unsigned short GetUsagePage() = 0;
			virtual unsigned short GetUsage() = 0;
			virtual unsigned short GetVendorID() = 0;
			virtual unsigned short GetProductID() = 0;

			virtual void GetVendorName(wchar_t *str) = 0;
			virtual void GetProduceName(wchar_t *str) = 0;
			virtual void GetSerialNumber(wchar_t *str) = 0;

			virtual void ReadInputStates(bool *bStates, valueState *vStates) = 0;

			virtual unsigned GetButtonCount() = 0;
			virtual unsigned GetValueCount() = 0;
			virtual void GetValueProperties(unsigned index, valueProperties *properties) = 0;

			virtual void GetInterfaceName(char *str) = 0;
		};
	}
}

