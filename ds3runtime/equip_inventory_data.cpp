#pragma once
#include "pch.h"
#include "equip_inventory_data.h"
#include "databaseaddress.h"
#include <ds3runtime/sprj_gaitem_imp.h>
#include "memory_util.h"

namespace ds3runtime {

EquipInventoryData::EquipInventoryData(uintptr_t address)
{
	this->address = address;
}

void EquipInventoryData::discardInventoryItems(int32_t inventoryItemId, int32_t quantity)
{
	void (*DiscardItems)(...);
	*(uintptr_t*)&DiscardItems = 0x140a28eb0;
	DiscardItems(address + 0x10, inventoryItemId);
}

void EquipInventoryData::addItem(ItemParamIdPrefix paramIdPrefix, int32_t paramId, uint32_t quantity, int32_t durability)
{
	int32_t data[2] = {};
	*(int32_t*)data = durability;
	void (*AddItem)(...);
	*(uintptr_t*)&AddItem = 0x14058aa20;
	AddItem(address, paramIdPrefix, paramId, quantity, data);
}

InventoryItem* EquipInventoryData::getInventoryItemById(int32_t inventoryItemId)
{
	uintptr_t (*GetInventoryItem)(...);
	*(uintptr_t*)&GetInventoryItem = 0x14058cb30;
	return reinterpret_cast<InventoryItem*>(GetInventoryItem(address, inventoryItemId));
}

int32_t EquipInventoryData::getInventoryItemCount()
{
	return *accessMultilevelPointer<int32_t>(address + 0x88) + 1;
}

}