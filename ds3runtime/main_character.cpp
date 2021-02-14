#pragma once
#include "pch.h"
#include "main_character.h"
#include "memory_util.h"
#include "databaseaddress.h"

namespace ds3runtime {

MainCharacter::MainCharacter(uintptr_t address) : Player(address)
{
}

uintptr_t MainCharacter::getMainCharacterAddress()
{
	return *accessMultilevelPointer<uintptr_t>(getDataBaseAddress(DataBaseAddress::WorldChrMan), 0x80);
}

bool MainCharacter::isMainCharacter(uintptr_t address)
{
	return isEntity(address) && address == *accessMultilevelPointer<uintptr_t>(getDataBaseAddress(DataBaseAddress::WorldChrMan), 0x80);
}

}