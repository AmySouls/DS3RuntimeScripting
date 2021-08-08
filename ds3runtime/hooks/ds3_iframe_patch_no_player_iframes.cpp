/*
	* DS3RuntimeScripting
	* Contributers: Amir
*/

#pragma once
#include "pch.h"
#include "ds3_iframe_patch_no_player_iframes.h"
#include "ds3runtime/ds3runtime.h"

namespace ds3runtime::ds3IFramePatch {

NoPlayerIFrames::NoPlayerIFrames()
	: Hook(0x1409B4A50, (uintptr_t)onDodgeCheck)
{
	instance = this;
}

bool NoPlayerIFrames::onDodgeCheck(uintptr_t debugFlags, uintptr_t chrIns)
{
	bool (*originalFunction)(...);
	*(uintptr_t*)&originalFunction = *instance->original;
	const bool originalResult = originalFunction(debugFlags, chrIns);
	uint32_t hitReceiverHandle = (uint32_t)ChrIns(chrIns).getHandle();

	if (hitReceiverHandle > 0x10068000 && 0x10068005 >= hitReceiverHandle) {
		return true;
	}
	
	return originalResult;
}

NoPlayerIFrames* NoPlayerIFrames::instance = nullptr;

}