/*
	* DS3RuntimeScripting
	* Contributers: Amir
*/

#pragma once
#include "pch.h"
#include "ds3runtime/hook.h"

namespace ds3runtime::ds3IFramePatch {

class HitBoxUUIDHook : public Hook
{
public:
	HitBoxUUIDHook();

	std::string getName()
	{
		return "ds3_iframe_patch_hit_uuid_hook";
	}

	static bool onHitBoxStructCreate(uintptr_t hitBoxAddress, uintptr_t unk0, uintptr_t unk1, uintptr_t hitBoxOwnerIns, uintptr_t unk2, uintptr_t unk3, uintptr_t unk4, uintptr_t unk5, uintptr_t unk6);
private:
	static HitBoxUUIDHook* instance;
};

}