/*
	* DS3RuntimeScripting
	* Contributers: Amir
*/

#pragma once
#include "pch.h"
#include "test_script.h"
#include "ds3runtime/ds3runtime.h"

namespace ds3runtime {

void TestScript::execute()
{
	uintptr_t* chrAddrPtr = accessMultilevelPointer<uintptr_t>(getDataBaseAddress(DataBaseAddress::WorldChrMan), 0x80);
	if (!chrAddrPtr) return;
	uintptr_t chrAddr = MainCharacter::getMainCharacterAddress();
	if (!MainCharacter::isMainCharacter(chrAddr)) return;
	MainCharacter chr(chrAddr);
	
	ds3runtime_global->getLog()->info(L"Test");
	if (chr.getAnimationString().compare(L"RollingMedium") == 0 && lastAnimationString.compare(L"RollingMedium") != 0) {
		chr.setHealth(std::max(0u, chr.getHealth() - 100));
	}

	lastAnimationString = chr.getAnimationString();

}

}