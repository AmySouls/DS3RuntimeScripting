/*
	* DS3RuntimeScripting
	* Contributers: Amir
*/

#pragma once
#include "pch.h"
#include "ds3runtime/script_module.h"
#include "ds3runtime/ds3runtime.h"

namespace ds3runtime::ds3IFramePatch {

struct HitBoxRecord
{
	int64_t uniqueId;
	int64_t parentUniqueId;
	int64_t siblingUniqueId;
	int64_t childUniqueId;
};

struct ReceivedHitRecord
{
	int64_t uniqueId;
	int64_t parentUniqueId;
	int64_t siblingUniqueId;
	int64_t childUniqueId;
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

	void registerOutgoingHitBox(uintptr_t hitBoxAddress);

	void unregisterOutgoingHitBox(uintptr_t hitBoxAddress);

	void registerOutgoingHitBoxParent(uintptr_t hitBoxAddress, int64_t parentUniqueId);

	void registerOutgoingHitBoxChild(uintptr_t hitBoxAddress, int64_t childUniqueId);

	void registerOutgoingHitBoxSibling(uintptr_t hitBoxAddress, int64_t siblingUniqueId);

	bool hasOutgoingHitBox(uintptr_t hitBoxAddress);

	HitBoxRecord getOutgoingHitBoxRecord(uintptr_t hitBoxAddress);
private:
	int64_t outgoingUniqueIdCounter = 0;
	std::unordered_map<uintptr_t, HitBoxRecord> outgoingHitBoxIdMap;
	std::unordered_map<uintptr_t*, std::vector<ReceivedHitRecord>> incomingHitBoxIdMap;
	std::vector<int64_t> hitsToIgnoreThisFrame;

	bool doesReceivedHitShareUniqueId(ReceivedHitRecord hitRecord, ReceivedHitRecord otherHitRecord);

	bool doesReceivedHitContainUniqueId(ReceivedHitRecord hitRecord, int64_t searchUniqueId);
};

}