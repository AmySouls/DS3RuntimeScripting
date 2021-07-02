/*
	* DS3RuntimeScripting
	* Contributers: Amir
*/

#pragma once
#include "pch.h"
#include "equip_inventory_data.h"

namespace ds3runtime {

enum class InventorySlot : uint32_t
{
	PrimaryLeftWep = 0,
	PrimaryRightWep = 1,
	SecondaryLeftWep = 2,
	SecondaryRightWep = 3,
	TertiaryLeftWep = 4,
	TertiaryRightHand = 5,
	PrimaryArrow = 6,
	PrimaryBolt = 7,
	SecondaryArrow = 8,
	SecondaryBolt = 9,
	Head = 12,
	Chest = 13,
	Hands = 14,
	Legs = 15,
	Ring1 = 17,
	Ring2 = 18,
	Ring3 = 19,
	Ring4 = 20,
	Covenant = 21,
};

enum class GoodsSlot : uint32_t
{
	QuickItem1 = 0,
	QuickItem2 = 1,
	QuickItem3 = 2,
	QuickItem4 = 3,
	QuickItem5 = 4,
	QuickItem6 = 5,
	QuickItem7 = 6,
	QuickItem8 = 7,
	QuickItem9 = 8,
	QuickItem10 = 9,
	Toolbelt1 = 10,
	Toolbelt2 = 11,
	Toolbelt3 = 12,
	Toolbelt4 = 13,
	Toolbelt5 = 14,
};

enum class ItemUniqueIdPrefix : uint32_t 
{
	Weapon = 0x80800000,
	Armor = 0x90800000,
	Accessory = 0xA0000000,
	Goods = 0xB0000000,
};

class EquipGameData
{
public:
	EquipGameData(uintptr_t address);

	int32_t getInventoryItemIdBySlot(InventorySlot inventorySlot);

	int32_t getInventoryItemIdByQuickSlot(GoodsSlot goodsSlot);

	int32_t getInventoryItemIdByToolbeltSlot(GoodsSlot goodsSlot);

	void equipInventoryItem(InventorySlot inventorySlot, int32_t inventoryItemId);

	void equipGoodsInventoryItem(GoodsSlot goodsSlot, int32_t inventoryItemId);

	void modifyInventoryItemQuantity(int32_t inventoryItemId, int32_t quantityDelta);

	uintptr_t getEquipInventoryData();
private:
	uintptr_t address;
};

}