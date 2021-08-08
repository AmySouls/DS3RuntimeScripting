/*
	* DS3RuntimeScripting
	* Contributers: Amir
*/

#pragma once
#include "pch.h"
#include "ds3_iframe_patch_hit_reg_hook.h"
#include "ds3runtime/ds3runtime.h"
#include "ds3runtime/world_chr_man.h"
#include "ds3runtime/memory_util.h"

namespace ds3runtime::ds3IFramePatch {

HitRegHook::HitRegHook()
	: Hook(0x140836410, (uintptr_t)onHitRegistration)
{
	instance = this;
}

void HitRegHook::onHitRegistration(uintptr_t hitBoxStructAddress, uint32_t hitReceiverHandle, uintptr_t unk0)
{
	if (hitReceiverHandle > 0x10068000 && 0x10068005 >= hitReceiverHandle) {
		return;
	}
		
	void (*originalFunction)(...);
	*(uintptr_t*)&originalFunction = *instance->original;
	originalFunction(hitBoxStructAddress, hitReceiverHandle, unk0);
}

HitRegHook* HitRegHook::instance = nullptr;

}