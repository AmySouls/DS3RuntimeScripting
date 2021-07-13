#pragma once
#include "pch.h"
#include "player_boss.h"

namespace ds3runtime {

PlayerBoss::PlayerBoss(uint16_t forwardId) : Boss(forwardId)
{
}

std::optional<uintptr_t> PlayerBoss::getChrAddress()
{
	for (int i = 0; i <= 5; i++) {
		const uintptr_t address = PlayerIns::getAddressByOffsetNumber((PlayerIns::OffsetNumber)i);
		if (address && PlayerIns(address).getForwardId() == getForwardId()) return address;
	}

	return {};
}

}