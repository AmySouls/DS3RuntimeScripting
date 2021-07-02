/*
	* DS3RuntimeScripting
	* Contributers: Amir
*/

#pragma once
#include "pch.h"

namespace ds3runtime {

struct InventoryItem
{
	int32_t uniqueId;
	int32_t giveId;
	uint32_t quantity;
	int32_t unknown1;
};

enum class ItemParamIdPrefix : int32_t
{
	Weapon = 0,
	Protector = 0x10000000,
	Accessory = 0x20000000,
	Goods = 0x40000000,
};

class EquipInventoryData
{
public:
	EquipInventoryData(uintptr_t address);

	void discardInventoryItems(int32_t inventoryItemId, int32_t quantity);

	void addItem(ItemParamIdPrefix paramIdPrefix, int32_t paramId, uint32_t quantity, int32_t durability);

	InventoryItem* getInventoryItemById(int32_t inventoryItemId);

	int32_t getInventoryItemCount();
private:
	uintptr_t address;
};

}