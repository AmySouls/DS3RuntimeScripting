/*
	* DS3RuntimeScripting
	* Contributers: Amir
*/

#pragma once
#include "pch.h"
#include "memory_util.h"

namespace ds3runtime {

class MiscFunctions
{
public:
	MiscFunctions();

	void luaCallSOS(uintptr_t chrIns, int32_t sosFlag);
};

}