#pragma once
#include "pch.h"
#include "ds3_iframe_patch.h"
#include <ds3runtime/hooks/session_receive_hook.h>
#include <ds3runtime/hooks/session_send_hook.h>
#include "fmod_system_handler.h"
#include "ds3runtime/world_chr_man.h"
#include "ds3runtime/hooks/ds3_iframe_patch_hit_uuid_identify.h"
#include <ds3runtime/sprj_session_manager.h>
#include <ds3runtime/hooks/sprj_chr_damage_module_hook.h>
#include <ds3runtime/scripts/param_patcher.h>

namespace ds3runtime::ds3IFramePatch {

	bool DS3IFramePatch::onAttach()
	{
		((SessionReceiveHook*)ds3runtime_global
			->accessHook("session_receive_hook"))->installPacketFilter("ds3_iframe_patch", [&](uintptr_t networkSession, uintptr_t* networkHandle, int id, char* buffer, uint32_t maxLength, uint32_t receiveLength) -> uint32_t {
				if (id != 20) return receiveLength;
				const int64_t hitBoxUniqueId = *reinterpret_cast<uint64_t*>(buffer + 0x78);
				const int64_t parentHitBoxUniqueId = *reinterpret_cast<uint64_t*>(buffer + 0x14);
				const int64_t siblingHitBoxUniqueId = *reinterpret_cast<uint64_t*>(buffer + 0x80);
				const int64_t childHitBoxUniqueId = *reinterpret_cast<uint64_t*>(buffer + 0x1C);

				const ReceivedHitRecord hitRecord = {
					hitBoxUniqueId,
					parentHitBoxUniqueId,
					siblingHitBoxUniqueId,
					childHitBoxUniqueId
				};

				const uint64_t now = std::chrono::duration_cast<std::chrono::milliseconds>(
					std::chrono::system_clock::now().time_since_epoch()).count();
				const uint64_t hitTimeOutMillis = 20000;
				bool doesHaveHit = false;

				for (auto it = incomingHitBoxIdMap.begin(); it != incomingHitBoxIdMap.end(); ++it) {
					auto netHits = it->second;

					netHits.erase(std::remove_if(netHits.begin(), netHits.end(), [&](auto netHit) -> bool {
						if (doesReceivedHitShareUniqueId(netHit, hitRecord)) doesHaveHit = true;

						if (now > netHit.timeStamp + hitTimeOutMillis) {
							return true;
						}
						else return false;
						}), netHits.end());

					it->second = netHits;
				}

				if (!WorldChrMan::hasInstance() || !PlayerIns::isMainChrLoaded()) return receiveLength;
				PlayerIns mainChr(PlayerIns::getMainChrAddress());

				if ((!incomingHitBoxIdMap.contains(networkHandle) || !doesHaveHit) && !mainChr.isDodging()) {
					ReceivedHitRecord hitRecord = {};
					hitRecord.uniqueId = hitBoxUniqueId;
					hitRecord.siblingUniqueId = siblingHitBoxUniqueId;
					hitRecord.parentUniqueId = parentHitBoxUniqueId;
					hitRecord.childUniqueId = childHitBoxUniqueId;
					hitRecord.timeStamp = now;
					incomingHitBoxIdMap[networkHandle].push_back(hitRecord);
					SprjSessionManager sessionManager(SprjSessionManager::getInstance());
					PlayerIns attackerChr(sessionManager.getPlayerInsByNetworkHandle(networkHandle));

					for (uintptr_t i = 0; i < 8; i++) {
						const int32_t specialEffectId = *reinterpret_cast<int32_t*>(buffer + 0x4C + i * 4ul);
						if (specialEffectId == -1 || specialEffectId == 0) continue;
						ParamHandler effectParam("", L"SpEffectParam", specialEffectId);
						if (!effectParam.isValidParam() || !effectParam.readBinary(0x160, 1)) continue;
						packet::Effect hitEffectPacket(std::vector<char>(12));
						hitEffectPacket.setField<int32_t>("forward_id", attackerChr.getForwardId());
						hitEffectPacket.setField<int32_t>("effect_param_id", specialEffectId);
						PlayerNetworkSession(networkSession).packetSend(networkHandle, &hitEffectPacket);
					}

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
					const int64_t uniqueId = outgoingHitBoxIdMap[hitBoxStructAddress].uniqueId;
					const int64_t siblingUniqueId = outgoingHitBoxIdMap[hitBoxStructAddress].siblingUniqueId;
					const int64_t childUniqueId = outgoingHitBoxIdMap[hitBoxStructAddress].childUniqueId;
					const int64_t parentUniqueId = outgoingHitBoxIdMap[hitBoxStructAddress].parentUniqueId;

					for (auto id : hitsToIgnoreThisFrame) {
						if (id == uniqueId || id == siblingUniqueId || id == childUniqueId || id == parentUniqueId) return 0;
					}

					hitsToIgnoreThisFrame.push_back(uniqueId);
					memcpy(reinterpret_cast<int64_t*>(buffer + 0x78), &uniqueId, 8);
					memcpy(reinterpret_cast<int64_t*>(buffer + 0x80), &siblingUniqueId, 8);
					memcpy(reinterpret_cast<int64_t*>(buffer + 0x14), &parentUniqueId, 8);
					memcpy(reinterpret_cast<int64_t*>(buffer + 0x1C), &childUniqueId, 8);
				}

				return maxLength;
				});

		((SprjChrDamageModuleHook*)ds3runtime_global
			->accessHook("sprj_chr_damage_module_hook"))->installFilter("ds3_iframe_patch", [&](uintptr_t sprjChrDamageModule, uintptr_t attackerChr, char* attackDataBuffer) -> bool {
				uint32_t receiverHandle = *accessMultilevelPointer<uint32_t>(sprjChrDamageModule + 8, 8);
				uint32_t attackerHandle = *reinterpret_cast<uint32_t*>(attackerChr + 8);

			if ((receiverHandle > 0x10068000 && receiverHandle <= 0x10068005) || (attackerHandle > 0x10068000 && attackerHandle <= 0x10068005)) {
				return true;
			}
			
			for (uintptr_t i = 0; i < 5; i++) {
				const int32_t specialEffectId = *reinterpret_cast<int32_t*>(attackDataBuffer + 0x80 + i * 4ul);
				if (specialEffectId == -1 || specialEffectId == 0) continue;
				void (*applyEffect)(...);
				*(uintptr_t*)&applyEffect = 0x140886C40;
				applyEffect(attackerChr, specialEffectId, attackerChr, 0, 0, 0, 0, 0, 0, 0, 0);
				if (!PlayerNetworkSession::hasInstance()) continue;
				packet::Effect hitEffectPacket(std::vector<char>(12));
				hitEffectPacket.setField<int32_t>("forward_id", ChrIns(attackerChr).getForwardId());
				hitEffectPacket.setField<int32_t>("effect_param_id", specialEffectId);
				PlayerNetworkSession(PlayerNetworkSession::getInstance()).sessionPacketSend(&hitEffectPacket);
			}

			return true;
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
	((SprjChrDamageModuleHook*)ds3runtime_global
		->accessHook("sprj_chr_damage_module_hook"))
		->uninstallFilter("ds3_iframe_patch");
	return true;
}

void DS3IFramePatch::registerOutgoingHitBox(uintptr_t hitBoxAddress)
{
	outgoingHitBoxIdMap[hitBoxAddress] = { outgoingUniqueIdCounter, -1, -1, -1 };
	outgoingUniqueIdCounter++;
}

void DS3IFramePatch::unregisterOutgoingHitBox(uintptr_t hitBoxAddress)
{
	outgoingHitBoxIdMap.erase(hitBoxAddress);
}

void DS3IFramePatch::registerOutgoingHitBoxParent(uintptr_t hitBoxAddress, int64_t parentUniqueId)
{
	outgoingHitBoxIdMap[hitBoxAddress].parentUniqueId = parentUniqueId;
}

void DS3IFramePatch::registerOutgoingHitBoxChild(uintptr_t hitBoxAddress, int64_t childUniqueId)
{
	outgoingHitBoxIdMap[hitBoxAddress].childUniqueId = childUniqueId;
}

void DS3IFramePatch::registerOutgoingHitBoxSibling(uintptr_t hitBoxAddress, int64_t siblingUniqueId)
{
	outgoingHitBoxIdMap[hitBoxAddress].siblingUniqueId = siblingUniqueId;
}

bool DS3IFramePatch::hasOutgoingHitBox(uintptr_t hitBoxAddress)
{
	return outgoingHitBoxIdMap.contains(hitBoxAddress);
}

HitBoxRecord DS3IFramePatch::getOutgoingHitBoxRecord(uintptr_t hitBoxAddress)
{
	return outgoingHitBoxIdMap[hitBoxAddress];
}

bool DS3IFramePatch::doesReceivedHitShareUniqueId(ReceivedHitRecord hitRecord, ReceivedHitRecord otherHitRecord)
{
	return (hitRecord.uniqueId != -1 && doesReceivedHitContainUniqueId(otherHitRecord, hitRecord.uniqueId))
		|| (hitRecord.childUniqueId != -1 && doesReceivedHitContainUniqueId(otherHitRecord, hitRecord.childUniqueId))
		|| (hitRecord.parentUniqueId != -1 && doesReceivedHitContainUniqueId(otherHitRecord, hitRecord.parentUniqueId))
		|| (hitRecord.siblingUniqueId != -1 && doesReceivedHitContainUniqueId(otherHitRecord, hitRecord.siblingUniqueId));
}

bool DS3IFramePatch::doesReceivedHitContainUniqueId(ReceivedHitRecord hitRecord, int64_t searchUniqueId)
{
	return hitRecord.uniqueId == searchUniqueId
		|| hitRecord.childUniqueId == searchUniqueId
		|| hitRecord.parentUniqueId == searchUniqueId
		|| hitRecord.siblingUniqueId == searchUniqueId;
}

}