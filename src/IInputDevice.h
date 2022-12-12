#pragma once


namespace TChapman500
{
	namespace Input
	{
		struct value_properties
		{
			unsigned short UsagePage;
			unsigned short Usage;
			unsigned short Bits;
			bool HasNullState;
			int MinValue;
			int MaxValue;
		};

		struct value_state
		{
			int RawValue;
			float CenterRelative;
			float EndRelative;
		};

		class IInputDevice
		{
		public:
			static void GetFloatValues(int min, int max, int raw, float *center, float *end);

			virtual unsigned short GetUsagePage() = 0;
			virtual unsigned short GetUsage() = 0;
			virtual unsigned short GetVendorID() = 0;
			virtual unsigned short GetProductID() = 0;

			virtual void GetVendorName(wchar_t *str) = 0;
			virtual void GetProductName(wchar_t *str) = 0;
			virtual void GetSerialNumber(wchar_t *str) = 0;

			virtual void ReadInputStates(bool *bStates, value_state *vStates) = 0;

			virtual unsigned GetButtonCount() = 0;
			virtual unsigned GetValueCount() = 0;
			virtual void GetValueProperties(unsigned index, value_properties *properties) = 0;

			virtual void GetInterfaceName(char *str) = 0;
		};
	}
}

