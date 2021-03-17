/*
	* DS3RuntimeScripting
	* Contributers: Amir
*/

#pragma once
#include "pch.h"
#include "ds3runtime/async_module.h"
#include "ds3runtime/packet.h"
#include "ds3runtime/hooks/session_send_hook.h"

namespace ds3runtime {

struct DelayedPacket {
	uint64_t timeToSend;
	uintptr_t* networkHandle;
	std::optional<packet::Packet> packet;
};

class LatencySimulator : public ScriptModule
{
public:
	void onAttach();

	void onDetach();

	void execute();

	std::string getName()
	{
		return "test_script";
	}
private:
	std::vector<DelayedPacket> delayedPackets;
	const uint64_t millisToDelay = 200;
};

}