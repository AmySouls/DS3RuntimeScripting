#pragma once
#include "pch.h"
#include "equip_game_data.h"
#include "databaseaddress.h"
#include <ds3runtime/sprj_gaitem_imp.h>
#include "memory_util.h"

namespace ds3runtime {

EquipGameData::EquipGameData(uintptr_t address)
{
	this->address = address;
}

void EquipGameData::equipInventoryItem(InventorySlot inventorySlot, int32_t inventoryItemId)
{
	char weirdData[0x20] = {};
	char weirdBA[0x20] = {};
	*(char**)weirdData = accessMultilevelPointer<char>(DataBaseAddress::GAME, 0x10, 0x6AC);
	*(uintptr_t*)(weirdData + 8) = 0x140A27F09;
	*(int32_t*)(weirdData + 0x10) = inventoryItemId;
	*(char**)(weirdData + 0x18) = weirdBA;
	*(uintptr_t*)weirdBA = 0x1427AFA38;
	*(uintptr_t*)(weirdBA + 8) = address;
	*(uint32_t*)(weirdBA + 0x10) = (uint32_t)inventorySlot;
	*(uint32_t*)(weirdBA + 0x14) = inventoryItemId == -1 ? 0 : EquipInventoryData(getEquipInventoryData()).getInventoryItemById(inventoryItemId)->uniqueId;
	void (*switchEquipment)(...);
	*(uintptr_t*)&switchEquipment = 0x1405886a0;
	switchEquipment(address, inventorySlot, inventoryItemId, weirdData);
}

void EquipGameData::equipGoodsInventoryItem(GoodsSlot goodsSlot, int32_t inventoryItemId)
{
	char data[0x20] = {};
	*(uint32_t*)data = inventoryItemId == -1 ? 0 : EquipInventoryData(getEquipInventoryData()).getInventoryItemById(inventoryItemId)->uniqueId;
	void (*equipGoods)(...);
	*(uintptr_t*)&equipGoods = 0x140583ED0;
	equipGoods(address, goodsSlot, data, inventoryItemId, 1);
}

int32_t EquipGameData::getInventoryItemIdBySlot(InventorySlot inventorySlot)
{
	return *accessMultilevelPointer<int32_t>(address + 0x24 + (uintptr_t)inventorySlot * 4);
}

int32_t EquipGameData::getInventoryItemIdByQuickSlot(GoodsSlot goodsSlot)
{
	return *accessMultilevelPointer<int32_t>(address + 0x26C + (uintptr_t)goodsSlot * 8);
}

int32_t EquipGameData::getInventoryItemIdByToolbeltSlot(GoodsSlot goodsSlot)
{
	return *accessMultilevelPointer<int32_t>(address + 0x26C + (uintptr_t)goodsSlot * 8 + 4);
}

void EquipGameData::modifyInventoryItemQuantity(int32_t inventoryItemId, int32_t quantityDelta)
{
	void (*ModifyItemQuantity)(...);
	*(uintptr_t*)&ModifyItemQuantity = 0x14057ea20;
	ModifyItemQuantity(address, inventoryItemId, quantityDelta, 1);
}

uintptr_t EquipGameData::getEquipInventoryData()
{
	return address + 0x1A8;
}

}