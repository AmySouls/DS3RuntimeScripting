#pragma once
#include "pch.h"
#include "game_option_man.h"
#include "memory_util.h"
#include "databaseaddress.h"

namespace ds3runtime {

GameOptionMan::GameOptionMan(uintptr_t address)
{
	this->address = address;
}

bool GameOptionMan::isAutoSave()
{
	return *accessMultilevelPointer<uint8_t>(address + 0xB10);
}

void GameOptionMan::setAutoSave(bool autoSave)
{
	*accessMultilevelPointer<uint8_t>(address + 0xB10) = autoSave;
}

uintptr_t GameOptionMan::getInstance()
{
	return *accessMultilevelPointer<uintptr_t>(DataBaseAddress::BaseC);
}

bool GameOptionMan::hasInstance()
{
	return accessMultilevelPointer<uintptr_t>(DataBaseAddress::BaseC) != nullptr;
}

}