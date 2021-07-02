/*
	* DS3RuntimeScripting
	* Contributers: Amir
*/

#pragma once
#include "pch.h"
#include "memory_util.h"
#include "sprj_gaitem_ins.h"

namespace ds3runtime {

class SprjGaitemIns
{
public:
	SprjGaitemIns(uintptr_t address);

	int32_t getId();

	bool isValid();

	uintptr_t getVtablePtr();
private:
	uintptr_t address;
protected:
	uintptr_t getAddress();
};

}