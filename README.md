# Joystick
This is supposed to be a cross-platform Joystick library, but for now, it's just for windows.  Forks are pull requests to improve this library are welcome.  I have made a simple [Joystick Viewer](https://github.com/TChapman500/WindowsJoystick) for testing this library.  Just don't hit the refresh button.

## Use in Projects
For each platform, you should only need to download the platform-specific static libraries as well as the platform-independent header files.

### Extending `Joystick`
The `Joystick` class may be extended to implement device-specific functions such as multiple HAT switches or custom output effects.  No platform-specific code may be used in any class derived from `Joystick`.

## Extending the Library
The `IInputSystem` is designed to be a platform-independent class from which platform-specific interfaces can be derived.  `IInput` and `IOutput` are platform-independent base classes for platform-specific interfaces to be derived from.  `IInput` is for getting user input from the device while `IOutput` is for force feedback effects.
