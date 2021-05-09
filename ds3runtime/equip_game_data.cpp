#pragma once
#include "pch.h"
#include "equip_game_data.h"
#include "databaseaddress.h"
#include <ds3runtime/sprj_gaitem_imp.h>

namespace ds3runtime {

EquipGameData::EquipGameData(uintptr_t address)
{
	this->address = address;
}

void EquipGameData::equipGameItem(InventorySlot inventorySlot, int32_t inventoryItemId)
{
	auto sprjGaItemImp = SprjGaitemImp(SprjGaitemImp::getInstance());
	char weirdData[0x20] = {};
	char weirdBA[0x20] = {};
	*(char**)weirdData = accessMultilevelPointer<char>(DataBaseAddress::GAME, 0x10, 0x6AC);
	*(uintptr_t*)(weirdData + 8) = 0x140A27F09;
	*(int32_t*)(weirdData + 0x10) = inventoryItemId;
	*(char**)(weirdData + 0x18) = weirdBA;
	*(uintptr_t*)weirdBA = 0x1427AFA38;
	*(uintptr_t*)(weirdBA + 8) = address;
	*(uint32_t*)(weirdBA + 0x10) = 1;
	*(uint32_t*)(weirdBA + 0x14) = sprjGaItemImp.getOffsetIdByInventoryId(inventoryItemId);
	void (*switchEquipment)(...);
	*(uintptr_t*)&switchEquipment = 0x1405886a0;
	switchEquipment(address, 1, inventoryItemId, weirdData);
}

uintptr_t EquipGameData::getInstance()
{
	return (uintptr_t)accessMultilevelPointer<uintptr_t>(DataBaseAddress::GAME, 0x10, 0x228);
}

bool EquipGameData::hasInstance()
{
	return accessMultilevelPointer<uintptr_t>(DataBaseAddress::GAME, 0x10, 0x228) != nullptr;
}

}