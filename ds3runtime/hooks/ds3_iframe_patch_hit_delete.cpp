/*
	* DS3RuntimeScripting
	* Contributers: Amir
*/

#pragma once
#include "pch.h"
#include "ds3_iframe_patch_hit_delete.h"
#include "ds3runtime/ds3runtime.h"
#include "ds3runtime/scripts/ds3_iframe_patch.h"

namespace ds3runtime::ds3IFramePatch {

HitBoxDeleteHook::HitBoxDeleteHook()
	: Hook(0x1408321f0, (uintptr_t)onHitBoxDelete)
{
	instance = this;
}

void HitBoxDeleteHook::onHitBoxDelete(uintptr_t hitBoxAddress)
{
	void (*originalFunction)(...);
	*(uintptr_t*)&originalFunction = *instance->original;
	originalFunction(hitBoxAddress);
	DS3IFramePatch* ds3IFramePatch = (DS3IFramePatch*)ds3runtime_global->accessScript("ds3_iframe_patch");
	ds3IFramePatch->unregisterOutgoingHitBox(hitBoxAddress);
}

HitBoxDeleteHook* HitBoxDeleteHook::instance = nullptr;

}