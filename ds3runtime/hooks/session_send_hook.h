/*
	* DS3RuntimeScripting
	* Contributers: Amir
*/

#pragma once
#include "pch.h"
#include "ds3runtime/hook.h"
#include "ds3runtime/player_network_session.h"

namespace ds3runtime {

typedef std::function<uint32_t(uintptr_t, uintptr_t*, int32_t, char*, uint32_t)> SessionSendHookPacketFilter;

class SessionSendHook : public Hook
{
public:
	SessionSendHook();

	std::string getName()
	{
		return "session_send_hook";
	}

	void installPacketFilter(std::string key, SessionSendHookPacketFilter function);

	void uninstallPacketFilter(std::string key);

	static uint32_t onPlayerNetworkSessionDebugSend(uintptr_t networkSession, uintptr_t* networkHandle, int32_t id, char* buffer, uint32_t maxLength);
private:
	std::unordered_map<std::string, SessionSendHookPacketFilter> packetFilters;
	std::mutex mut;
	std::condition_variable cond;

	static SessionSendHook* instance;
};

}