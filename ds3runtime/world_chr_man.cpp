#pragma once
#include "pch.h"
#include "world_chr_man.h"
#include "databaseaddress.h"
#include "player_ins.h"

namespace ds3runtime {

WorldChrMan::WorldChrMan(uintptr_t address)
{
	this->address = address;
}

void WorldChrMan::reloadCharacterFiles(std::wstring characterId)
{
	*accessMultilevelPointer<bool>(0x144768F7F) = true;
	void(*ReloadChrFunction)(...);
	*(uintptr_t*)&ReloadChrFunction = 0x1408D1E10;
	ReloadChrFunction(address, characterId.c_str());
}

void WorldChrMan::findEntityTest(int32_t id)
{
	int32_t buffer[2] = {};
	void(*FindEntityTest)(...);
	*(uintptr_t*)&FindEntityTest = 0x140436870;
	FindEntityTest(buffer, id);
	spdlog::debug("Contents: {} {} {}", buffer[0], buffer[1], *(uintptr_t*)buffer);
}

uintptr_t WorldChrMan::getInsByHandle(int32_t handle)
{
	uintptr_t (*GetInsByHandle)(int32_t handle);
	*(uintptr_t*)&GetInsByHandle = 0x140601fc0;
	return GetInsByHandle(handle);
}

uintptr_t WorldChrMan::getInstance()
{
	return *accessMultilevelPointer<uintptr_t>(DataBaseAddress::WorldChrMan);
}

bool WorldChrMan::hasInstance()
{
	return accessMultilevelPointer<uintptr_t>(DataBaseAddress::WorldChrMan) != nullptr;
}

};