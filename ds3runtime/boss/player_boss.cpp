#pragma once
#include "pch.h"
#include "player_boss.h"

namespace ds3runtime {

PlayerBoss::PlayerBoss(std::shared_ptr<PlayerIns> playerIns) : Boss(playerIns)
{
}

}