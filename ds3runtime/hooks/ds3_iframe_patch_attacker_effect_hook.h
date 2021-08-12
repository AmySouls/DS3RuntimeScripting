/*
	* DS3RuntimeScripting
	* Contributers: Amir
*/

#pragma once
#include "pch.h"
#include "ds3runtime/hook.h"

namespace ds3runtime::ds3IFramePatch {

class AttackerEffectHook : public Hook
{
public:
	AttackerEffectHook();

	std::string getName()
	{
		return "ds3_iframe_patch_attacker_effect_hook";
	}

	static void onSpEffectApply(uintptr_t chrIns, int32_t spEffectId, uintptr_t unk0, uintptr_t unk1, uintptr_t unk2, uintptr_t unk3, uintptr_t unk4, uintptr_t unk5, uintptr_t unk6, uintptr_t unk7);
private:
	static AttackerEffectHook* instance;
};

}