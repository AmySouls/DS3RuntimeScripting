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

std::array<float, 3> WorldChrMan::getCamVector() const
{
	std::array<float, 3> quaternion;
	memcpy(&quaternion[0], accessMultilevelPointer<float>(address + 0x31A0, 0x30), sizeof(float) * 3);
	return quaternion;
}

bool WorldChrMan::doesChrHaveSpEffect(ChrIns chr, const int32_t& spEffectId) const
{
	const uintptr_t effectBase = *accessMultilevelPointer<uintptr_t>(chr.getAddress() + 0x11c8);
	uintptr_t effectPtr = *accessMultilevelPointer<uintptr_t>(effectBase + 8);
	if (reinterpret_cast<uintptr_t*>(effectPtr) == nullptr) return false;
	uint8_t effectcount = 0;

	for (effectPtr = *accessMultilevelPointer<uintptr_t>(effectPtr + 0x78); accessMultilevelPointer<uintptr_t*>(effectPtr) != nullptr; effectPtr = *accessMultilevelPointer<uintptr_t>(effectPtr + 0x78)) {
		if (*accessMultilevelPointer<int32_t>(effectPtr + 0x60) == spEffectId) return true;
	}
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