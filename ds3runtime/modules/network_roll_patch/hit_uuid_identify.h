/*
	* DS3RuntimeScripting
	* Contributers: Amir
*/

#pragma once
#include "pch.h"
#include "ds3runtime/hook.h"

namespace ds3runtime::ds3IFramePatch {

class HitUUIDIdentifyHook : public Hook
{
public:
	HitUUIDIdentifyHook();

	std::string getName()
	{
		return "ds3_iframe_patch_hit_uuid_identify";
	}

	uintptr_t getLastHitBoxAddress();

	static void onHitCheck(uintptr_t hitBoxStructAddress, uintptr_t unk0, uintptr_t unk1, uintptr_t unk2, uintptr_t unk3, uintptr_t unk4, uintptr_t unk5);
private:
	uintptr_t lastHitBoxAddress = 0;

	static HitUUIDIdentifyHook* instance;
};

}