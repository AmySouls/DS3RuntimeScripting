/*
	* DS3RuntimeScripting
	* Contributers: Amir
*/

#pragma once
#include "pch.h"
#include "ds3runtime/script_module.h"
#include "ds3runtime/ds3runtime.h"

namespace ds3runtime::ds3IFramePatch {

struct Settings
{
	bool isFriendlyHitstun = true;
	float maxHitTime = 1.5;
};

struct HitBoxRecord
{
	uint16_t uniqueId;
	uint16_t parentUniqueId;
	uint16_t siblingUniqueId;
	uint16_t childUniqueId;
	int32_t ownerHandle;
};

struct ReceivedHitRecord
{
	uint16_t uniqueId;
	uint16_t parentUniqueId;
	uint16_t siblingUniqueId;
	uint16_t childUniqueId;
	uint64_t timeStamp;
};

struct RecentHitRecord
{
	HitBoxRecord hitBoxRecord;
	std::array<int32_t, 8> iSpEffectIdOnHit;
	uint64_t timeStamp;
};

class DS3IFramePatch : public ScriptModule
{
public:
	bool onAttach();

	bool onDetach();

	void execute();

	std::string getName()
	{
		return "ds3_iframe_patch";
	}

	void registerOutgoingHitBox(uintptr_t hitBoxAddress, int32_t ownerHandle);

	void unregisterOutgoingHitBox(uintptr_t hitBoxAddress);

	void registerOutgoingHitBoxParent(uintptr_t hitBoxAddress, uint16_t parentUniqueId);

	void registerOutgoingHitBoxChild(uintptr_t hitBoxAddress, uint16_t childUniqueId);

	void registerOutgoingHitBoxSibling(uintptr_t hitBoxAddress, uint16_t siblingUniqueId);

	bool hasOutgoingHitBox(uintptr_t hitBoxAddress);

	HitBoxRecord getOutgoingHitBoxRecord(uintptr_t hitBoxAddress);

	std::optional<uintptr_t> getOutgoingHitBoxByHitList(uintptr_t hitList);
private:
	Settings settings;
	uint16_t outgoingUniqueIdCounter = 0;
	std::unordered_map<uintptr_t, HitBoxRecord> outgoingHitBoxIdMap;
	std::unordered_map<uintptr_t, std::vector<int32_t>> outgoingHitBoxCosmeticHitList;
	std::unordered_map<uintptr_t*, std::vector<ReceivedHitRecord>> incomingHitBoxIdMap;
	std::vector<uint16_t> hitsToIgnoreThisFrame;
	std::vector<RecentHitRecord> recentlySentHitIds;
	std::optional<std::vector<char>> lastIncomingHitData;
	const uint64_t hitTimeOutMillis = 20000;

	uint32_t processIncomingPlayerHitPacket(uintptr_t networkSession, uintptr_t* networkHandle, int id, char* buffer, uint32_t maxLength, uint32_t receiveLength);

	uint32_t processIncomingHitVfxPacket(uintptr_t networkSession, uintptr_t* networkHandle, int id, char* buffer, uint32_t maxLength, uint32_t receiveLength);

	uint32_t processOutgoingPlayerHitPacket(uintptr_t playerNetworkSession, uintptr_t* networkHandle, int id, char* buffer, uint32_t maxLength);

	uint32_t processOutgoingHitVfxPacket(uintptr_t playerNetworkSession, uintptr_t* networkHandle, int id, char* buffer, uint32_t maxLength);

	bool doesReceivedHitShareUniqueId(ReceivedHitRecord hitRecord, ReceivedHitRecord otherHitRecord);

	bool doesReceivedHitContainUniqueId(ReceivedHitRecord hitRecord, uint16_t searchUniqueId);

	void processWeaponDurablityUse();

	uint64_t getNow();
};

}