/*
* DS3RuntimeScripting
* Contributers: Amir
*/

#pragma once
#include "pch.h"
#include "ds3runtime/hook.h"
#include <ds3runtime/scripts/ds3_iframe_patch.h>

namespace ds3runtime::ds3IFramePatch {

struct CostedHit
{
	uintptr_t hitBoxAddress;
	uint64_t timeStamp;
};

class FixDurablityCost : public Hook
{
public:
	FixDurablityCost();

	std::string getName()
	{
		return "ds3_iframe_patch_durablity_cost_hook";
	}

	static bool onDurablityUse(uintptr_t equipGameData, uintptr_t unk0, uintptr_t unk1, uintptr_t unk2);
private:
	static FixDurablityCost* instance;
};

}