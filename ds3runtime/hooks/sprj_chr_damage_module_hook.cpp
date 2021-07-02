/*
	* DS3RuntimeScripting
	* Contributers: Amir
*/

#pragma once
#include "pch.h"
#include "sprj_chr_damage_module_hook.h"
#include "ds3runtime/ds3runtime.h"

namespace ds3runtime {

SprjChrDamageModuleHook::SprjChrDamageModuleHook()
	: Hook(0x14451f085, (uintptr_t)onApplyDamage)
{
	instance = this;
}

void SprjChrDamageModuleHook::onApplyDamage(uintptr_t sprjChrDamageModule, uintptr_t attackerChr, char* attackDataBuffer)
{
	bool permitted = true;

	for (auto filter : instance->filters) {
		permitted = filter.second(sprjChrDamageModule, attackerChr, attackDataBuffer);
		if (permitted == false) return;
	}

	void (*originalFunction)(...);
	*(uintptr_t*)&originalFunction = *instance->original;
	originalFunction(sprjChrDamageModule, attackerChr, attackDataBuffer);
}

void SprjChrDamageModuleHook::installFilter(std::string key, AttackFilter function)
{
	filters[key] = function;
}

void SprjChrDamageModuleHook::uninstallFilter(std::string key)
{
	filters.erase(key);
}

SprjChrDamageModuleHook* SprjChrDamageModuleHook::instance = nullptr;

}