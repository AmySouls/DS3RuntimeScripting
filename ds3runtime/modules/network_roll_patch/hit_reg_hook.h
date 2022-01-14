/*
	* DS3RuntimeScripting
	* Contributers: Amir
*/

#pragma once
#include "pch.h"
#include "ds3runtime/hook.h"

namespace ds3runtime::ds3IFramePatch {

class HitRegHook : public Hook
{
public:
	HitRegHook();

	std::string getName()
	{
		return "ds3_iframe_patch_hit_reg_hook";
	}

	static void onHitRegistration(uintptr_t hitBoxStructAddress, uint32_t hitReceiverHandle, uintptr_t unk0);
private:
	static HitRegHook* instance;
};

}