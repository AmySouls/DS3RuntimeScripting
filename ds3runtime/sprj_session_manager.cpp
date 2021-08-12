#pragma once
#include "pch.h"
#include "sprj_session_manager.h"
#include "databaseaddress.h"
#include "player_ins.h"

namespace ds3runtime {

SprjSessionManager::SprjSessionManager(uintptr_t address)
{
	this->address = address;
}

bool SprjSessionManager::areHandlesEqual(uintptr_t* networkHandle, uintptr_t* networkHandleOther)
{
	return *networkHandle == *networkHandleOther;
}

bool SprjSessionManager::isValidNetworkHandle(uintptr_t* networkHandle)
{
	char**** handle;
	handle = (char****)(*(char**)(address + 0x18) + 0x68);

	if ((uintptr_t)handle > 0x7FF000000000) for (uintptr_t i = 0; handle[0] && handle[0] + i < handle[1]; ++i) {
		if (!IsBadReadPtr(handle[0][i], 8) && handle[0][i][13] == *(char**)networkHandle) return true;
	}

	return false;
}

uint64_t SprjSessionManager::getSteamIdByNetworkHandle(uintptr_t* networkHandle)
{
	char**** handle;
	handle = (char****)(*(char**)(address + 0x18) + 0x68);

	if ((uintptr_t)handle > 0x7FF000000000) for (uintptr_t i = 0; handle[0] && handle[0] + i < handle[1]; ++i) {
		if (!IsBadReadPtr(handle[0][i], 8) && handle[0][i][13] == *(char**)networkHandle) return (uint64_t)handle[0][i][25];
	}

	return 0;
}

void SprjSessionManager::disconnectNetworkHandle(uintptr_t* networkHandle)
{
	char**** handle;
	handle = (char****)(*(char**)(address + 0x18) + 0x68);

	if ((uintptr_t)handle > 0x7FF000000000) for (uintptr_t i = 0; handle[0] && handle[0] + i < handle[1]; ++i) {
		if (!IsBadReadPtr(handle[0][i], 8) && handle[0][i][13] == *(char**)networkHandle) {
			*(int32_t*)&handle[0][i][11] = 0;
			uintptr_t baseC = *(uintptr_t*)DataBaseAddress::BaseC;
			int32_t realPlayerInsCount = 0;
			for (int i = 1; i <= 5; i++) if (PlayerIns::getAddressByOffsetNumber((PlayerIns::OffsetNumber)i)) realPlayerInsCount++;
			int32_t* playerInsCount = (int32_t*)(baseC + 0xD38);
			if (*playerInsCount > realPlayerInsCount) *playerInsCount = realPlayerInsCount;
		}
	}
}

std::vector<uintptr_t*> SprjSessionManager::getNetworkHandles()
{
	std::vector<uintptr_t*> handles;
	char**** handle;
	handle = (char****)(*(char**)(address + 0x18) + 0x68);

	if ((uintptr_t)handle > 0x7FF000000000) for (uintptr_t i = 0; handle[0] && handle[0] + i < handle[1]; ++i) {
		if (!IsBadReadPtr(handle[0][i], 8)) handles.push_back((uintptr_t*)&handle[0][i][13]);
	}

	return handles;
}

bool SprjSessionManager::anyActiveConnections()
{
	char**** handle;
	handle = (char****)(*(char**)(address + 0x18) + 0x68);

	if ((uintptr_t)handle > 0x7FF000000000) for (uintptr_t i = 1; handle[0] && handle[0] + i < handle[1]; ++i) {
		if (!IsBadReadPtr(handle[0][i], 8) && *(int32_t*)&handle[0][i][11] != 0) return true;
	}

	return false;
}

uintptr_t SprjSessionManager::getPlayerInsByNetworkHandle(uintptr_t* networkHandle)
{
	uintptr_t(*GetChrInsByNetworkHandle)(...);
	*(uintptr_t*)&GetChrInsByNetworkHandle = 0x1408D11A0;
	return GetChrInsByNetworkHandle(*(uintptr_t*)DataBaseAddress::WorldChrMan, networkHandle);
}

void SprjSessionManager::leaveSession()
{
	uintptr_t(*LeaveSessionCall)(uintptr_t sessionManager);
	*(uintptr_t*)&LeaveSessionCall = 0x140DEB7F0;
	LeaveSessionCall(address);
}

SprjSessionManager::NetworkState SprjSessionManager::getState()
{
	return (SprjSessionManager::NetworkState)*accessMultilevelPointer<uint8_t>(address + 0x16C);
}

uintptr_t SprjSessionManager::getPlayerGameDataByNetworkHandle(uintptr_t* networkHandle)
{
	
	uintptr_t(*FunctionCall)(uintptr_t* networkHandle);
	*(uintptr_t*)&FunctionCall = 0x1405951f0;
	return FunctionCall(networkHandle);
}

uintptr_t SprjSessionManager::getInstance()
{
	return *accessMultilevelPointer<uintptr_t>(DataBaseAddress::SprjSessionManager);
}

bool SprjSessionManager::hasInstance()
{
	return accessMultilevelPointer<uintptr_t>(DataBaseAddress::SprjSessionManager) != nullptr;
}

};