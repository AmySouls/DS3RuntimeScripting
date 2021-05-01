/*
	* DS3RuntimeScripting
	* Contributers: Amir
*/

#pragma once
#include "pch.h"
#include "ds3runtime/hook.h"
#include "ds3runtime/chr_ins.h"

namespace ds3runtime {

typedef std::function<int32_t(uintptr_t, uintptr_t, uintptr_t, int32_t)> ThrowNodeFilter;

class ThrowHook : public Hook
{
public:
	ThrowHook();

	void installFilter(std::string key, ThrowNodeFilter function);

	void uninstallFilter(std::string key);

	std::string getName()
	{
		return "throw_hook";
	}

	static void onThrow(uintptr_t throwAreaParam, uintptr_t throwerChr, uintptr_t victimChr, int32_t id, int8_t param5, int8_t param6);
private:
	std::unordered_map<std::string, ThrowNodeFilter> filters;

	static ThrowHook* instance;
};

}