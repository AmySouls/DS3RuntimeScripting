#pragma once
#include "pch.h"
#include "player_network_session.h"
#include "databaseaddress.h"

namespace ds3runtime {

PlayerNetworkSession::PlayerNetworkSession(uintptr_t address)
{
	this->address = address;
}

uintptr_t PlayerNetworkSession::getInstance()
{
	return *accessMultilevelPointer<uintptr_t>(0x14474CFF8);
}

bool PlayerNetworkSession::hasInstance()
{
	return accessMultilevelPointer<uintptr_t>(0x14474CFF8) != nullptr;
}

void PlayerNetworkSession::debugPacketSend(uintptr_t* networkHandle, int32_t packetId, char* packetBuffer, uint32_t packetSize)
{
	void(*DebugPacketSend)(...);
	*(uintptr_t*)&DebugPacketSend = 0x1407875D0;
	DebugPacketSend(address, packetId, packetBuffer, packetSize);
}

void PlayerNetworkSession::sessionPacketSend(int32_t packetId, char* packetBuffer, uint32_t packetSize)
{
	void(*SessionPacketSend)(...);
	*(uintptr_t*)&SessionPacketSend = 0x140787870;
	SessionPacketSend(address, packetId, packetBuffer, packetSize);
}

uintptr_t* PlayerNetworkSession::getNetworkHandleByOffset(uintptr_t offsetNumber)
{
	return nullptr;
}

uintptr_t PlayerNetworkSession::getPlayerInsByNetworkHandle(uintptr_t* networkHandle)
{
	uintptr_t(*SessionPacketSend)(...);
	*(uintptr_t*)&SessionPacketSend = 0x1408D11A0;
	return SessionPacketSend(*(uintptr_t*)getDataBaseAddress(DataBaseAddress::WorldChrMan), networkHandle);
}

};