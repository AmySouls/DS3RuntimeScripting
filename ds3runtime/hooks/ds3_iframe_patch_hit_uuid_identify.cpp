/*
	* DS3RuntimeScripting
	* Contributers: Amir
*/

#pragma once
#include "pch.h"
#include "ds3runtime/ds3runtime.h"
#include "ds3_iframe_patch_hit_uuid_identify.h"
#include "ds3runtime/scripts/ds3_iframe_patch.h"

namespace ds3runtime::ds3IFramePatch {

HitUUIDIdentifyHook::HitUUIDIdentifyHook()
	: Hook(0x140836490, (uintptr_t)onHitCheck)
{
	instance = this;
}

uintptr_t HitUUIDIdentifyHook::getLastHitBoxAddress()
{
	return lastHitBoxAddress;
}

void HitUUIDIdentifyHook::onHitCheck(uintptr_t hitBoxStructAddress, uintptr_t unk0, uintptr_t unk1, uintptr_t unk2, uintptr_t unk3, uintptr_t unk4, uintptr_t unk5)
{
	void (*originalFunction)(...);
	*(uintptr_t*)&originalFunction = *instance->original;
	instance->lastHitBoxAddress = hitBoxStructAddress;
	originalFunction(hitBoxStructAddress, unk0, unk1, unk2, unk3, unk4, unk5);
}

HitUUIDIdentifyHook* HitUUIDIdentifyHook::instance = nullptr;

}