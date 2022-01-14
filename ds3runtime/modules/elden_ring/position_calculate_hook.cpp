/*
	* DS3RuntimeScripting
	* Contributers: Amir
*/

#pragma once
#include "pch.h"
#include "position_calculate_hook.h"
#include "ds3runtime/ds3runtime.h"
#include "ds3runtime/world_chr_man.h"
#include "ds3runtime/player_ins.h"
#include "ds3runtime/scripts/animation_id_handler.h"

namespace ds3runtime::eldenring {

PositionCalculateHook::PositionCalculateHook()
	: Hook(0x14165b420, (uintptr_t)onUpdatePosition, { "animation_id_handler" })
{
	instance = this;
}

void PositionCalculateHook::onUpdatePosition(uintptr_t CsHkCharacterProxy, uintptr_t* SprjChrPhysicsModulePtr, uintptr_t unk0, uintptr_t unk1, uintptr_t unk2)
{
	void (*originalFunction)(...);
	*(uintptr_t*)&originalFunction = *instance->original;

	for (auto filter : instance->filters) {
		filter.second(CsHkCharacterProxy, SprjChrPhysicsModulePtr);
	}

	originalFunction(CsHkCharacterProxy, SprjChrPhysicsModulePtr, unk0, unk1, unk2);
}

void PositionCalculateHook::installFilter(std::string key, MotionFilter function)
{
	filters[key] = function;
}

void PositionCalculateHook::uninstallFilter(std::string key)
{
	filters.erase(key);
}

PositionCalculateHook* PositionCalculateHook::instance = nullptr;

}