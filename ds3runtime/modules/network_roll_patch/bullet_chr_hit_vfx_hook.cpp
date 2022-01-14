/*
	* DS3RuntimeScripting
	* Contributers: Amir
*/

#pragma once
#include "pch.h"
#include "bullet_Chr_hit_vfx_hook.h"
#include "ds3runtime/ds3runtime.h"
#include "ds3runtime/world_chr_man.h"
#include "ds3runtime/memory_util.h"
#include "ds3runtime/scripts/param_patcher.h"

namespace ds3runtime::ds3IFramePatch {

BulletChrHitVfxHook::BulletChrHitVfxHook()
	: Hook(0x140971020, (uintptr_t)onSpawnVfx, {})
{
	instance = this;
}

void BulletChrHitVfxHook::onSpawnVfx(uintptr_t sprjBulletIns, uintptr_t unk0, uintptr_t chrIns)
{
	const int32_t bulletId = *accessMultilevelPointer<int32_t>(sprjBulletIns + 0x4E4);
	ParamHandler bulletParam("", L"Bullet", bulletId);
	if (!bulletParam.isValidParam()) return;

	if (bulletParam.read<uint8_t>(0x92) == 1) {
		return;
	}
		
	void (*originalFunction)(...);
	*(uintptr_t*)&originalFunction = *instance->original;
	originalFunction(sprjBulletIns, unk0, chrIns);
}

BulletChrHitVfxHook* BulletChrHitVfxHook::instance = nullptr;

}