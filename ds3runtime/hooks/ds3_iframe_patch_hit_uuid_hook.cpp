/*
	* DS3RuntimeScripting
	* Contributers: Amir
*/

#pragma once
#include "pch.h"
#include "ds3_iframe_patch_hit_uuid_hook.h"
#include "ds3runtime/ds3runtime.h"
#include "ds3runtime/scripts/ds3_iframe_patch.h"
#include "ds3runtime/world_chr_man.h"
#include "ds3runtime/player_ins.h"

namespace ds3runtime::ds3IFramePatch {

HitBoxUUIDHook::HitBoxUUIDHook()
	: Hook(0x140832500, (uintptr_t)onHitBoxStructCreate)
{
	instance = this;
}

uintptr_t HitBoxUUIDHook::onHitBoxStructCreate(uintptr_t hitBoxAddress, uintptr_t unk0, uintptr_t unk1, uintptr_t hitBoxOwnerIns, uintptr_t unk2)
{
	bool (*originalFunction)(...);
	*(uintptr_t*)&originalFunction = *instance->original;
	
	if (WorldChrMan::hasInstance() && PlayerIns::isMainChrLoaded() && hitBoxOwnerIns == PlayerIns::getMainChrAddress()) {
		((DS3IFramePatch*)ds3runtime_global->accessScript("ds3_iframe_patch"))->registerOutgoingHitBox(hitBoxAddress);
	}

	return originalFunction(hitBoxAddress, unk0, unk1, hitBoxOwnerIns, unk2);
}

HitBoxUUIDHook* HitBoxUUIDHook::instance = nullptr;

}