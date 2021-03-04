/*
	* DS3RuntimeScripting
	* Contributers: Amir
*/

#pragma once
#include "pch.h"
#include "ds3runtime/hook.h"
#include "ds3runtime/player_network_session.h"

namespace ds3runtime {

typedef void (*playerNetworkSessionReceive)(uintptr_t networkSession, uintptr_t* networkHandle, int32_t id, char* buffer, uint32_t maxLength);

class SessionReceiveHook : public Hook
{
public:
	SessionReceiveHook();

	static uint32_t onPlayerNetworkSessionReceive(uintptr_t networkSession, uintptr_t* networkHandle, int32_t id, char* buffer, uint32_t maxLength);

	std::string getName()
	{
		return "session_receive_hook";
	}

	void installPacketFilter(std::string key, std::function<boolean(uintptr_t, uintptr_t*, int32_t, char*, uint32_t)> function);

	void uninstallPacketFilter(std::string key);
private:
	std::unordered_map<std::string, std::function<boolean(uintptr_t, uintptr_t*, int32_t, char*, uint32_t)>> packetFilters;

	static SessionReceiveHook* instance;
};

}