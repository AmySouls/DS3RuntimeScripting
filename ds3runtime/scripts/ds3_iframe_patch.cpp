#pragma once
#include "pch.h"
#include "ds3_iframe_patch.h"
#include <ds3runtime/hooks/session_receive_hook.h>
#include <ds3runtime/hooks/session_send_hook.h>
#include "fmod_system_handler.h"
#include "ds3runtime/world_chr_man.h"
#include "ds3runtime/hooks/ds3_iframe_patch_hit_uuid_identify.h"

namespace ds3runtime::ds3IFramePatch {

bool DS3IFramePatch::onAttach()
{
	((SessionReceiveHook*)ds3runtime_global
		->accessHook("session_receive_hook"))->installPacketFilter("ds3_iframe_patch", [&](uintptr_t networkSession, uintptr_t* networkHandle, int id, char* buffer, uint32_t maxLength, uint32_t receiveLength) -> uint32_t {
			if (id != 20) return receiveLength;
			
			const uint64_t hitBoxUniqueId = *reinterpret_cast<uint64_t*>(buffer + 0x78);
			const uint64_t now = std::chrono::duration_cast<std::chrono::milliseconds>(
				std::chrono::system_clock::now().time_since_epoch()).count();
			const uint64_t hitTimeOutMillis = 20000;
			bool doesHaveUniqueId = false;
			
			for (auto it = incomingHitBoxIdMap.begin(); it != incomingHitBoxIdMap.end(); ++it) {
				auto netHits = it->second;

				netHits.erase(std::remove_if(netHits.begin(), netHits.end(), [&](auto netHit) -> bool {
					if (hitBoxUniqueId == netHit.uniqueId) doesHaveUniqueId = true;

					if (now > netHit.timeStamp + hitTimeOutMillis) {
						return true;
					}
					else return false;
					}), netHits.end());

				it->second = netHits;
			}

			if (!WorldChrMan::hasInstance() || !PlayerIns::isMainChrLoaded()) return receiveLength;
			PlayerIns mainChr(PlayerIns::getMainChrAddress());

			if ((!incomingHitBoxIdMap.contains(networkHandle) || !doesHaveUniqueId) && !mainChr.isDodging()) {
				incomingHitBoxIdMap[networkHandle].push_back({ hitBoxUniqueId, now });
				return receiveLength;
			}
			else {
				return 0;
			}
			});

	((SessionSendHook*)ds3runtime_global
		->accessHook("session_send_hook"))->installPacketFilter("ds3_iframe_patch", [&](uintptr_t playerNetworkSession, uintptr_t* networkHandle, int id, char* buffer, uint32_t maxLength) -> uint32_t {
			if (id != 20) return maxLength;
			uintptr_t hitBoxStructAddress = ((HitUUIDIdentifyHook*)ds3runtime_global->accessHook("ds3_iframe_patch_hit_uuid_identify"))->getLastHitBoxAddress();
			
			if (outgoingHitBoxIdMap.contains(hitBoxStructAddress)) {
				const uint64_t uniqueId = outgoingHitBoxIdMap[hitBoxStructAddress];

				for (auto id : hitsToIgnoreThisFrame) {
					if (id == uniqueId) return 0;
				}

				hitsToIgnoreThisFrame.push_back(uniqueId);
				memcpy(reinterpret_cast<uint64_t*>(buffer + 0x78), &uniqueId, 8);
			}
			
			return maxLength;
			});

	return true;
}

void DS3IFramePatch::execute()
{
	hitsToIgnoreThisFrame.clear();
}

bool DS3IFramePatch::onDetach()
{
	((SessionReceiveHook*)ds3runtime_global
		->accessHook("session_receive_hook"))
		->uninstallPacketFilter("ds3_iframe_patch");
	((SessionReceiveHook*)ds3runtime_global
		->accessHook("session_send_hook"))
		->uninstallPacketFilter("ds3_iframe_patch");
	return true;
}

void DS3IFramePatch::registerOutgoingHitBox(uintptr_t hitBoxAddress)
{
	outgoingHitBoxIdMap[hitBoxAddress] = outgoingUniqueIdCounter;
	outgoingUniqueIdCounter++;
}

}