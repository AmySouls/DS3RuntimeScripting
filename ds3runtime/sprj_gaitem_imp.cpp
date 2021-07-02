#pragma once
#include "pch.h"
#include "sprj_gaitem_imp.h"
#include "databaseaddress.h"

namespace ds3runtime {

SprjGaitemImp::SprjGaitemImp(uintptr_t address)
{
	this->address = address;
}

std::optional<SprjGaitemIns> SprjGaitemImp::getItemByUniqueId(uint32_t uniqueId)
{
	std::optional<SprjGaitemIns> item;
	uintptr_t* itemPointer = accessMultilevelPointer<uintptr_t>(address + (uniqueId % 0x10000ull) * 8ull + 0x40ull);
	
	if (itemPointer) {
		auto itemCheck = SprjGaitemIns(*itemPointer);
		if (itemCheck.isValid()) item = itemCheck;
	}

	return item;
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