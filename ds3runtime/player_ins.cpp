#pragma once
#include "pch.h"
#include "memory_util.h"
#include "databaseaddress.h"
#include "player_ins.h"

namespace ds3runtime {

PlayerIns::PlayerIns(uintptr_t address) : ChrIns::ChrIns(address)
{
}

uintptr_t PlayerIns::getPlayerGameData()
{
	return *accessMultilevelPointer<uintptr_t>(address + 0x1FA0);
}

uintptr_t* PlayerIns::getNetworkPointer()
{
	return accessMultilevelPointer<uintptr_t>(address + 0x1FD0, 8); //PlayerDebugSession
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

bool PlayerIns::isNoGoodsConsume()
{
	return (*accessMultilevelPointer<uint8_t>(address + 0x1EEA) & (uint8_t)pow(2, 3)) == pow(2, 3);
}

void PlayerIns::setNoGoodsConsume(bool value)
{
	uint8_t* newByte = accessMultilevelPointer<uint8_t>(address + 0x1EEA);
	if (value) *newByte = *newByte | (uint8_t)pow(2, 3);
	else *newByte = (*newByte & ~(uint8_t)pow(2, 3));
}

uintptr_t PlayerIns::getPlayerCtrl()
{
	return *accessMultilevelPointer<uintptr_t>(address + 0x50);
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

bool PlayerIns::isValid()
{
	bool isPlayer = false;
	for (int i = 0; i < 5; i++) if (getAddressByOffsetNumber((OffsetNumber)i) == address) isPlayer = true;
	return isPlayer && accessMultilevelPointer<uint32_t>(address + 0x1FA0, 0xFC);
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