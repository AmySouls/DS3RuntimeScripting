#pragma once
#include "pch.h"
#include "damage_update_test.h"
#include "ds3runtime/ds3runtime.h"
#include <ds3runtime/scripts/param_patcher.h>
#include <ds3runtime/world_chr_man.h>
#include <ds3runtime/hooks/session_receive_hook.h>
#include <ds3runtime/sprj_session_manager.h>
#include <ds3runtime/hooks/session_send_hook.h>

namespace ds3runtime {

bool DamageUpdateTest::onAttach()
{
	/*
	((SessionReceiveHook*)ds3runtime_global
		->accessHook("session_receive_hook")
		.get())->installPacketFilter("damage_update_test", [&](uintptr_t networkSession, uintptr_t* networkHandle, int id, char* buffer, uint32_t maxLength, uint32_t receiveLength) -> uint32_t {
			auto session = PlayerNetworkSession(networkSession);
			auto sessionManager = SprjSessionManager(SprjSessionManager::getInstance());

			if (id == 28) {
				didReceiveHitOnThisFrame = true;
			}

			return receiveLength;
			});
	return true;
	*/

	((SessionSendHook*)ds3runtime_global
		->accessHook("session_send_hook"))
		->installPacketFilter("damage_update_test", [&](uintptr_t playerNetworkSession, uintptr_t* networkHandle, int id, char* buffer, uint32_t maxLength) -> uint32_t {
			auto session = PlayerNetworkSession(playerNetworkSession);

			if (id == 20) {
				spdlog::debug("Time between hit id {} and the previous: {}", *(int32_t*)(buffer + 0x44), lastHitSendTime);
				lastHitSendTime.reset();
			}

			return maxLength;
		});

	return true;
}

void DamageUpdateTest::execute()
{
	if (!PlayerNetworkSession::hasInstance()) {
		remove();
		return;
	}

	/*
	for (int i = 0; i < 30; i++) {
		auto session = PlayerNetworkSession(PlayerNetworkSession::getInstance());
		packet::Hit packet = packet::Hit(std::vector<char>(0xB0));
		packet.setField("physical_damage", 1.0f);
		packet.setField("atk_param_id", 3203000);
		*(int32_t*)(packet.getData() + 0x74) = -1;
		session.sessionPacketSend(&packet);
	}

	if (didReceiveHitOnThisFrame) {
		spdlog::debug("Frames since last received hit: {}", currentFrame - lastFrameWithHitReceive);
		spdlog::debug("Time since last received hit: {}", lastHitReceiveTime);
		totalFramesBetweenHitReceives += currentFrame - lastFrameWithHitReceive;
		lastFrameWithHitReceive = currentFrame;
		lastHitReceiveTime.reset();
		didReceiveHitOnThisFrame = false;
	}

	currentFrame++;

	if (currentFrame > 240) {
		spdlog::debug("Average frames between hits: {}", totalFramesBetweenHitReceives / 240.0f);
		remove();
	}
	*/
}

bool DamageUpdateTest::onDetach()
{
	((SessionReceiveHook*)ds3runtime_global
		->accessHook("session_receive_hook"))
		->uninstallPacketFilter("damage_update_test");
	return true;
}

}