/*
	* DS3RuntimeScripting
	* Contributers: Amir
*/

#pragma once
#include "pch.h"
#include "ds3_iframe_patch_durablity_cost_hook.h"
#include "ds3runtime/ds3runtime.h"
#include <ds3runtime/hooks/ds3_iframe_patch_hit_uuid_identify.h>

namespace ds3runtime::ds3IFramePatch {

FixDurablityCost::FixDurablityCost()
	: Hook(0x14057e2e0, (uintptr_t)onDurablityUse)
{
	instance = this;
}

bool FixDurablityCost::onDurablityUse(uintptr_t equipGameData, uintptr_t unk0, uintptr_t unk1, uintptr_t unk2)
{
	bool (*originalFunction)(...);
	*(uintptr_t*)&originalFunction = *instance->original;
	return true;
}

FixDurablityCost* FixDurablityCost::instance = nullptr;

}