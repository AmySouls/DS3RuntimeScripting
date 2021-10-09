/*
* DS3RuntimeScripting
* Contributers: Amir
*/

#pragma once
#include "pch.h"

namespace ds3runtime {

class GameOptionMan
{
public:
	GameOptionMan(uintptr_t address);

	bool isAutoSave();

	void setAutoSave(bool autoSave);

	bool isMyWorld();

	int32_t getInvadeType() const;

	static uintptr_t getInstance();

	static bool hasInstance();
private:
	uintptr_t address;
};

}