/*
	* DS3RuntimeScripting
	* Contributers: Amir
	* Further notes on how to use this class:
*/

#pragma once
#include "pch.h"
#include "memory_util.h"

namespace ds3runtime {

class PlayerNetworkSession
{
public:
	PlayerNetworkSession(uintptr_t address);

	uintptr_t* getNetworkHandleByOffset(uintptr_t offsetNumber);

	uintptr_t getPlayerInsByNetworkHandle(uintptr_t* networkHandle);

	static uintptr_t getInstance();

	static bool hasInstance();
private:
	uintptr_t address;

	void debugPacketSend(uintptr_t* networkHandle, int32_t packetId, char* packetBuffer, uint32_t packetSize);

	void sessionPacketSend(int32_t packetId, char* packetBuffer, uint32_t packetSize);
};

}