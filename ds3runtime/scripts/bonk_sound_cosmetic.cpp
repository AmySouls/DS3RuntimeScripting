#pragma once
#include "pch.h"
#include "bonk_sound_cosmetic.h"
#include <ds3runtime/hooks/sprj_chr_damage_module_hook.h>
#include "fmod_system_handler.h"

namespace ds3runtime {

bool BonkSoundCosmetic::onAttach()
{
	if (!ds3runtime_global->accessScript("fmod_system_handler")
		|| !((FMODSystemHandler*)ds3runtime_global->accessScript("fmod_system_handler"))->getFMODSystem()) return false;

	((SprjChrDamageModuleHook*)ds3runtime_global
		->accessHook("sprj_chr_damage_module_hook"))->installFilter("bonk_sound_cosmetic", [&](uintptr_t, uintptr_t, char* attackDataBuffer) -> bool {
			int32_t* atkId = reinterpret_cast<int32_t*>(attackDataBuffer + 0x44);
			static const int32_t reinforedClubBaseAtkId = 3302000;
			static const int32_t clubBaseAtkId = 3300000;

			if (*atkId - *atkId % 1000 == 3300000
				|| *atkId - *atkId % 1000 == 3302000) {
				FMOD::System* fmodSystem = ((FMODSystemHandler*)ds3runtime_global->accessScript("fmod_system_handler"))->getFMODSystem();
				FMOD::Sound* bonkSound = nullptr;
				fmodSystem->createStream("DS3RuntimeScripting\\assets\\sounds\\bonk.wav", FMOD_DEFAULT, nullptr, &bonkSound);
				fmodSystem->playSound(bonkSound, nullptr, false, nullptr);
			}

			return true;
			});

	return true;
}

void BonkSoundCosmetic::execute()
{
	
}

bool BonkSoundCosmetic::onDetach()
{
	((SprjChrDamageModuleHook*)ds3runtime_global->accessHook("sprj_chr_damage_module_hook"))->uninstallFilter("bonk_sound_cosmetic");
	return true;
}

}