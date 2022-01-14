/*
	* DS3RuntimeScripting
	* Contributers: Amir
*/

#pragma once
#include "pch.h"
#include "bullet_hit_hierarchy_hook.h"
#include "ds3runtime/ds3runtime.h"
#include "ds3_iframe_patch.h"
#include "ds3runtime/world_chr_man.h"
#include "ds3runtime/player_ins.h"

namespace ds3runtime::ds3IFramePatch {

BulletHierarchyHook::BulletHierarchyHook()
	: Hook(0x140839eb0, (uintptr_t)onSetHierarchy, { "ds3_iframe_patch" })
{
	instance = this;
}

void BulletHierarchyHook::onSetHierarchy(uintptr_t* hitboxStructPointers, uint32_t hitboxId, uintptr_t hitboxHitList, uintptr_t parentHitboxHitList)
{
	DS3IFramePatch* ds3IFramePatch = (DS3IFramePatch*)ds3runtime_global->accessScript("ds3_iframe_patch");
	void (*originalFunction)(...);
	*(uintptr_t*)&originalFunction = *instance->original;
	originalFunction(hitboxStructPointers, hitboxId, hitboxHitList, parentHitboxHitList);

	auto parentHitboxOpt = ds3IFramePatch->getOutgoingHitBoxByHitList(parentHitboxHitList);
	if (!parentHitboxOpt.has_value()) return;
	auto parentHitbox = parentHitboxOpt.value();

	uint32_t offset = hitboxId >> 0x10;
	uint32_t* hitboxPtr = nullptr;

	if (offset < 0x80) { //Some fromsoft decompiled code mess
		hitboxPtr = reinterpret_cast<uint32_t*>(
			hitboxStructPointers[2] + static_cast<uintptr_t>(offset) * 0x2C0);
		//spdlog::debug("Here 3: {} | {} | {}", offset, *hitboxPtr, hitboxId);

		if (*hitboxPtr != hitboxId) return;
	}
	else {
		if (offset != 0x80) {
			return;
		}

		hitboxPtr = reinterpret_cast<uint32_t*>(*hitboxStructPointers);

		if (hitboxPtr == nullptr) return;

		while (*hitboxPtr != hitboxId) {
			hitboxPtr = *reinterpret_cast<uint32_t**>(hitboxPtr + 0xAC);

			if (hitboxPtr == nullptr) return;
		}
	}

	if (hitboxPtr == nullptr) return;

	/*
	if (*reinterpret_cast<uintptr_t**>(hitboxPtr + 0x98) == nullptr) {
		if (hitboxHitList != 0) {
			offset = *reinterpret_cast<uint32_t*>(hitboxHitList + 0x18);
			//*reinterpret_cast<uint32_t*>(hitboxHitList + 0x18) =
			//	(offset + 1 ^ offset) & 0x3fffffff ^ offset;
		}

		//*reinterpret_cast<uintptr_t*>(hitboxPtr + 0x98) = hitboxHitList;
	}

	if (*reinterpret_cast<uintptr_t**>(hitboxPtr + 0x9C) == nullptr) {
		if (parentHitboxHitList != 0) {
			offset = *reinterpret_cast<uint32_t*>(parentHitboxHitList + 0x18);
			//*reinterpret_cast<uint32_t*>(parentHitboxHitList + 0x18) =
			//	(offset + 1 ^ offset) & 0x3fffffff ^ offset;z
		}

		//*reinterpret_cast<uintptr_t*>(hitboxPtr + 0x9C) = parenthitboxHitList;
	}
	*/

	if (!ds3IFramePatch->hasOutgoingHitBox(reinterpret_cast<uintptr_t>(hitboxPtr))) return;
	const uintptr_t childHitbox = reinterpret_cast<uintptr_t>(hitboxPtr);
	auto child = ds3IFramePatch->getOutgoingHitBoxRecord(childHitbox);
	auto parent = ds3IFramePatch->getOutgoingHitBoxRecord(parentHitbox);
	
	if (child.parentUniqueId == 0) {
		ds3IFramePatch->registerOutgoingHitBoxParent(childHitbox, parent.uniqueId);
		ds3IFramePatch->registerOutgoingHitBoxChild(parentHitbox, child.uniqueId);
		return;
	}
}

BulletHierarchyHook* BulletHierarchyHook::instance = nullptr;

}