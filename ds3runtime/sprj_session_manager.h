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
	enum class NetworkState : uint8_t
	{
		None = 0,
		TrytoCreateSession = 1,
		FailedToCreateSessio =2,
		Host = 3,
		TrytoJoinSession = 4,
		FailedToJoinSession = 5,
		Client = 6,
		OnLeaveSession = 7,
		FailedToLeaveSession = 8
	};

	SprjSessionManager(uintptr_t address);

	NetworkState getState();

	bool areHandlesEqual(uintptr_t* networkHandle, uintptr_t* networkHandleOther);

	bool isValidNetworkHandle(uintptr_t* networkHandle);

	void disconnectNetworkHandle(uintptr_t* networkHandle);

	std::vector<uintptr_t*> getNetworkHandles();

	bool anyActiveConnections();

	uintptr_t getPlayerInsByNetworkHandle(uintptr_t* networkHandle);

	uint64_t getSteamIdByNetworkHandle(uintptr_t* networkHandle);

	void leaveSession();

	static uintptr_t getPlayerGameDataByNetworkHandle(uintptr_t* networkHandle);

	static uintptr_t getInstance();

	static bool hasInstance();
private:
	uintptr_t address;
};

}