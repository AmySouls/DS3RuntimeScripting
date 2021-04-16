#pragma once
#include "pch.h"
#include "misc_functions.h"
#include "databaseaddress.h"
#include "ds3runtime.h"

namespace ds3runtime {

MiscFunctions::MiscFunctions()
{
}

void MiscFunctions::luaCallSOS(uintptr_t chrIns, int32_t sosFlag)
{
	void(*CallSOS)(...);
	*(uintptr_t*)&CallSOS = 0x1408B0BE0;
	CallSOS(chrIns, sosFlag);
}

};