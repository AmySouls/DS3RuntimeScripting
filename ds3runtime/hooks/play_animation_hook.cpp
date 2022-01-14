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
#include <ds3runtime/hooks/game_frame_hook.h>
#include <ds3runtime/scripts/animation_id_handler.h>

namespace ds3runtime {

PlayAnimationHook::PlayAnimationHook()
	: Hook(0x140D84870, (uintptr_t)onPlayAnimation, {})
{
	instance = this;
}

void PlayAnimationHook::onPlayAnimation(uintptr_t hkbCharacter, int32_t* animationId)
{
	int32_t filteredId = *animationId;

	for (auto filter : instance->filters) {
		filteredId = filter.second(hkbCharacter, filteredId);
		if (filteredId == 0) return;
	}

	if (PlayerIns::isMainChrLoaded()) {
		auto* gameFrameHook = (GameFrameHook*)ds3runtime_global->accessHook("game_frame_hook");
		auto* handler = (AnimationIdHandler*)ds3runtime_global->accessScript("animation_id_handler");
		std::optional<int32_t> animationSubId = handler->getAnimationId(PlayerIns::getMainChrAddress());
		spdlog::debug("Animation played: {} | Frame: {} | Hkb Character Addr: {} | Animation sub id c0000: {}", 
			ds3runtime_global->utf8_encode(PlayerIns(PlayerIns::getMainChrAddress()).getHkbStringFromId(*animationId)), 
			gameFrameHook->getFrameNumber(), 
			(void*)hkbCharacter,
			animationSubId.value_or(-1));
	}

	*animationId = filteredId;
	void (*originalFunction)(...);
	*(uintptr_t*)&originalFunction = *instance->original;
	originalFunction(hkbCharacter, animationId);
}

void PlayAnimationHook::installFilter(std::string key, AnimationFilter function)
{
	filters[key] = function;
}

void PlayAnimationHook::uninstallFilter(std::string key)
{
	filters.erase(key);
}

PlayAnimationHook* PlayAnimationHook::instance = nullptr;

}