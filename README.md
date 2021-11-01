# Joystick
This is supposed to be a cross-platform Joystick library, but for now, it's just for windows.  Forks are pull requests to improve this library are welcome.  I have made a simple [Joystick Viewer](https://github.com/TChapman500/WindowsJoystick) for testing this library.  Just don't hit the refresh button.

## Use in Projects
For each platform, you should only need to download the platform-specific static library, and the `InputSystem.h`, `IInputSystem.h`, `Joystick.h`, `InputButton.h`, `InputAxis.h`, and `InputHAT.h` files.  To initialize the input system, call `TChapman500::JoystickAPI::CreateInputSystem()`, and then call `TChapman500::JoystickAPI::DestroyInputSystem()` to deallocate the memory.

### Extending `Joystick`
The `Joystick` class is the only class that is designed to be extended by applications.  Because of the way the internals of the library are designed, no platform-specific code should be required in your derived class.

## Extending the Library
The `IInputSystem` is designed to be a platform-neutral class from which platform-specific interfaces can be derived.  Do not add any platform-specific headers to `InputSystem.h`.  Platform-specific initialization code goes into `InputSystem.cpp` inside `CreateInputSystem()` using preprocessor directives specific to your platform of choice.

For the platform of your choice, create one `IInputDevice`-derived class per available API, based on the capabilities of the available APIs.  These interface classes should remain fully-compatible with the base `Joystick` class.
