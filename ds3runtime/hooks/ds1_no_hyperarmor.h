/*
	* DS3RuntimeScripting
	* Contributers: Amir
*/

#pragma once
#include "pch.h"
#include "ds3runtime/hook.h"
#include "ds3runtime/chr_ins.h"

namespace ds3runtime {

class DS1NoHyperArmor : public Hook
{
public:
	DS1NoHyperArmor();

	std::string getName()
	{
		return "ds1_no_hyperarmor";
	}

	static void onTaeEvent(uintptr_t sprjChrTaeAnimEvent);
private:
	static DS1NoHyperArmor* instance;
};

}