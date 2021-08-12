/*
	* DS3RuntimeScripting
	* Contributers: Amir
*/

#pragma once
#include "pch.h"
#include "ds3runtime/hook.h"

namespace ds3runtime::ds3IFramePatch {

class HitBoxHierarchyHook : public Hook
{
public:
	HitBoxHierarchyHook();

	std::string getName()
	{
		return "ds3_iframe_patch_hit_sibling_hook";
	}

	static void onSetHierarchy(uintptr_t siblingHitBox1, uintptr_t siblingHitBox2);
private:
	static HitBoxHierarchyHook* instance;
};

}