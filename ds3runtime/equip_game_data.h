/*
	* DS3RuntimeScripting
	* Contributers: Amir
*/

#pragma once
#include "pch.h"
#include "memory_util.h"

namespace ds3runtime {

class EquipGameData
{
public:
	enum class InventorySlot : uint32_t {
		PrimaryLeftWep = 0,
		PrimaryRightWep = 1,
		SecondaryLeftWep = 2,
		SecondaryRightWep = 3,
		TertiaryLeftWep = 4,
		TertiaryRightHand = 5,
		Helmet = 6,
		Armor = 7,
		Gauntlet = 8,
		Leggings = 9,
	};

	EquipGameData(uintptr_t address);

	void equipGameItem(InventorySlot inventorySlot, int32_t inventoryItemId);

	static uintptr_t getInstance();

	static bool hasInstance();
private:
	uintptr_t address;
};

}