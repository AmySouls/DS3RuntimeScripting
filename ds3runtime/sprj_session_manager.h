/*
	* DS3RuntimeScripting
	* Contributers: Amir
	* Further notes on how to use this class:
*/

#pragma once
#include "pch.h"
#include "memory_util.h"

namespace ds3runtime {

class SprjSessionManager
{
public:
	SprjSessionManager(uintptr_t address);

	bool areHandlesEqual(uintptr_t* networkHandle, uintptr_t* networkHandleOther);

	bool isValidNetworkHandle(uintptr_t* networkHandle);

	std::vector<uintptr_t*> getNetworkHandles();

	uintptr_t getPlayerInsByNetworkHandle(uintptr_t* networkHandle);

	uint64_t getSteamIdByNetworkHandle(uintptr_t* networkHandle);

	static uintptr_t getInstance();

	static bool hasInstance();
private:
	uintptr_t address;
};

}