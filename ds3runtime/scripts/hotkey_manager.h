/*
	* DS3RuntimeScripting
	* Contributers: Amir
*/

#pragma once
#include "pch.h"
#include "ds3runtime/script_module.h"

namespace ds3runtime {

class HotkeyManager : public ScriptModule
{
public:
	bool onAttach();

	void execute();

	bool onDetach();

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