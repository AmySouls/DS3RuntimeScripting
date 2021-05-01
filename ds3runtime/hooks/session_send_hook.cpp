/*
	* DS3RuntimeScripting
	* Contributers: Amir
*/

#pragma once
#include "pch.h"
#include "session_send_hook.h"
#include "ds3runtime/ds3runtime.h"
#include <spdlog/fmt/bin_to_hex.h>

namespace ds3runtime {

SessionSendHook::SessionSendHook()
	: Hook(0x1407875D0, (uintptr_t)onPlayerNetworkSessionDebugSend)
{
	instance = this;
}

uint32_t SessionSendHook::onPlayerNetworkSessionDebugSend(uintptr_t networkSession, uintptr_t* networkHandle, int32_t id, char* buffer, uint32_t maxLength)
{
	uint32_t(*originalFunction)(uintptr_t playerNetworkSession, uintptr_t* networkHandle, int id, char* buffer, uint32_t maxLength);
	*(uintptr_t*)&originalFunction = *instance->original;

	for (auto filter : instance->packetFilters) {
		maxLength = filter.second(networkSession, networkHandle, id, buffer, maxLength);
		if (maxLength == 0) return 0;
	}

	return originalFunction(networkSession, networkHandle, id, buffer, maxLength);
}

void SessionSendHook::installPacketFilter(std::string key, SessionSendHookPacketFilter function)
{
	packetFilters[key] = function;
}

void SessionSendHook::uninstallPacketFilter(std::string key)
{
	packetFilters.erase(key);
}

SessionSendHook* SessionSendHook::instance = nullptr;

}