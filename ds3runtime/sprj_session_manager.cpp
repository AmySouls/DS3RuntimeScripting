#pragma once
#include "pch.h"
#include "sprj_session_manager.h"
#include "databaseaddress.h"

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
	spdlog::debug("Handle: {}", (void*)handle);
	if ((uintptr_t)handle > 0x7FF000000000) spdlog::debug("Handle 0: {} {}", (void*)handle[0], (void*)handle[1]);

	if ((uintptr_t)handle > 0x7FF000000000) for (uintptr_t i = 0; handle[0] && handle[0] + i < handle[1]; ++i) {
		spdlog::debug("inner: {}", (void*)handle[0][i]);
		if (!IsBadReadPtr(handle[0][i], 8) && handle[0][i][13] == *(char**)networkHandle) return true;
	}

	return false;
}

uint64_t SprjSessionManager::getSteamIdByNetworkHandle(uintptr_t* networkHandle)
{
	char**** handle;
	handle = (char****)(*(char**)(address + 0x18) + 0x68);
	spdlog::debug("Handle: {}", (void*)handle);
	if ((uintptr_t)handle > 0x7FF000000000) spdlog::debug("Handle 0: {} {}", (void*)handle[0], (void*)handle[1]);

	if ((uintptr_t)handle > 0x7FF000000000) for (uintptr_t i = 0; handle[0] && handle[0] + i < handle[1]; ++i) {
		spdlog::debug("inner: {}", (void*)handle[0][i]);
		if (!IsBadReadPtr(handle[0][i], 8) && handle[0][i][13] == *(char**)networkHandle) return (uint64_t)handle[0][i][25];
	}

	return 0;
}

std::vector<uintptr_t*> SprjSessionManager::getNetworkHandles()
{
	std::vector<uintptr_t*> handles;
	char**** handle;
	handle = (char****)(*(char**)(address + 0x18) + 0x68);
	spdlog::debug("Handle: {}", (void*)handle);
	if ((uintptr_t)handle > 0x7FF000000000) spdlog::debug("Handle 0: {}", (void*)handle[0], (void*)handle[1]);

	if ((uintptr_t)handle > 0x7FF000000000) for (uintptr_t i = 0; handle[0] && handle[0] + i < handle[1]; ++i) {
		spdlog::debug("inner: {}", (void*)handle[0][i]);
		if (!IsBadReadPtr(handle[0][i], 8)) handles.push_back((uintptr_t*)&handle[0][i][13]);
	}

	return handles;
}

uintptr_t SprjSessionManager::getPlayerInsByNetworkHandle(uintptr_t* networkHandle)
{
	uintptr_t(*GetChrInsByNetworkHandle)(...);
	*(uintptr_t*)&GetChrInsByNetworkHandle = 0x1408D11A0;
	return GetChrInsByNetworkHandle(*(uintptr_t*)DataBaseAddress::WorldChrMan, networkHandle);
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