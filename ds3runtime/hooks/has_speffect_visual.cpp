#pragma once
#include "pch.h"
#include <ds3runtime/hooks/has_speffect_visual.h>
#include "ds3runtime/world_chr_man.h"

namespace ds3runtime {

HasSpEffectVisual::HasSpEffectVisual()
	: Hook(0x140881F10, (uintptr_t)onGetPhantomColorId)
{
	instance = this;
}

int32_t HasSpEffectVisual::onGetPhantomColorId(uintptr_t chrAddress)
{
	int32_t(*originalFunction)(uintptr_t);
	*(uintptr_t*)&originalFunction = *instance->original;
	int32_t phantomColorId = originalFunction(chrAddress);

	if (!WorldChrMan::hasInstance()) return 0;
	WorldChrMan worldChrMan(WorldChrMan::getInstance());

	if (worldChrMan.doesChrHaveSpEffect(chrAddress, 37)) {
		return 87;
	}

	return phantomColorId;
}

HasSpEffectVisual* HasSpEffectVisual::instance = nullptr;

}