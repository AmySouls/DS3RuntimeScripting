/*
	* DS3RuntimeScripting
	* Contributers: Amir
	* Further notes on how to use this class:
*/

#pragma once
#include "pch.h"
#include "memory_util.h"
#include "chr_ins.h"

namespace ds3runtime {

class IChrFinder
{
public:
	static bool isBothChrAllies(const uintptr_t& chr1, const uintptr_t& chr2);
private:
};

}