#pragma once
#include "pch.h"
#include "sprj_lua_event_condition_item.h"
#include "databaseaddress.h"

namespace ds3runtime {

SprjLuaEventConditionItem::SprjLuaEventConditionItem(uintptr_t address)
{
	this->address = address;
}

void SprjLuaEventConditionItem::giveItem(int32_t quantity, int32_t giveItemId, int32_t durablity)
{
	static int countAdded = 0;
	countAdded++;
	int32_t input1[4] = {};
	int32_t input2[8] = {};
	input1[0] = 1;
	input1[1] = giveItemId;
	input1[2] = quantity;
	input1[3] = durablity;
	void (*function)(...);
	*(uintptr_t*)&function = 0x1407BBA70;
	function(address, input1, input2);
}

uintptr_t SprjLuaEventConditionItem::getInstance()
{
	return *accessMultilevelPointer<uintptr_t>(DataBaseAddress::SprjLuaEventConditionItem);
}

bool SprjLuaEventConditionItem::hasInstance()
{
	return accessMultilevelPointer<uintptr_t>(DataBaseAddress::SprjLuaEventConditionItem) != nullptr;
}

}