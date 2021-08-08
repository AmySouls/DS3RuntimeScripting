/*
	* DS3RuntimeScripting
	* Contributers: Amir
*/

#pragma once
#include "pch.h"
#include "ds3runtime/hook.h"
#include "ds3runtime/chr_ins.h"

namespace ds3runtime {

class DS1Backstabs : public Hook
{
public:
	DS1Backstabs();

	std::string getName()
	{
		return "ds1_backstabs";
	}

	static int32_t onThrowCheck(uintptr_t throwAttemptChr, uintptr_t throwTargetChr, char* throwInfoStruct, uintptr_t unk0);
private:
	static DS1Backstabs* instance;
};

}