# DS3RuntimeScripting
DS3RuntimeScripting is a DLL you can compile for easy creation of mods for DARK SOULS III that interact with the game at runtime.
```cpp
static bool attach()
{
    ds3runtime_global.reset(new DS3RuntimeScripting);
    ds3runtime_global->addHook(std::make_shared<GameFrameHook>());
    ds3runtime_global->addHook(std::make_shared<LuaCapture>());
    ds3runtime_global->runScript(std::make_shared<HotkeyManager>());
    ds3runtime_global->addHook(std::make_shared<PlayAnimationHook>());
    ds3runtime_global->runScript(std::make_shared<AnimationIdHandler>());
    ds3runtime_global->runScript(std::make_shared<ParamPatcher>());
    ds3runtime_global->attach();
    return true;
}
```

## Hooks & Scripts
There is an interface to easily install hooks into the games memory image, or run scripts either on the game's main thread or asyncronously on a detached thread.
```cpp
#include "ds3runtime/script_module.h"

namespace ds3runtime {

class HotkeyManager : public ScriptModule
{
public:
	void onAttach();

	void execute();

	void onDetach();

	std::string getName()
	{
		return "hotkey_manager";
	}

	void registerKeyDownEvent(std::string eventName, std::function<void()> callback);

	void registerKeyUpEvent(std::string eventName, std::function<void()> callback);
private:
	HWND windowHandle;
	WNDPROC originalWindowProc = 0;
	std::unordered_map<std::string, std::function<void()>> keyDownEvents;
	std::unordered_map<std::string, std::function<void()>> keyUpEvents;

	static HotkeyManager* instance;

	static LRESULT hotkeyWindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
};

}
```

## Game Object Handle Classes
You can use these for accessing class objects, structs, arrays and lists contained in the game.
```cpp
uint32_t ChrIns::getHealth()
{
	return *accessMultilevelPointer<uint32_t>(address + 0x1F90, 0x18, 0xD8);
}

void ChrIns::setHealth(uint32_t health)
{
	*accessMultilevelPointer<uint32_t>(address + 0x1F90, 0x18, 0xD8) = health;
}
```
