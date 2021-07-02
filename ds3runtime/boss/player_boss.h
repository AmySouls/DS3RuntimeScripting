/*
	* DS3RuntimeScripting
	* Contributers: Amir
*/

#pragma once
#include "pch.h"
#include "boss.h"
#include "ds3runtime/player_ins.h"

namespace ds3runtime {

class PlayerBoss : public Boss
{
public:
	PlayerBoss(std::shared_ptr<PlayerIns> playerIns);
private:
protected:
};

}