/*
	* DS3RuntimeScripting
	* Contributers: Amir
*/

#pragma once
#include "pch.h"
#include "ds3runtime/ds3runtime.h"
#include "ds3runtime/hooks/ds3_iframe_patch_attacker_effect_hook.h"
#include "ds3runtime/scripts/ds3_iframe_patch.h"
#include "ds3runtime/scripts/param_patcher.h"

namespace ds3runtime::ds3IFramePatch {

AttackerEffectHook::AttackerEffectHook()
	: Hook(0x14087CE00, (uintptr_t)onSpEffectApply)
{
	instance = this;
}

void AttackerEffectHook::onSpEffectApply(uintptr_t chrIns, int32_t spEffectId, uintptr_t unk0, uintptr_t unk1, uintptr_t unk2, uintptr_t unk3, uintptr_t unk4, uintptr_t unk5, uintptr_t unk6, uintptr_t unk7)
{
	void (*originalFunction)(...);
	*(uintptr_t*)&originalFunction = *instance->original;
	ChrIns attackerChr(chrIns);
	if (attackerChr.getHandle() == ChrIns::Handle::MainChr && spEffectId != -1 && ParamHandler("", L"SpEffectParam", spEffectId).isValidParam() && ParamHandler("", L"SpEffectParam", spEffectId).readBinary(0x160, 1)) return;
	originalFunction(unk0, spEffectId, unk0, unk1, unk2, unk3, unk4, unk5, unk6, unk7);
}

AttackerEffectHook* AttackerEffectHook::instance = nullptr;
}