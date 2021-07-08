/*
	* DS3RuntimeScripting
	* Contributers: Amir
*/

#pragma once
#include "pch.h"
#include "throw_hook.h"
#include "ds3runtime/ds3runtime.h"

namespace ds3runtime {

ThrowHook::ThrowHook()
	: Hook(0x1405F3840, (uintptr_t)onThrow)
{
	instance = this;
}

void ThrowHook::onThrow(uintptr_t throwAreaParam, uintptr_t throwerChr, uintptr_t victimChr, int32_t id, int8_t param5, int8_t param6)
{
	int32_t filteredId = id;

	for (auto filter : instance->filters) {
		filteredId = filter.second(throwAreaParam, throwerChr, victimChr, filteredId);
		if (filteredId == 0) return;
	}

	void (*originalFunction)(...);
	*(uintptr_t*)&originalFunction = *instance->original;
	originalFunction(throwAreaParam, throwerChr, victimChr, id, param5, param6); //Search for ThrowAreaParam in ghidra for more information.
}

void ThrowHook::installFilter(std::string key, ThrowNodeFilter function)
{
	filters[key] = function;
}

void ThrowHook::uninstallFilter(std::string key)
{
	filters.erase(key);
}

ThrowHook* ThrowHook::instance = nullptr;

}