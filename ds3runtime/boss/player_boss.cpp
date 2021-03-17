#pragma once
#include "pch.h"
#include "player_boss.h"

namespace ds3runtime {

PlayerBoss::PlayerBoss(PlayerIns playerIns) : Boss(playerIns)
{
	savedEquipment = std::array<int32_t, 47>();
}

}