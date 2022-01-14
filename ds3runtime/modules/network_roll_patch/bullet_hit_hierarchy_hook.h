/*
	* DS3RuntimeScripting
	* Contributers: Amir
*/

#pragma once
#include "pch.h"
#include "ds3runtime/hook.h"

namespace ds3runtime::ds3IFramePatch {

class BulletHierarchyHook : public Hook
{
public:
	BulletHierarchyHook();

	std::string getName()
	{
		return "ds3_iframe_patch_bullet_hierarchy_hook";
	}

	static void onSetHierarchy(uintptr_t* hitboxStructPointers, uint32_t hitboxId, uintptr_t hitboxHitList, uintptr_t unk0);
private:
	static BulletHierarchyHook* instance;
};

}