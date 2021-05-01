/*
	* DS3RuntimeScripting
	* Contributers: Amir
*/

#pragma once
#include "pch.h"
#include "ds3runtime/hook.h"

namespace ds3runtime {

typedef std::function<bool(uintptr_t, uintptr_t, char*)> AttackFilter;

class SprjChrDamageModuleHook : public Hook
{
public:
	SprjChrDamageModuleHook();

	void installFilter(std::string key, AttackFilter function);

	void uninstallFilter(std::string key);

	std::string getName()
	{
		return "sprj_chr_damage_module_hook";
	}

	static void onApplyDamage(uintptr_t sprjChrDamageModule, uintptr_t attackerChr, char* attackDataBuffer);
private:
	std::unordered_map<std::string, AttackFilter> filters;

	static SprjChrDamageModuleHook* instance;
};

}