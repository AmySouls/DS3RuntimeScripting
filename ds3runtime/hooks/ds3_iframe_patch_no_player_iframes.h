/*
* DS3RuntimeScripting
* Contributers: Amir
*/

#pragma once
#include "pch.h"
#include "ds3runtime/hook.h"

namespace ds3runtime::ds3IFramePatch {

class NoPlayerIFrames : public Hook
{
public:
	NoPlayerIFrames();

	std::string getName()
	{
		return "ds3_iframe_patch_no_player_iframes";
	}

	static bool onDodgeCheck(uintptr_t debugFlags, uintptr_t chrIns);
private:
	static NoPlayerIFrames* instance;
};

}