/*
	* DS3RuntimeScripting
	* Contributers: Amir
*/

#pragma once
#include "pch.h"
#include "ds3_iframe_patch_hit_sibling_hook.h"
#include "ds3runtime/ds3runtime.h"
#include "ds3runtime/scripts/ds3_iframe_patch.h"
#include "ds3runtime/world_chr_man.h"
#include "ds3runtime/player_ins.h"

namespace ds3runtime::ds3IFramePatch {

HitBoxHierarchyHook::HitBoxHierarchyHook()
	: Hook(0x140832100, (uintptr_t)onSetHierarchy)
{
	instance = this;
}

void HitBoxHierarchyHook::onSetHierarchy(uintptr_t parentHitBox, uintptr_t childHitBox)
{
	void (*originalFunction)(...);
	*(uintptr_t*)&originalFunction = *instance->original;
	originalFunction(parentHitBox, childHitBox);
	DS3IFramePatch* ds3IFramePatch = (DS3IFramePatch*)ds3runtime_global->accessScript("ds3_iframe_patch");

	if (ds3IFramePatch->hasOutgoingHitBox(parentHitBox) && ds3IFramePatch->hasOutgoingHitBox(childHitBox)) {
		HitBoxRecord parent = ds3IFramePatch->getOutgoingHitBoxRecord(parentHitBox);
		HitBoxRecord child = ds3IFramePatch->getOutgoingHitBoxRecord(childHitBox);

		if (child.parentUniqueId == 0) {
			ds3IFramePatch->registerOutgoingHitBoxParent(childHitBox, parent.uniqueId);
			ds3IFramePatch->registerOutgoingHitBoxChild(parentHitBox, child.uniqueId);
			return;
		}

		uintptr_t siblingHitBox = *reinterpret_cast<uintptr_t*>(childHitBox + 0x280);
		if (siblingHitBox == 0) return;

		for (uintptr_t previousSiblingPtr = siblingHitBox + 0x280, previousSiblingCheck = *reinterpret_cast<uintptr_t*>(previousSiblingPtr);
				previousSiblingCheck == 0 && siblingHitBox != 0;
				siblingHitBox = *reinterpret_cast<uintptr_t*>(previousSiblingPtr), previousSiblingPtr = siblingHitBox + 0x280) {
			previousSiblingCheck = *reinterpret_cast<uintptr_t*>(previousSiblingPtr);
		}

		HitBoxRecord sibling = ds3IFramePatch->getOutgoingHitBoxRecord(siblingHitBox);
		ds3IFramePatch->registerOutgoingHitBoxSibling(siblingHitBox, child.uniqueId);
		ds3IFramePatch->registerOutgoingHitBoxChild(parentHitBox, child.uniqueId);
	}
}

HitBoxHierarchyHook* HitBoxHierarchyHook::instance = nullptr;

}