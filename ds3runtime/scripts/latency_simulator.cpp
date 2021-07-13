/*
	* DS3RuntimeScripting
	* Contributers: Amir
*/

#pragma once
#include "pch.h"
#include "latency_simulator.h"
#include "ds3runtime/ds3runtime.h"
#include "ds3runtime/sprj_session_manager.h"
#include <random>

namespace ds3runtime {

LatencySimulator::LatencySimulator(LatencyAttributes latencyAttributes)
{
	this->latencyAttributes = latencyAttributes;
}

bool LatencySimulator::onAttach()
{
	auto sessionSendHook = (SessionSendHook*)ds3runtime_global->accessHook("session_send_hook");

	sessionSendHook->installPacketFilter("latency_simulator", [&](uintptr_t networkSession, uintptr_t* networkHandle, int32_t id, char* buffer, uint32_t maxLength) {
			DelayedPacket delayedPacket = {};
			delayedPacket.timeToSend = std::chrono::duration_cast<std::chrono::milliseconds>(
				std::chrono::system_clock::now().time_since_epoch()).count() + latencyAttributes.flatOutLatency;
			std::random_device rd;
			std::mt19937 gen(rd());
			std::uniform_real_distribution<> dis(0.0, latencyAttributes.jitterOut);
			delayedPacket.timeToSend += (uint64_t)dis(gen);
			delayedPacket.networkHandle = networkHandle;
			delayedPacket.packet = packet::Packet(id, buffer, maxLength);
			if (delayedPackets[delayedPackets.size() - 1].timeToSend >= delayedPacket.timeToSend) 
				delayedPacket.timeToSend = delayedPackets[delayedPackets.size() - 1].timeToSend + 1;
			delayedPackets.push_back(delayedPacket);
			return 0;
		});

	return true;
}

bool LatencySimulator::onDetach()
{
	auto sessionSendHook = (SessionSendHook*)ds3runtime_global->accessHook("session_send_hook");
	sessionSendHook->uninstallPacketFilter("latency_simulator");
	return true;
}

void LatencySimulator::execute()
{
	delayedPackets.erase(std::remove_if(delayedPackets.begin(), delayedPackets.end(), [&](auto delayedPacket) {
			const uint64_t now = std::chrono::duration_cast<std::chrono::milliseconds>(
				std::chrono::system_clock::now().time_since_epoch()).count();
			if (now < delayedPacket.timeToSend) return false;
			if (!PlayerNetworkSession::hasInstance() || !SprjSessionManager::hasInstance()) return true;
			auto session = PlayerNetworkSession(PlayerNetworkSession::getInstance());
			auto sessionManager = SprjSessionManager(SprjSessionManager::getInstance());
			if (!delayedPacket.packet.has_value() || !sessionManager.isValidNetworkHandle(delayedPacket.networkHandle)) return true;
			session.packetSend(delayedPacket.networkHandle, &*delayedPacket.packet);
			return true;
		}), delayedPackets.end());
}

}