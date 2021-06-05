/*
	* DS3RuntimeScripting
	* Contributers: Amir
*/

#pragma once
#include "pch.h"
#include "memory_util.h"

namespace ds3runtime {

struct InventoryItem
{
	int32_t uniqueId;
	int32_t giveId;
	uint32_t quantity;
	int32_t unknown1;
};

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
	Helmet = 12,
	Armor = 13,
	Gauntlet = 14,
	Leggings = 15,
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

enum class ItemParamIdPrefix : int32_t
{
	Weapon = 0,
	Protector = 0x10000000,
	Accessory = 0x20000000,
	Goods = 0x40000000,
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

	void discardInventoryItems(int32_t inventoryItemId, int32_t quantity);

	void modifyInventoryItemQuantity(int32_t inventoryItemId, int32_t quantityDelta);

	void addItem(ItemParamIdPrefix paramIdPrefix, int32_t paramId, uint32_t quantity, int32_t durability);

	InventoryItem* getInventoryItemById(int32_t inventoryItemId);

	int32_t getInventoryItemCount();

	static uintptr_t getInstance();

	static bool hasInstance();
private:
	uintptr_t address;
};

}