/*
	* DS3RuntimeScripting
	* Contributers: Amir
	* 
	* Use this api's network utilites in your scripts to ensure intended behaivor and utilize possible work arounds.
*/

#pragma once
#include "memory_util.h"
#include "databaseaddress.h"
#include "player.h"

namespace ds3runtime {

class MainCharacter : public Player
{
public:
	MainCharacter(uintptr_t address);

	static uintptr_t getMainCharacterAddress();

	static bool isMainCharacter(uintptr_t address);
private:
};

}