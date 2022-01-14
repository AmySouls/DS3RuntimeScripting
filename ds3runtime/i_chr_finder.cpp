#pragma once
#include "pch.h"
#include "i_chr_finder.h"
#include "player_ins.h"

namespace ds3runtime {

bool IChrFinder::isBothChrAllies(const uintptr_t& chr1, const uintptr_t& chr2)
{
	bool(*Function)(...);
	*(uintptr_t*)&Function = 0x1409FFF50;
	return !Function(chr1, chr2);
}

};