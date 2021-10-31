#include "InputSystem.h"

#ifdef _WIN32
#include "WinInputSystem.h"
#endif

namespace TChapman500
{
	namespace JoystickAPI
	{
		IInputSystem *CreateInputSystem()
		{
			IInputSystem *result = nullptr;

#ifdef _WIN32
			result = new WinInputSystem();
#endif


			return result;
		}
		void DestroyInputSystem(IInputSystem *inputSystem)
		{
			delete inputSystem;
		}
	}
}

