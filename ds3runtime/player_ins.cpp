#pragma once
#include "pch.h"
#include "memory_util.h"
#include "databaseaddress.h"
#include "player_ins.h"

namespace ds3runtime {

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
	return (Covenant)*accessMultilevelPointer<uint32_t>(address + 0x1FA0, 0xF7);
}

void PlayerIns::setCovenant(Covenant covenant)
{
	*accessMultilevelPointer<uint32_t>(address + 0x1AD0, 0xF7) = (uint32_t)covenant;
}

PlayerIns::SummonType PlayerIns::getSummonType()
{
	return (SummonType)*accessMultilevelPointer<uint32_t>(address + 0x1FA0, 0xFC);
}

void PlayerIns::setSummonType(PlayerIns::SummonType summonType)
{
	*accessMultilevelPointer<uint32_t>(address + 0x1FA0, 0xFC) = (uint32_t)summonType;
}

int32_t PlayerIns::getLeftHandWeapon(uint32_t slotNumber)
{
	return *accessMultilevelPointer<int32_t>(address + 0x1FD0, 0x32C + (slotNumber - 1) * 8);
}

void PlayerIns::setLeftHandWeapon(uint32_t slotNumber, int32_t equipParamWeaponId)
{
	*accessMultilevelPointer<int32_t>(address + 0x1FD0, 0x32C + (slotNumber - 1) * 8) = equipParamWeaponId;
}

int32_t PlayerIns::getRightHandWeapon(uint32_t slotNumber)
{
	return *accessMultilevelPointer<int32_t>(address + 0x1FD0, 0x330 + (slotNumber - 1) * 8);
}

void PlayerIns::setRightHandWeapon(uint32_t slotNumber, int32_t equipParamWeaponId)
{
	*accessMultilevelPointer<int32_t>(address + 0x1FD0, 0x330 + (slotNumber - 1) * 8) = equipParamWeaponId;
}

int32_t PlayerIns::getHead()
{
	return *accessMultilevelPointer<int32_t>(address + 0x1FD0, 0x35C);
}

void PlayerIns::setHead(int32_t equipParamProtectorId)
{
	*accessMultilevelPointer<int32_t>(address + 0x1FD0, 0x35C) = equipParamProtectorId;
}

int32_t PlayerIns::getChest()
{
	return *accessMultilevelPointer<int32_t>(address + 0x1FD0, 0x360);
}

void PlayerIns::setChest(int32_t equipParamProtectorId)
{
	*accessMultilevelPointer<int32_t>(address + 0x1FD0, 0x360) = equipParamProtectorId;
}

int32_t PlayerIns::getHands()
{
	return *accessMultilevelPointer<int32_t>(address + 0x1FD0, 0x364);
}

void PlayerIns::setHands(int32_t equipParamProtectorId)
{
	*accessMultilevelPointer<int32_t>(address + 0x1FD0, 0x364) = equipParamProtectorId;
}

int32_t PlayerIns::getLegs()
{
	return *accessMultilevelPointer<int32_t>(address + 0x1FD0, 0x368);
}

void PlayerIns::setLegs(int32_t equipParamProtectorId)
{
	*accessMultilevelPointer<int32_t>(address + 0x1FD0, 0x368) = equipParamProtectorId;
}

int32_t PlayerIns::getRing(uint32_t slotNumber)
{
	return *accessMultilevelPointer<int32_t>(address + 0x1FD0, 0x330 + (slotNumber - 1) * 8);
}

void PlayerIns::setRing(uint32_t slotNumber, int32_t equipParamAccessoryId)
{
	*accessMultilevelPointer<int32_t>(address + 0x1FD0, 0x330 + (slotNumber - 1) * 8) = equipParamAccessoryId;
}

uintptr_t* PlayerIns::getNetworkHandle()
{
	return accessMultilevelPointer<uintptr_t>(address + 0x1FD0, 0x8);
}

uintptr_t PlayerIns::getAddressByOffsetNumber(OffsetNumber offsetNumber)
{
	return *accessMultilevelPointer<uintptr_t>(DataBaseAddress::WorldChrMan, 0x40, (uint32_t)offsetNumber * 0x38);
}

bool PlayerIns::isPlayer(uintptr_t address)
{
	bool isPlayer = false;
	for (int i = 0; i < 5; i++) if (getAddressByOffsetNumber((OffsetNumber)i) == address) isPlayer = true;
	return isChrIns(address) && isPlayer && accessMultilevelPointer<uint32_t>(address + 0x1FA0, 0xFC);
}

uintptr_t PlayerIns::getMainChrAddress()
{
	return *accessMultilevelPointer<uintptr_t>(DataBaseAddress::WorldChrMan, 0x80);
}

bool PlayerIns::isMainChr(uintptr_t address)
{
	return isChrIns(address) && address == (uintptr_t)accessMultilevelPointer<uintptr_t>(DataBaseAddress::WorldChrMan, 0x80, 0);
}

bool PlayerIns::isMainChrLoaded()
{
	return accessMultilevelPointer<uintptr_t>(DataBaseAddress::WorldChrMan, 0x80);
}

};