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
#include <spdlog/fmt/bin_to_hex.h>

namespace ds3runtime::ds3IFramePatch {

	bool DS3IFramePatch::onAttach()
	{
		((SessionReceiveHook*)ds3runtime_global
			->accessHook("session_receive_hook"))->installPacketFilter("ds3_iframe_patch_damage_fix", [&](uintptr_t networkSession, uintptr_t* networkHandle, int id, char* buffer, uint32_t maxLength, uint32_t receiveLength) -> uint32_t {
				if (id != 20) return receiveLength;
				const uint16_t hitBoxUniqueId = *reinterpret_cast<uint16_t*>(buffer + 0x78);
				const uint16_t parentHitBoxUniqueId = *reinterpret_cast<uint16_t*>(buffer + 0x7A);
				const uint16_t siblingHitBoxUniqueId = *reinterpret_cast<uint16_t*>(buffer + 0x84);
				const uint16_t childHitBoxUniqueId = *reinterpret_cast<uint16_t*>(buffer + 0x86);
				const uint64_t now = std::chrono::duration_cast<std::chrono::milliseconds>(
					std::chrono::system_clock::now().time_since_epoch()).count();

				const ReceivedHitRecord hitRecord = {
					hitBoxUniqueId,
					parentHitBoxUniqueId,
					siblingHitBoxUniqueId,
					childHitBoxUniqueId,
					now,
				};

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
						if (!effectParam.isValidParam()) continue;

						if (effectParam.readBinary(0x160, 1)) {
							packet::Effect hitEffectPacket(std::vector<char>(12));
							hitEffectPacket.setField<int32_t>("forward_id", attackerChr.getForwardId());
							hitEffectPacket.setField<int32_t>("effect_param_id", specialEffectId);
							PlayerNetworkSession(networkSession).packetSend(networkHandle, &hitEffectPacket);
						}
					}

					return receiveLength;
				}
				else {
					return 0;
				}
				});

		((SessionSendHook*)ds3runtime_global
			->accessHook("session_send_hook"))->installPacketFilter("ds3_iframe_patch_damage_fix", [&](uintptr_t playerNetworkSession, uintptr_t* networkHandle, int id, char* buffer, uint32_t maxLength) -> uint32_t {
				if (id != 20) return maxLength;
				uintptr_t hitBoxStructAddress = ((HitUUIDIdentifyHook*)ds3runtime_global->accessHook("ds3_iframe_patch_hit_uuid_identify"))->getLastHitBoxAddress();

				if (outgoingHitBoxIdMap.contains(hitBoxStructAddress)) {
					const uint16_t uniqueId = outgoingHitBoxIdMap[hitBoxStructAddress].uniqueId;
					const uint16_t siblingUniqueId = outgoingHitBoxIdMap[hitBoxStructAddress].siblingUniqueId;
					const uint16_t childUniqueId = outgoingHitBoxIdMap[hitBoxStructAddress].childUniqueId;
					const uint16_t parentUniqueId = outgoingHitBoxIdMap[hitBoxStructAddress].parentUniqueId;

					for (auto id : hitsToIgnoreThisFrame) {
						if (id == uniqueId || id == siblingUniqueId || id == childUniqueId || id == parentUniqueId) return 0;
					}

					hitsToIgnoreThisFrame.push_back(uniqueId);
					memcpy(reinterpret_cast<uint16_t*>(buffer + 0x78), &uniqueId, 2);
					memcpy(reinterpret_cast<uint16_t*>(buffer + 0x7A), &parentUniqueId, 2);
					memcpy(reinterpret_cast<uint16_t*>(buffer + 0x84), &siblingUniqueId, 2);
					memcpy(reinterpret_cast<uint16_t*>(buffer + 0x86), &childUniqueId, 2);
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

	std::vector<int32_t> brokenSpells = {
		2456000,
		2450000,
		2402000,
		1307000,
		1306000,
		1370000,
		2407000,
		2610000,
		2453000,
		2411000,
	};

	for (int32_t spell : brokenSpells) {
		ParamHandler effectParam("ds3_iframe_patch", L"Magic", spell);
		if (!effectParam.isValidParam()) continue;
		effectParam.patch<int32_t>(0x64, -1);
		effectParam.patch<int32_t>(0x68, -1);
		effectParam.patch<int32_t>(0x6C, -1);
		effectParam.patch<int32_t>(0x70, -1);
		effectParam.patch<uint8_t>(0x1E, 99);
		effectParam.patch<uint8_t>(0x1F, 99);
	}

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
		->uninstallPacketFilter("ds3_iframe_patch_patch_damage_fix");
	((SessionReceiveHook*)ds3runtime_global
		->accessHook("session_send_hook"))
		->uninstallPacketFilter("ds3_iframe_patch_damage_fix");
	((SprjChrDamageModuleHook*)ds3runtime_global
		->accessHook("sprj_chr_damage_module_hook"))
		->uninstallFilter("ds3_iframe_patch");
	((ParamPatcher*)ds3runtime_global
		->accessScript("param_patcher"))->restore("ds3_iframe_patch");
	return true;
}

void DS3IFramePatch::registerOutgoingHitBox(uintptr_t hitBoxAddress)
{
	outgoingHitBoxIdMap[hitBoxAddress] = { outgoingUniqueIdCounter, 0, 0, 0 };
	outgoingUniqueIdCounter++;
}

void DS3IFramePatch::unregisterOutgoingHitBox(uintptr_t hitBoxAddress)
{
	outgoingHitBoxIdMap.erase(hitBoxAddress);
}

void DS3IFramePatch::registerOutgoingHitBoxParent(uintptr_t hitBoxAddress, uint16_t parentUniqueId)
{
	outgoingHitBoxIdMap[hitBoxAddress].parentUniqueId = parentUniqueId;
}

void DS3IFramePatch::registerOutgoingHitBoxChild(uintptr_t hitBoxAddress, uint16_t childUniqueId)
{
	outgoingHitBoxIdMap[hitBoxAddress].childUniqueId = childUniqueId;
}

void DS3IFramePatch::registerOutgoingHitBoxSibling(uintptr_t hitBoxAddress, uint16_t siblingUniqueId)
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
	return (hitRecord.uniqueId != 0 && doesReceivedHitContainUniqueId(otherHitRecord, hitRecord.uniqueId))
		|| (hitRecord.childUniqueId != 0 && doesReceivedHitContainUniqueId(otherHitRecord, hitRecord.childUniqueId))
		|| (hitRecord.parentUniqueId != 0 && doesReceivedHitContainUniqueId(otherHitRecord, hitRecord.parentUniqueId))
		|| (hitRecord.siblingUniqueId != 0 && doesReceivedHitContainUniqueId(otherHitRecord, hitRecord.siblingUniqueId));
}

bool DS3IFramePatch::doesReceivedHitContainUniqueId(ReceivedHitRecord hitRecord, uint16_t searchUniqueId)
{
	return hitRecord.uniqueId == searchUniqueId
		|| hitRecord.childUniqueId == searchUniqueId
		|| hitRecord.parentUniqueId == searchUniqueId
		|| hitRecord.siblingUniqueId == searchUniqueId;
}

}