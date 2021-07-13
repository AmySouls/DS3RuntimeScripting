/*
	* DS3RuntimeScripting
	* Contributers: Amir
*/

#pragma once
#include "pch.h"
#include "ds3runtime/async_module.h"
#include "ds3runtime/game_packet.h"
#include "ds3runtime/hooks/session_send_hook.h"

namespace ds3runtime {

struct LatencyAttributes {
	uint64_t flatOutLatency;
	uint64_t flatInLatency;
	uint64_t jitterOut;
	uint64_t jitterIn;
	float dropRateOut;
	float dropRateIn;
};

struct DelayedPacket {
	uint64_t timeToSend;
	uintptr_t* networkHandle;
	std::optional<packet::Packet> packet;
};

class LatencySimulator : public ScriptModule
{
public:
	LatencySimulator(LatencyAttributes latencyAttributes);

	bool onAttach();

	bool onDetach();

	void execute();

	std::string getName()
	{
		return "latency_simulator";
	}
private:
	std::vector<DelayedPacket> delayedPackets;
	LatencyAttributes latencyAttributes;
};

}