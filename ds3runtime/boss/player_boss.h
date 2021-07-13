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
	PlayerBoss(uint16_t forwardId);

	std::optional<uintptr_t> getChrAddress();
private:
protected:
};

}