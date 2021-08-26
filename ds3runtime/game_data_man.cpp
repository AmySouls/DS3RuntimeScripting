#pragma once
#include "pch.h"
#include "game_data_man.h"
#include "memory_util.h"
#include "databaseaddress.h"

namespace ds3runtime {

GameDataMan::GameDataMan(uintptr_t address)
{
	this->address = address;
}

uintptr_t GameDataMan::getPlayerGameData()
{
	return *accessMultilevelPointer<uintptr_t>(address + 0x10);
}

int32_t GameDataMan::getClearCount() const
{
	return *accessMultilevelPointer<int32_t>(address + 0x78);
}

void GameDataMan::setClearCount(const int32_t& clearCount)
{
	*accessMultilevelPointer<int32_t>(address + 0x78) = clearCount;
}

int32_t GameDataMan::getRescueWhite() const //Succesful co-op?
{
	return *accessMultilevelPointer<int32_t>(address + 0x88);
}

int32_t GameDataMan::getKillBlack() const //Dark spirit kills
{
	return *accessMultilevelPointer<int32_t>(address + 0x8C);
}

int32_t GameDataMan::getDeathNum() const
{
	return *accessMultilevelPointer<int32_t>(address + 0x98);
}

uint32_t GameDataMan::getPlayTime() const
{
	return *accessMultilevelPointer<uint32_t>(address + 0xA4);
}

uintptr_t GameDataMan::getInstance()
{
	return *accessMultilevelPointer<uintptr_t>(DataBaseAddress::GAME);
}

bool GameDataMan::hasInstance()
{
	return accessMultilevelPointer<uintptr_t>(DataBaseAddress::GAME) != nullptr;
}

}