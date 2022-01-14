/*
	* DS3RuntimeScripting
	* Contributers: Amir
*/

#pragma once
#include "pch.h"
#include "ds3runtime/hook.h"

namespace ds3runtime::ds3IFramePatch {

class HitBoxDeleteHook : public Hook
{
public:
	HitBoxDeleteHook();

	std::string getName()
	{
		return "ds3_iframe_patch_hit_delete";
	}

	static void onHitBoxDelete(uintptr_t hitBoxAddress);
private:
	static HitBoxDeleteHook* instance;
};

}