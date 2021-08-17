#pragma once
#include "pch.h"
#include <ds3runtime/hooks/session_receive_hook.h>
#include <ds3runtime/hooks/session_send_hook.h>
#include <ds3runtime/scripts/mod_p2p_filter.h>

namespace ds3runtime {

bool ModP2PFilter::onAttach()
{
	((SessionReceiveHook*)ds3runtime_global
		->accessHook("session_receive_hook"))->installPacketFilter("mod_p2p_filter", [&](uintptr_t networkSession, uintptr_t* networkHandle, int id, char* buffer, uint32_t maxLength, uint32_t receiveLength) -> uint32_t {
			if (id != 73) return receiveLength;

			if (*(uint16_t*)(buffer + 1) == modKey) {
				*(uint16_t*)(buffer + 1) = 0;
				*(uint8_t*)buffer = *(uint8_t*)(buffer + 3);
			}
			else { 
				*(uint8_t*)buffer = 1;
			}

			return receiveLength;
			});

	((SessionSendHook*)ds3runtime_global
		->accessHook("session_send_hook"))->installPacketFilter("mod_p2p_filter", [&](uintptr_t playerNetworkSession, uintptr_t* networkHandle, int id, char* buffer, uint32_t maxLength) -> uint32_t {
			if (id != 73) return maxLength;
			*(uint16_t*)(buffer + 1) = modKey;
			*(uint8_t*)(buffer + 3) = *(uint8_t*)buffer;
			*(uint8_t*)buffer = 1;
			return maxLength;
			});
	
	return true;
}

void ModP2PFilter::execute()
{
}

bool ModP2PFilter::onDetach()
{
	((SessionReceiveHook*)ds3runtime_global
		->accessHook("session_receive_hook"))
		->uninstallPacketFilter("mod_p2p_filter");
	((SessionReceiveHook*)ds3runtime_global
		->accessHook("session_send_hook"))
		->uninstallPacketFilter("mod_p2p_filter");
	return true;
}

}