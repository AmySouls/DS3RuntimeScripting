/*
	* DS3RuntimeScripting
	* Contributers: Amir
*/

#pragma once
#include "pch.h"

namespace ds3runtime {

class GameDataMan
{
public:
	GameDataMan(uintptr_t address);

	uintptr_t getPlayerGameData();

	int32_t getClearCount();

	void setClearCount(int32_t clearCount);

	int32_t getRescueWhite(); //Succesful co-op?

	int32_t getKillBlack(); //Dark spirit kills

	int32_t getDeathNum();

	uint32_t getPlayTime();

	static uintptr_t getInstance();

	static bool hasInstance();
private:
	uintptr_t address;
};

}