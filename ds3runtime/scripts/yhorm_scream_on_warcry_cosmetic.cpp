#pragma once
#include "pch.h"
#include "yhorm_scream_on_warcry_cosmetic.h"
#include <ds3runtime/hooks/play_animation_hook.h>
#include "fmod_system_handler.h"

namespace ds3runtime {

	bool YhormScreamOnWarcry::onAttach()
	{
		if (!ds3runtime_global->accessScript("fmod_system_handler")
			|| !((FMODSystemHandler*)ds3runtime_global->accessScript("fmod_system_handler"))->getFMODSystem()) return false;

		((PlayAnimationHook*)ds3runtime_global
			->accessHook("play_anim_hook"))->installFilter("yhorm_scream_on_warcry_cosmetic", [this](uintptr_t hkbCharacter, int32_t animationPlayId) -> int32_t {
				if (!PlayerIns::isMainChrLoaded()) return animationPlayId;
				ChrIns chr(PlayerIns::getMainChrAddress());
				if (!chr.isValid() || !chr.hasHkbCharacter() || hkbCharacter != chr.getHkbCharacter() || animationPlayId != 3338) return animationPlayId;
			FMOD::System* fmodSystem = ((FMODSystemHandler*)ds3runtime_global->accessScript("fmod_system_handler"))->getFMODSystem();
			FMOD::Sound* yhormScreamSound = nullptr;
			fmodSystem->createStream("DS3RuntimeScripting\\assets\\sounds\\yhorm_scream.wav", FMOD_DEFAULT, nullptr, &yhormScreamSound);
			fmodSystem->playSound(yhormScreamSound, nullptr, false, nullptr);
			return animationPlayId;
			});

	return true;
}

void YhormScreamOnWarcry::execute()
{
	
}

bool YhormScreamOnWarcry::onDetach()
{
	((PlayAnimationHook*)ds3runtime_global->accessHook("play_anim_hook"))->uninstallFilter("yhorm_scream_on_warcry_cosmetic");
	return true;
}

}