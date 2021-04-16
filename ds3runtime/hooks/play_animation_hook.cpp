/*
	* DS3RuntimeScripting
	* Contributers: Amir
*/

#pragma once
#include "pch.h"
#include "play_animation_hook.h"
#include "ds3runtime/ds3runtime.h"
#include "ds3runtime/player_ins.h"
#include <ds3runtime/databaseaddress.h>
#include "ds3runtime/sprj_gaitem_imp.h"

namespace ds3runtime {

PlayAnimationHook::PlayAnimationHook()
	: Hook(0x140D84870, (uintptr_t)onPlayAnimation)
{
	instance = this;
}

void PlayAnimationHook::onPlayAnimation(uintptr_t chrAnimationHandle, int32_t* animationId)
{
	int32_t filteredId = *animationId;

	for (auto filter : instance->filters) {
		spdlog::debug("Filtered id: {}", filteredId);
		filteredId = filter.second(chrAnimationHandle, filteredId);
		if (filteredId == 0) return;
	}

	*animationId = filteredId;
	void (*originalFunction)(...);
	*(uintptr_t*)&originalFunction = *instance->original;
	originalFunction(chrAnimationHandle, animationId);
	//auto list = SprjGaitemImp(SprjGaitemImp::getInstance()).getItemList();
	//for (auto item : list) spdlog::debug("Item id in inventory: {}", item.getId());
}

void PlayAnimationHook::installFilter(std::string key, AnimationFilter function)
{
	filters[key] = function;;
}

void PlayAnimationHook::uninstallFilter(std::string key)
{
	filters.erase(key);
}

PlayAnimationHook* PlayAnimationHook::instance = nullptr;

}