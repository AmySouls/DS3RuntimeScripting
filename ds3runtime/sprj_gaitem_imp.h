/*
	* DS3RuntimeScripting
	* Contributers: Amir
*/

#pragma once
#include "pch.h"
#include "memory_util.h"
#include "sprj_gaitem_ins.h"

namespace ds3runtime {

class SprjGaitemImp
{
public:
	SprjGaitemImp(uintptr_t address);

	std::optional<SprjGaitemIns> getItemByOffsetId(uint32_t id);

	int32_t getOffsetIdByInventoryId(int32_t id);

	std::optional<SprjGaitemIns> getItemByInventoryId(int32_t id);

	std::vector<SprjGaitemIns> getItemList();

	static uintptr_t getInstance();

	static bool hasInstance();
private:
	uintptr_t address;
};

}