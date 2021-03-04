#pragma once
#include "pch.h"
#include "memory_util.h"
#include "databaseaddress.h"
#include "player_ins.h"

namespace ds3runtime {

enum class PlayerIns::OffsetNumber {
	MainCharacter = 0,
	Player1 = 1,
	Player2 = 2,
	Player3 = 3,
	Player4 = 4,
	Player5 = 5
};

enum class PlayerIns::Covenant {
	None = 0,
	BladeOfTheDarkmoon = 1,
	WarriorsOfSunlight = 2,
	MoundMakers = 3,
	SpearsOfTheChurch = 4,
	FingersOfRosaria = 5,
	WatchdogsOfFarron = 6,
	AldrichFaithfuls = 7,
	WayOfBlue = 8,
	BlueSentinels = 9
};

PlayerIns::PlayerIns(uintptr_t address) : ChrIns::ChrIns(address)
{
}

uintptr_t* PlayerIns::getNetworkPointer()
{
	return accessMultilevelPointer<uintptr_t>(address + 0x1FD0, 8);
}

uint32_t PlayerIns::getNumber()
{
	return *accessMultilevelPointer<uint32_t>(address + 0x1FA0, 0x10);
}

std::wstring PlayerIns::getName()
{
	return std::wstring(accessMultilevelPointer<wchar_t>(address + 0x1FA0, 0x88));
}

void PlayerIns::setName(std::wstring name)
{
	wchar_t* namePointer = accessMultilevelPointer<wchar_t>(address + 0x1FA0, 0x88);
	memset(namePointer, 0, 16);
	memcpy(namePointer, name.c_str(), name.length() * 2);
}

PlayerIns::Covenant PlayerIns::getCovenant()
{
	return (Covenant)*accessMultilevelPointer<uint32_t>(address + 0x1FD0, 0xF7);
}

void PlayerIns::setCovenant(Covenant covenant)
{
	*accessMultilevelPointer<uint32_t>(address + 0x1FD0, 0xF7) = (uint32_t)covenant;
}

uint32_t PlayerIns::getLeftHandWeapon(uint32_t slotNumber)
{
	return *accessMultilevelPointer<uint32_t>(address + 0x1FD0, 0x32C + (slotNumber - 1) * 8);
}

void PlayerIns::setLeftHandWeapon(uint32_t slotNumber, uint32_t equipParamWeaponId)
{
	*accessMultilevelPointer<uint32_t>(address + 0x1FD0, 0x32C + (slotNumber - 1) * 8) = equipParamWeaponId;
}

uint32_t PlayerIns::getRightHandWeapon(uint32_t slotNumber)
{
	return *accessMultilevelPointer<uint32_t>(address + 0x1FD0, 0x330 + (slotNumber - 1) * 8);
}

void PlayerIns::setRightHandWeapon(uint32_t slotNumber, uint32_t equipParamWeaponId)
{
	*accessMultilevelPointer<uint32_t>(address + 0x1FD0, 0x330 + (slotNumber - 1) * 8) = equipParamWeaponId;
}

uint32_t PlayerIns::getHead()
{
	return *accessMultilevelPointer<uint32_t>(address + 0x1FD0, 0x35C);
}

void PlayerIns::setHead(uint32_t equipParamProtectorId)
{
	*accessMultilevelPointer<uint32_t>(address + 0x1FD0, 0x35C) = equipParamProtectorId;
}

uint32_t PlayerIns::getChest()
{
	return *accessMultilevelPointer<uint32_t>(address + 0x1FD0, 0x360);
}

void PlayerIns::setChest(uint32_t equipParamProtectorId)
{
	*accessMultilevelPointer<uint32_t>(address + 0x1FD0, 0x360) = equipParamProtectorId;
}

uint32_t PlayerIns::getHands()
{
	return *accessMultilevelPointer<uint32_t>(address + 0x1FD0, 0x364);
}

void PlayerIns::setHands(uint32_t equipParamProtectorId)
{
	*accessMultilevelPointer<uint32_t>(address + 0x1FD0, 0x364) = equipParamProtectorId;
}

uint32_t PlayerIns::getLegs()
{
	return *accessMultilevelPointer<uint32_t>(address + 0x1FD0, 0x368);
}

void PlayerIns::setLegs(uint32_t equipParamProtectorId)
{
	*accessMultilevelPointer<uint32_t>(address + 0x1FD0, 0x368) = equipParamProtectorId;
}

uintptr_t* PlayerIns::getNetworkHandle()
{
	return accessMultilevelPointer<uintptr_t>(address + 0x1FD0, 0x8);
}

uintptr_t PlayerIns::getAddressByOffsetNumber(OffsetNumber offsetNumber)
{
	return *accessMultilevelPointer<uintptr_t>(getDataBaseAddress(DataBaseAddress::WorldChrMan), 0x40, (uint32_t)offsetNumber * 0x38);
}

bool PlayerIns::isPlayer(uintptr_t address)
{
	bool isPlayer = false;
	for (int i = 0; i < 5; i++) if (getAddressByOffsetNumber((OffsetNumber)i) == address) isPlayer = true;
	return isChrIns(address) && isPlayer;
}

uintptr_t PlayerIns::getMainChrAddress()
{
	return *accessMultilevelPointer<uintptr_t>(getDataBaseAddress(DataBaseAddress::WorldChrMan), 0x80);
}

bool PlayerIns::isMainChr(uintptr_t address)
{
	return isChrIns(address) && address == *accessMultilevelPointer<uintptr_t>(getDataBaseAddress(DataBaseAddress::WorldChrMan), 0x80);
}

};