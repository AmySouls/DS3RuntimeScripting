#pragma once
#include "pch.h"
#include "hotkey_manager.h"
#include "ds3runtime/ds3runtime.h"

namespace ds3runtime {

bool HotkeyManager::onAttach()
{
	return true;
}

bool HotkeyManager::onDetach()
{
	if (originalWindowProc) SetWindowLongPtrW(windowHandle, GWLP_WNDPROC, (LONG_PTR)originalWindowProc);
	return true;
}

void HotkeyManager::execute()
{
	if (originalWindowProc == 0) {
		windowHandle = FindWindowW(0, L"DARK SOULS III");
		DWORD processId;

		if (windowHandle
			&& GetWindowThreadProcessId(windowHandle, &processId)
			&& processId == GetCurrentProcessId() && GetWindowLongPtrW(windowHandle, GWLP_WNDPROC) != (LONG_PTR)hotkeyWindowProc) {
			auto previousWindowProc = (WNDPROC)SetWindowLongPtrW(windowHandle, GWLP_WNDPROC, (LONG_PTR)hotkeyWindowProc);
			originalWindowProc = previousWindowProc;
			instance = this;
		}
	}
}

void HotkeyManager::registerKeyDownEvent(std::string eventName, std::function<void()> callback)
{
	keyDownEvents[eventName] = callback;
}

void HotkeyManager::registerKeyUpEvent(std::string eventName, std::function<void()> callback)
{
	keyUpEvents[eventName] = callback;
}

HotkeyManager* HotkeyManager::instance = nullptr;

LRESULT HotkeyManager::hotkeyWindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg) {
	case WM_KEYDOWN:
		for (auto event : instance->keyDownEvents) event.second();
		break;
	case WM_KEYUP:
		for (auto event : instance->keyUpEvents) event.second();
		break;
	case WM_SYSKEYDOWN:
		break;
	case WM_SYSKEYUP:
		break;
	}

	return CallWindowProcW(instance->originalWindowProc, hwnd, uMsg, wParam, lParam);
}

}