/*
	* DS3RuntimeScripting
	* Contributers: Amir
*/

#pragma once
#include "pch.h"
#include "ds3runtime/hook.h"

namespace ds3runtime::ds3IFramePatch {

class HitRegHookShockwave : public Hook
{
public:
	HitRegHookShockwave();

	std::string getName()
	{
		return "ds3_iframe_patch_hit_reg_hook_shockwave";
	}

	static bool onHitRegistration(uintptr_t CSDamagePriorityCtrl, uintptr_t hitReceiverIns);
private:
	static HitRegHookShockwave* instance;
};

}