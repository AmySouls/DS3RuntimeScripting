/*
	* DS3RuntimeScripting
	* Contributers: Amir
*/

#pragma once
#include "pch.h"
#include "ds3runtime/script_module.h"
#include "ds3runtime/ds3runtime.h"

namespace ds3runtime::ds3IFramePatch {

struct NetHitMetaInfo
{
	uint64_t uniqueId;
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
private:
	uint64_t outgoingUniqueIdCounter = 0;
	std::unordered_map<uintptr_t, uint64_t> outgoingHitBoxIdMap;
	std::unordered_map<uintptr_t*, std::vector<NetHitMetaInfo>> incomingHitBoxIdMap;
	std::vector<uint64_t> hitsToIgnoreThisFrame;
};

}