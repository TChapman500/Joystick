#pragma once

namespace TChapman500
{
	namespace JoystickAPI
	{
		class IInputSystem;

		IInputSystem *CreateInputSystem();
		void DestroyInputSystem(IInputSystem *inputSystem);
	}
}



