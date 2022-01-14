#pragma once
#include "pch.h"
#include "ds3_iframe_patch.h"
#include <ds3runtime/hooks/session_receive_hook.h>
#include <ds3runtime/hooks/session_send_hook.h>
#include "ds3runtime/scripts/fmod_system_handler.h"
#include "ds3runtime/world_chr_man.h"
#include "hit_uuid_identify.h"
#include <ds3runtime/sprj_session_manager.h>
#include <ds3runtime/hooks/sprj_chr_damage_module_hook.h>
#include <ds3runtime/scripts/param_patcher.h>
#include <spdlog/fmt/bin_to_hex.h>
#include "ds3runtime/hooks/jmp_table_status.h"
#include "ds3runtime/i_chr_finder.h"

namespace ds3runtime::ds3IFramePatch {

	bool DS3IFramePatch::onAttach()
	{
		((SessionReceiveHook*)ds3runtime_global
			->accessHook("session_receive_hook"))->installPacketFilter("ds3_iframe_patch_damage_fix", [&](uintptr_t networkSession, uintptr_t* networkHandle, int id, char* buffer, uint32_t maxLength, uint32_t receiveLength) -> uint32_t {
				if (id == 20) { //Player Hit
					return processIncomingPlayerHitPacket(networkSession, networkHandle, id, buffer, maxLength, receiveLength);
				} else if (id == 28) { //Hit Vfx
					return processIncomingHitVfxPacket(networkSession, networkHandle, id, buffer, maxLength, receiveLength);
				}

				return receiveLength;
				});

		((SessionSendHook*)ds3runtime_global
			->accessHook("session_send_hook"))->installPacketFilter("ds3_iframe_patch_damage_fix", [&](uintptr_t playerNetworkSession, uintptr_t* networkHandle, int id, char* buffer, uint32_t maxLength) -> uint32_t {
				if (id == 20) { //Player Hit
					return processOutgoingPlayerHitPacket(playerNetworkSession, networkHandle, id, buffer, maxLength);
				} else if (id == 28) { //Hit Vfx
					return processOutgoingHitVfxPacket(playerNetworkSession, networkHandle, id, buffer, maxLength);
				}

				return maxLength;
				});
		

		((SprjChrDamageModuleHook*)ds3runtime_global
			->accessHook("sprj_chr_damage_module_hook"))->installFilter("ds3_iframe_patch", [&](uintptr_t sprjChrDamageModule, uintptr_t attackerChr, char* attackDataBuffer) -> bool {
			uintptr_t receiverChr = *accessMultilevelPointer<uintptr_t>(sprjChrDamageModule + 8);
			uint32_t receiverHandle = *accessMultilevelPointer<uint32_t>(sprjChrDamageModule + 8, 8);
			uint32_t attackerHandle = *reinterpret_cast<uint32_t*>(attackerChr + 8);

			//Apply hit effects for hits between local player and NPCs
			if (!(receiverHandle > 0x10068000 && receiverHandle <= 0x10068005) 
					&& !(attackerHandle > 0x10068000 && attackerHandle <= 0x10068005)) {
				for (uintptr_t i = 0; i < 5; i++) {
					const int32_t specialEffectId = *reinterpret_cast<int32_t*>(attackDataBuffer + 0x80 + i * 4ul);
					if (specialEffectId == -1 || specialEffectId == 0) continue;
					ChrIns(attackerChr).applySpEffect(specialEffectId);
				}
			}

			if (receiverHandle != 0x10068000 && ChrIns(receiverChr).getCharacterId() != L"c0000" || ChrIns(attackerChr).getCharacterId() != L"c0000") return true;

			const bool areAllies = IChrFinder::isBothChrAllies(attackerChr, *accessMultilevelPointer<uintptr_t>(sprjChrDamageModule + 8));

			if (areAllies && settings.isFriendlyHitstun) { //Process friendly hitstun
				float* knockBackAmount = reinterpret_cast<float*>(attackDataBuffer + 0x5C);
				int32_t* attackId = reinterpret_cast<int32_t*>(attackDataBuffer + 0x44);
				int32_t* damageLevel = reinterpret_cast<int32_t*>(attackDataBuffer + 0x24);
				float* damageAmount = reinterpret_cast<float*>(attackDataBuffer);
				std::optional<ParamHandler> atkParam = ParamHandler("", L"AtkParam_Pc", *attackId);

				if (!atkParam.has_value() || !atkParam->isValidParam()) return false;
				const bool throwFlag = atkParam->readBinary(0x7D, 0); //Cancel throw attacks
				if (throwFlag) return false;

				for (int32_t i = 0; i < 5; ++i) { //Clear all damage
					*(damageAmount + i) = 0.0f;
				}

				*knockBackAmount = std::min(*knockBackAmount / 5, 1.0f); //Weaken and limit knockback

				//Change knockback hitstun to heavy
				if (*damageLevel == 4 || *damageLevel == 7 || *damageLevel == 10) {
					*damageLevel = 3;
				}
			}
			else if (areAllies) {
				return false;
			}

			for (uintptr_t i = 0; i < 8; i++) {
				const int32_t specialEffectId = *reinterpret_cast<int32_t*>(attackDataBuffer + 0x80 + i * 4ul);
				if (specialEffectId == -1 || specialEffectId == 0) continue;
			}

			ChrIns(receiverChr).applySpEffect(9400);
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
		break; //Disabled for testing
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

	recentlySentHitIds.erase(std::remove_if(recentlySentHitIds.begin(), recentlySentHitIds.end(), [&](auto recentHitRecord) -> bool {
		if (getNow() > recentHitRecord.timeStamp + hitTimeOutMillis) {
			return true;
		}
		else return false;
		}), recentlySentHitIds.end());
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

void DS3IFramePatch::registerOutgoingHitBox(uintptr_t hitBoxAddress, int32_t ownerHandle)
{
	outgoingHitBoxIdMap[hitBoxAddress] = { outgoingUniqueIdCounter, 0, 0, 0, ownerHandle};
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

std::optional<uintptr_t> DS3IFramePatch::getOutgoingHitBoxByHitList(uintptr_t hitList)
{
	for (auto hitboxRecordEntry : outgoingHitBoxIdMap) {
		auto hitListPtr = accessMultilevelPointer<uintptr_t>(hitboxRecordEntry.first + 0x270);
		
		if (hitListPtr == nullptr) continue;
		if (*hitListPtr == hitList) return hitboxRecordEntry.first;
	}

	return {};
}

uint32_t DS3IFramePatch::processIncomingPlayerHitPacket(uintptr_t networkSession, uintptr_t* networkHandle, int id, char* buffer, uint32_t maxLength, uint32_t receiveLength)
{
	const uint16_t hitBoxUniqueId = *reinterpret_cast<uint16_t*>(buffer + 0x78);
	const uint16_t parentHitBoxUniqueId = *reinterpret_cast<uint16_t*>(buffer + 0x7A);
	const uint16_t siblingHitBoxUniqueId = *reinterpret_cast<uint16_t*>(buffer + 0x84);
	const uint16_t childHitBoxUniqueId = *reinterpret_cast<uint16_t*>(buffer + 0x86);
	const uint64_t now = getNow();

	const ReceivedHitRecord hitRecord = {
		hitBoxUniqueId,
		parentHitBoxUniqueId,
		siblingHitBoxUniqueId,
		childHitBoxUniqueId,
		now,
	};

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

	//Checks InvokeThrowStart2 jmp table
	auto isInThrowInvulnerablity =
		reinterpret_cast<JmpTableStatus*>(ds3runtime_global->accessHook("jmp_table_status"))->getJmpTableStatus(67);
	SprjSessionManager sessionManager(SprjSessionManager::getInstance());
	
	//Check if players are allies, and if the hit is supposed to hit allies.
	PlayerIns attackerChr(sessionManager.getPlayerInsByNetworkHandle(networkHandle));
	const bool areAllies = IChrFinder::isBothChrAllies(mainChr.getAddress(), attackerChr.getAddress());
	ParamHandler atkParam("", L"AtkParam_Pc", *reinterpret_cast<int32_t*>(buffer + 0x44));

	if (!atkParam.isValidParam()) {
		ParamHandler bulletParam("", L"Bullet", *reinterpret_cast<int32_t*>(buffer + 0x44));
		if (!bulletParam.isValidParam()) return 0;
		atkParam = ParamHandler("", L"AtkParam_Pc", bulletParam.read<int32_t>(0x0));
	}

	const bool isHitEnemyTeam = atkParam.readBinary(0x81, 0);
	const bool isHitFriendlyTeam = atkParam.readBinary(0x81, 1);
	const bool throwFlag = atkParam.readBinary(0x7D, 0);
	
	if ((!isHitEnemyTeam && !areAllies)
		|| (!isHitFriendlyTeam && areAllies && (!settings.isFriendlyHitstun || throwFlag))) { //Check if meant to hit
		return 0;
	}

	if (!doesHaveHit
			&& !mainChr.isDodging()
			&& !isInThrowInvulnerablity) {
		ReceivedHitRecord hitRecord = {};
		hitRecord.uniqueId = hitBoxUniqueId;
		hitRecord.siblingUniqueId = siblingHitBoxUniqueId;
		hitRecord.parentUniqueId = parentHitBoxUniqueId;
		hitRecord.childUniqueId = childHitBoxUniqueId;
		hitRecord.timeStamp = now;
		incomingHitBoxIdMap[networkHandle].push_back(hitRecord);
		lastIncomingHitData = std::vector(buffer, buffer + receiveLength);
		return receiveLength;
	}
	else {
		return 0;
	}
}

uint32_t DS3IFramePatch::processIncomingHitVfxPacket(uintptr_t networkSession, 
	uintptr_t* networkHandle, 
	int id, 
	char* buffer, 
	uint32_t maxLength, 
	uint32_t receiveLength)
{
	if (!WorldChrMan::hasInstance() || !PlayerIns::isMainChrLoaded()) return maxLength;
	PlayerIns mainChr(PlayerIns::getMainChrAddress());
	int32_t hitChrId = *accessMultilevelPointer<int16_t>(buffer + 2);
	int32_t hitterChrId = *accessMultilevelPointer<int16_t>(buffer + 4);
	std::optional<PlayerIns> hitChr;

	if (hitterChrId != mainChr.getForwardId()) return maxLength; //We only want to check for hits that are done by the local player.

	for (int32_t i = 1; i <= 5; ++i) { //See if any player chrs have this forward id
		const uintptr_t chrAddress = 
			PlayerIns::getAddressByOffsetNumber(static_cast<PlayerIns::OffsetNumber>(i));

		if (PlayerIns::isPlayer(chrAddress) && PlayerIns(chrAddress).getForwardId() == hitChrId) {
			hitChr = chrAddress;
			break;
		}
	}

	if (!hitChr.has_value()) return maxLength;

	//Retrieve hitbox unique id from hit vfx packet
	const uint16_t hitBoxUniqueId = *reinterpret_cast<uint16_t*>(buffer + 0x30);
	auto it = recentlySentHitIds.begin();

	while (it != recentlySentHitIds.end()) {
		if (hitBoxUniqueId == it->hitBoxRecord.uniqueId) { //Recognized successful hit!
			processWeaponDurablityUse();

			for (int32_t spEffectId : it->iSpEffectIdOnHit) {
				if (spEffectId == -1 || spEffectId == 0) continue;
				ParamHandler effectParam("", L"SpEffectParam", spEffectId);
				if (!effectParam.isValidParam()) continue;

				if (!effectParam.readBinary(0x160, 1)) continue;
				mainChr.applySpEffect(spEffectId);
				spdlog::debug("Applied sp effect to self");
			}

			it = recentlySentHitIds.erase(it);
		} 
		else ++it;
	}

	return maxLength;
}

uint32_t DS3IFramePatch::processOutgoingPlayerHitPacket(uintptr_t playerNetworkSession, 
	uintptr_t* networkHandle, 
	int id, 
	char* buffer, 
	uint32_t maxLength)
{
	uintptr_t hitBoxStructAddress = dynamic_cast<HitUUIDIdentifyHook*>(ds3runtime_global->accessHook("ds3_iframe_patch_hit_uuid_identify"))->getLastHitBoxAddress();

	if (outgoingHitBoxIdMap.contains(hitBoxStructAddress)) {
		const uint16_t uniqueId = outgoingHitBoxIdMap[hitBoxStructAddress].uniqueId;
		const uint16_t siblingUniqueId = outgoingHitBoxIdMap[hitBoxStructAddress].siblingUniqueId;
		const uint16_t childUniqueId = outgoingHitBoxIdMap[hitBoxStructAddress].childUniqueId;
		const uint16_t parentUniqueId = outgoingHitBoxIdMap[hitBoxStructAddress].parentUniqueId;

		for (auto id : hitsToIgnoreThisFrame) {
			if (id == uniqueId || id == siblingUniqueId || id == childUniqueId || id == parentUniqueId) return 0;
		}

		hitsToIgnoreThisFrame.push_back(uniqueId);
		std::array<int32_t, 8> iSpEffectIdOnHit = {};
		memcpy(&iSpEffectIdOnHit[0], buffer + 0x4C, 8 * 4);
		recentlySentHitIds.push_back({ outgoingHitBoxIdMap[hitBoxStructAddress], iSpEffectIdOnHit, getNow() });
		memcpy(reinterpret_cast<uint16_t*>(buffer + 0x78), &uniqueId, 2);
		memcpy(reinterpret_cast<uint16_t*>(buffer + 0x7A), &parentUniqueId, 2);
		memcpy(reinterpret_cast<uint16_t*>(buffer + 0x84), &siblingUniqueId, 2);
		memcpy(reinterpret_cast<uint16_t*>(buffer + 0x86), &childUniqueId, 2);
	}

	spdlog::debug("Sending attack!");

	return maxLength;
}

uint32_t DS3IFramePatch::processOutgoingHitVfxPacket(uintptr_t playerNetworkSession, 
	uintptr_t* networkHandle, 
	int id, 
	char* buffer, 
	uint32_t maxLength)
{
	if (!lastIncomingHitData.has_value() || !WorldChrMan::hasInstance() || !PlayerIns::isMainChrLoaded()) return maxLength;
	PlayerIns mainChr(PlayerIns::getMainChrAddress());
	int32_t hitChrId = *accessMultilevelPointer<int16_t>(buffer + 2);
	int32_t hitterChrId = *accessMultilevelPointer<int16_t>(buffer + 4);
	std::optional<PlayerIns> hitterChr;

	if (hitChrId != mainChr.getForwardId()) return maxLength;

	for (int32_t i = 1; i <= 5; ++i) {
		const uintptr_t chrAddress = 
			PlayerIns::getAddressByOffsetNumber(static_cast<PlayerIns::OffsetNumber>(i));

		if (PlayerIns::isPlayer(chrAddress) && PlayerIns(chrAddress).getForwardId() == hitterChrId) {
			hitterChr = chrAddress;
			break;
		}
	}

	if (!hitterChr.has_value() || lastIncomingHitData->size() < 0xB0) return maxLength;

	//Insert unique id into hit vfx packet
	*reinterpret_cast<uint16_t*>(buffer + 0x30) = *reinterpret_cast<uint16_t*>(&lastIncomingHitData.value()[0] + 0x78);

	return maxLength;
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

void DS3IFramePatch::processWeaponDurablityUse()
{
	if (!WorldChrMan::hasInstance() || !PlayerIns::isMainChrLoaded()) return;
	PlayerIns mainChr(PlayerIns::getMainChrAddress());
	void(*UseWeaponDurablity)(...);
	*(uintptr_t*)&UseWeaponDurablity = *accessMultilevelPointer<uintptr_t>(mainChr.getAddress() + 0x320);
	UseWeaponDurablity(mainChr.getAddress(), -1, 0);
}

uint64_t DS3IFramePatch::getNow()
{
	return std::chrono::duration_cast<std::chrono::milliseconds>(
					std::chrono::system_clock::now().time_since_epoch()).count();
}

}