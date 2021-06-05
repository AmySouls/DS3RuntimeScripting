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

	std::optional<SprjGaitemIns> getItemByUniqueId(uint32_t uniqueId);

	static uintptr_t getInstance();

	static bool hasInstance();
private:
	uintptr_t address;
};

}