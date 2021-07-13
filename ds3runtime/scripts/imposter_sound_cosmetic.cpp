#pragma once
#include "pch.h"
#include "imposter_sound_cosmetic.h"
#include "ds3runtime/ds3runtime.h"
#include <ds3runtime/hooks/session_receive_hook.h>
#include <ds3runtime/sprj_session_manager.h>
#include <ds3runtime/scripts/fmod_system_handler.h>

namespace ds3runtime {

bool ImposterSoundCosmetic::onAttach()
{
	if (!ds3runtime_global->accessScript("fmod_system_handler")
		|| !((FMODSystemHandler*)ds3runtime_global->accessScript("fmod_system_handler"))->getFMODSystem()) return false;

	((SessionReceiveHook*)ds3runtime_global
		->accessHook("session_receive_hook"))
		->installPacketFilter("imposter_sound_cosmetic", [&](uintptr_t networkSession, uintptr_t* networkHandle, int id, char* buffer, uint32_t maxLength, uint32_t receiveLength) -> uint32_t {
			auto session = PlayerNetworkSession(networkSession);
			auto sessionManager = SprjSessionManager(SprjSessionManager::getInstance());

			if (id == 8) {
				auto packet = packet::PlayerStruct(buffer, receiveLength);
				const int8_t summonType = packet.getField<int8_t>("summon_type");

				if (summonType == 3 || summonType == 17 || summonType == 18) {
					FMOD::System* fmodSystem = ((FMODSystemHandler*)ds3runtime_global->accessScript("fmod_system_handler"))
						->getFMODSystem();
					FMOD::Sound* imposterSound = nullptr;
					fmodSystem->createStream("DS3RuntimeScripting\\assets\\sounds\\imposter.wav", FMOD_DEFAULT, nullptr, &imposterSound);
					fmodSystem->playSound(imposterSound, nullptr, false, nullptr);
				}
			}

			return receiveLength;
			});

	return true;
}

void ImposterSoundCosmetic::execute()
{
}

bool ImposterSoundCosmetic::onDetach()
{
	((SessionReceiveHook*)ds3runtime_global
		->accessHook("session_receive_hook"))
		->uninstallPacketFilter("imposter_sound_cosmetic");
	return true;
}

}