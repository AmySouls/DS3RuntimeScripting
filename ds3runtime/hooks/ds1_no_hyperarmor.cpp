/*
	* DS3RuntimeScripting
	* Contributers: Amir
*/

#pragma once
#include "pch.h"
#include "ds1_no_hyperarmor.h"
#include "ds3runtime/ds3runtime.h"
#include "ds3runtime/memory_util.h"

namespace ds3runtime {

DS1NoHyperArmor::DS1NoHyperArmor()
	: Hook(0x1409E4765, (uintptr_t)onTaeEvent)
{
	instance = this;
}

void DS1NoHyperArmor::onTaeEvent(uintptr_t sprjChrTaeAnimEvent)
{
	return;
}

DS1NoHyperArmor* DS1NoHyperArmor::instance = nullptr;

}