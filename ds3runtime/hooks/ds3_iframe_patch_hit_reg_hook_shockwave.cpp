/*
	* DS3RuntimeScripting
	* Contributers: Amir
*/

#pragma once
#include "pch.h"
#include "ds3_iframe_patch_hit_reg_hook_shockwave.h"
#include "ds3runtime/ds3runtime.h"

namespace ds3runtime::ds3IFramePatch {

HitRegHookShockwave::HitRegHookShockwave()
	: Hook(0x1409AFCE0, (uintptr_t)onHitRegistration)
{
	instance = this;
}

bool HitRegHookShockwave::onHitRegistration(uintptr_t CSDamagePriorityCtrl, uintptr_t hitReceiverIns)
{
	const uint32_t receiverHandle = *(uint32_t*)(hitReceiverIns + 0x8);

	if (receiverHandle > 0x10068000 && 0x10068005 <= receiverHandle) {
		return true;
	}
		
	bool (*originalFunction)(...);
	*(uintptr_t*)&originalFunction = *instance->original;
	return originalFunction(CSDamagePriorityCtrl, hitReceiverIns);
}

HitRegHookShockwave* HitRegHookShockwave::instance = nullptr;

}