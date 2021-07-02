/*
* DS3RuntimeScripting
* Contributers: Amir
*/

#pragma once
#include "pch.h"
#include "memory_util.h"

namespace ds3runtime {

class SprjLuaEventConditionItem
{
public:
	SprjLuaEventConditionItem(uintptr_t address);

	void giveItem(int32_t quantity, int32_t giveItemId, int32_t durablity);

	static uintptr_t getInstance();

	static bool hasInstance();
private:
	uintptr_t address;
};

}