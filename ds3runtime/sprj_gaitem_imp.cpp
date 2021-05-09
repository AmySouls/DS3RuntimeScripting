#pragma once
#include "pch.h"
#include "sprj_gaitem_imp.h"
#include "databaseaddress.h"

namespace ds3runtime {

SprjGaitemImp::SprjGaitemImp(uintptr_t address)
{
	this->address = address;
}

std::optional<SprjGaitemIns> SprjGaitemImp::getItemByOffsetId(uint32_t id)
{
	std::optional<SprjGaitemIns> item;
	uintptr_t* itemPointer = accessMultilevelPointer<uintptr_t>(address + (id % 0x10000ull) * 8ull + 0x40ull);
	
	if (itemPointer) {
		auto itemCheck = SprjGaitemIns(*itemPointer);
		if (itemCheck.isValid()) item = itemCheck;
	}

	return item;
}

uint32_t SprjGaitemImp::getOffsetIdByInventoryId(int32_t id)
{
	uint32_t offsetId = 0;
	uintptr_t inventoryBase = (uintptr_t)accessMultilevelPointer<uintptr_t>(DataBaseAddress::GAME, 0x10, 0x228 + 0x1A8);

	if ((unsigned)id < *(unsigned*)(inventoryBase + 0x24)) {
		offsetId = *(uint32_t*)(id * 0x10 + *(intptr_t*)(inventoryBase + 0x58));
	}
	else {
		offsetId = *(uint32_t*)((id - *(unsigned*)(inventoryBase + 0x24)) * 0x10 + *(intptr_t*)(inventoryBase + 0x48));
	}

	return offsetId;
}

std::optional<SprjGaitemIns> SprjGaitemImp::getItemByInventoryId(int32_t id)
{	
	const uint32_t offsetId = getOffsetIdByInventoryId(id);
	return offsetId != 0 ? getItemByOffsetId(offsetId) : std::optional<SprjGaitemIns>();
}

std::unordered_map<int32_t, SprjGaitemIns> SprjGaitemImp::getItemList()
{
	std::unordered_map<int32_t, SprjGaitemIns> items;
	uintptr_t inventoryBase = (uintptr_t)accessMultilevelPointer<uintptr_t>(DataBaseAddress::GAME, 0x10, 0x228 + 0x1A8);
	const uint32_t count = *(int*)(inventoryBase + 0x88);

	for (int i = 0; i < count + 1U; i++) {
		std::optional<SprjGaitemIns> item = getItemByInventoryId(i);
		if (item.has_value()) items.emplace(i, *item);
	}

	return items;
}

void SprjGaitemImp::removeItemByOffsetId(uint32_t offsetId)
{
	void(*RemoveItem)(...);
	*(uintptr_t*)&RemoveItem = 0x140a225a0;
	RemoveItem(address, &offsetId);
}

uintptr_t SprjGaitemImp::getInstance()
{
	return *accessMultilevelPointer<uintptr_t>(DataBaseAddress::SprjGaitem);
}

bool SprjGaitemImp::hasInstance()
{
	return accessMultilevelPointer<uintptr_t>(DataBaseAddress::SprjGaitem) != nullptr;
}

}