#pragma once
#include "pch.h"
#include "talk_with_player_name.h"
#include "ds3runtime/ds3runtime.h"
#include <ds3runtime/hooks/session_receive_hook.h>
#include <ds3runtime/sprj_session_manager.h>

namespace ds3runtime {

bool TalkWithPlayerName::onAttach()
{
	((SessionReceiveHook*)ds3runtime_global
		->accessHook("session_receive_hook")
		.get())->installPacketFilter("talk_with_player_name", [&](uintptr_t networkSession, uintptr_t* networkHandle, int id, char* buffer, uint32_t maxLength, uint32_t receiveLength) -> uint32_t {
			auto session = PlayerNetworkSession(networkSession);
			auto sessionManager = SprjSessionManager(SprjSessionManager::getInstance());

			if (id == 8) {
				auto packet = packet::PlayerStruct(buffer, receiveLength);
				
			}

			return receiveLength;
			});

	return true;
}

void TalkWithPlayerName::execute()
{
}

void TalkWithPlayerName::onDetach()
{
	((SessionReceiveHook*)ds3runtime_global
		->accessHook("session_receive_hook")
		.get())->uninstallPacketFilter("player_name_talk");
}

}