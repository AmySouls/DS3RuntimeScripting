/*
	* DS3RuntimeScripting
	* Contributers: Amir
*/

#pragma once
#include "pch.h"
#include "ds1_backstabs.h"
#include "ds3runtime/ds3runtime.h"
#include "ds3runtime/world_chr_man.h"
#include "ds3runtime/memory_util.h"

namespace ds3runtime {

DS1Backstabs::DS1Backstabs()
	: Hook(0x1409e2f90, (uintptr_t)onThrowCheck)
{
	instance = this;
}

int32_t DS1Backstabs::onThrowCheck(uintptr_t throwAttemptChr, uintptr_t throwTargetChr, char* throwInfoStruct, uintptr_t unk0)
{
	uintptr_t* throwAreaParamPtr = accessMultilevelPointer<uintptr_t>(0x144743668);

	if (WorldChrMan::hasInstance()
			&& PlayerIns::isMainChrLoaded()
			&& PlayerIns(PlayerIns::getMainChrAddress()).isValid()
			&& throwAreaParamPtr != nullptr) {
	
		void (*throwFunc)(...);
		*(uintptr_t*)&throwFunc = 0x1405F3840;
		throwFunc(*throwAreaParamPtr, *accessMultilevelPointer<uintptr_t>(throwAttemptChr + 0x1F90, 0x88, 0x28), *accessMultilevelPointer<uintptr_t>(throwTargetChr + 0x1F90, 0x88, 0x28), *(int32_t*)(0x8 + throwInfoStruct));
		return 1;
	}

	int32_t (*originalFunction)(...);
	*(uintptr_t*)&originalFunction = *instance->original;
	return originalFunction(throwAttemptChr, throwTargetChr, throwInfoStruct, unk0);
}

DS1Backstabs* DS1Backstabs::instance = nullptr;

}