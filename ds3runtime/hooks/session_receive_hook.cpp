/*
	* DS3RuntimeScripting
	* Contributers: Amir
*/

#pragma once
#include "pch.h"
#include "session_receive_hook.h"
#include "ds3runtime/ds3runtime.h"
#include "spdlog/stopwatch.h"
#include <spdlog/fmt/bin_to_hex.h>

namespace ds3runtime {

SessionReceiveHook::SessionReceiveHook()
	: Hook(0x140787140, (uintptr_t)onPlayerNetworkSessionReceive)
{
	instance = this;
}

uint32_t SessionReceiveHook::onPlayerNetworkSessionReceive(uintptr_t networkSession, uintptr_t* networkHandle, int32_t id, char* buffer, uint32_t maxLength)
{
	uint32_t(*originalFunction)(uintptr_t playerNetworkSession, uintptr_t* networkHandle, int id, char* buffer, uint32_t maxLength);
	*(uintptr_t*)&originalFunction = *instance->original;
	uint32_t receiveLength = originalFunction(networkSession, networkHandle, id, buffer, maxLength);
	if (receiveLength == 0) return 0;
	std::lock_guard<std::mutex> lock(instance->mut);

	for (auto filter : instance->packetFilters) {
		receiveLength = filter.second(networkSession, networkHandle, id, buffer, maxLength, receiveLength);
		if (receiveLength == 0) return 0;
	}

	return receiveLength;
}

void SessionReceiveHook::installPacketFilter(std::string key, SessionReceiveHookPacketFilter function)
{
	std::lock_guard<std::mutex> lock(mut);
	packetFilters[key] = function;
	cond.notify_one();
}

void SessionReceiveHook::uninstallPacketFilter(std::string key)
{
	std::lock_guard<std::mutex> lock(mut);
	packetFilters.erase(key);
	cond.notify_one();
}

SessionReceiveHook* SessionReceiveHook::instance = nullptr;

}