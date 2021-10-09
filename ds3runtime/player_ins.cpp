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
	return *accessMultilevelPointer<uintptr_t>(getAddress() + 0x1FA0);
}

uintptr_t* PlayerIns::getNetworkPointer()
{
	return accessMultilevelPointer<uintptr_t>(getAddress() + 0x1FD0, 8);
}

int32_t PlayerIns::getLeftHandWeapon(const uint32_t& slotNumber) const
{
	return *accessMultilevelPointer<int32_t>(getAddress() + 0x1FA0, 0x32C + (slotNumber - 1) * 8);
}

void PlayerIns::setLeftHandWeapon(const uint32_t& slotNumber, const int32_t& equipParamWeaponId)
{
	*accessMultilevelPointer<int32_t>(getAddress() + 0x1FA0, 0x32C + (slotNumber - 1) * 8) = equipParamWeaponId;
}

int32_t PlayerIns::getRightHandWeapon(const uint32_t& slotNumber) const
{
	return *accessMultilevelPointer<int32_t>(getAddress() + 0x1FA0, 0x330 + (slotNumber - 1) * 8);
}

void PlayerIns::setRightHandWeapon(const uint32_t& slotNumber, const int32_t& equipParamWeaponId)
{
	*accessMultilevelPointer<int32_t>(getAddress() + 0x1FA0, 0x330 + (slotNumber - 1) * 8) = equipParamWeaponId;
}

int32_t PlayerIns::getHead() const
{
	return *accessMultilevelPointer<int32_t>(getAddress() + 0x1FA0, 0x35C);
}

void PlayerIns::setHead(const int32_t& equipParamProtectorId)
{
	*accessMultilevelPointer<int32_t>(getAddress() + 0x1FA0, 0x35C) = equipParamProtectorId;
}

int32_t PlayerIns::getChest() const
{
	return *accessMultilevelPointer<int32_t>(getAddress() + 0x1FA0, 0x360);
}

void PlayerIns::setChest(const int32_t& equipParamProtectorId)
{
	*accessMultilevelPointer<int32_t>(getAddress() + 0x1FA0, 0x360) = equipParamProtectorId;
}

int32_t PlayerIns::getHands() const
{
	return *accessMultilevelPointer<int32_t>(getAddress() + 0x1FA0, 0x364);
}

void PlayerIns::setHands(const int32_t& equipParamProtectorId)
{
	*accessMultilevelPointer<int32_t>(getAddress() + 0x1FA0, 0x364) = equipParamProtectorId;
}

int32_t PlayerIns::getLegs() const
{
	return *accessMultilevelPointer<int32_t>(getAddress() + 0x1FA0, 0x368);
}

void PlayerIns::setLegs(const int32_t& equipParamProtectorId)
{
	*accessMultilevelPointer<int32_t>(getAddress() + 0x1FA0, 0x368) = equipParamProtectorId;
}

int32_t PlayerIns::getRing(const uint32_t& slotNumber) const
{
	return *accessMultilevelPointer<int32_t>(getAddress() + 0x1FA0, 0x370 + (slotNumber - 1) * 4);
}

void PlayerIns::setRing(const uint32_t& slotNumber, const int32_t& equipParamAccessoryId)
{
	*accessMultilevelPointer<int32_t>(getAddress() + 0x1FA0, 0x370 + (slotNumber - 1) * 4) = equipParamAccessoryId;
}

int32_t PlayerIns::getAmmo(const uint32_t& slotNumber) const
{
	return *accessMultilevelPointer<int32_t>(getAddress() + 0x1FA0, 0x344 + (slotNumber - 1) * 4);
}

void PlayerIns::setAmmo(const uint32_t& slotNumber, const int32_t& equipParamAccessoryId)
{
	*accessMultilevelPointer<int32_t>(getAddress() + 0x1FA0, 0x344 + (slotNumber - 1) * 4) = equipParamAccessoryId;
}

int32_t PlayerIns::getCovenant() const
{
	return *accessMultilevelPointer<int32_t>(getAddress() + 0x1FA0, 0x380);
}

void PlayerIns::setCovenant(const int32_t& equipParamAccessoryId)
{
	*accessMultilevelPointer<int32_t>(getAddress() + 0x1FA0, 0x380) = equipParamAccessoryId;
}

bool PlayerIns::isNoGoodsConsume() const
{
	return (*accessMultilevelPointer<uint8_t>(getAddress() + 0x1EEA) & static_cast<uint8_t>(pow(2, 3))) == pow(2, 3);
}

void PlayerIns::setNoGoodsConsume(const bool& value)
{
	uint8_t* newByte = accessMultilevelPointer<uint8_t>(getAddress() + 0x1EEA);
	if (value) *newByte = *newByte | static_cast<uint8_t>(pow(2, 3));
	else *newByte = (*newByte & ~static_cast<uint8_t>(pow(2, 3)));
}

uintptr_t PlayerIns::getPlayerCtrl()
{
	return *accessMultilevelPointer<uintptr_t>(getAddress() + 0x50);
}

uintptr_t* PlayerIns::getNetworkHandle()
{
	return accessMultilevelPointer<uintptr_t>(getAddress() + 0x1FD0, 0x8);
}

uintptr_t PlayerIns::getAddressByOffsetNumber(const OffsetNumber& offsetNumber)
{
	return *accessMultilevelPointer<uintptr_t>(DataBaseAddress::WorldChrMan, 0x40, static_cast<uint32_t>(offsetNumber) * 0x38);
}

bool PlayerIns::isChrWithOffsetNumber(const OffsetNumber& offsetNumber)
{
	const uintptr_t* chrAddress = accessMultilevelPointer<uintptr_t>(DataBaseAddress::WorldChrMan, 0x40, static_cast<uint32_t>(offsetNumber) * 0x38);
	return chrAddress != nullptr && isPlayer(*chrAddress);
}

bool PlayerIns::isPlayer(const uintptr_t& address)
{
	bool isPlayer = false;
	for (int i = 0; i < 5; i++) if (getAddressByOffsetNumber(static_cast<OffsetNumber>(i)) == address) isPlayer = true;
	return isChrIns(address) && isPlayer && accessMultilevelPointer<uint32_t>(address + 0x1FA0, 0xFC);
}

bool PlayerIns::isValid() const
{
	bool isPlayer = false;
	for (int i = 0; i < 5; i++) if (getAddressByOffsetNumber(static_cast<OffsetNumber>(i)) == getAddress()) isPlayer = true;
	return isPlayer && accessMultilevelPointer<uint32_t>(getAddress() + 0x1FA0, 0xFC);
}

uintptr_t PlayerIns::getMainChrAddress()
{
	return *accessMultilevelPointer<uintptr_t>(DataBaseAddress::WorldChrMan, 0x80);
}

bool PlayerIns::isMainChr(const uintptr_t& address)
{
	return isChrIns(address) && address == reinterpret_cast<uintptr_t>(accessMultilevelPointer<uintptr_t>(DataBaseAddress::WorldChrMan, 0x80, 0));
}

bool PlayerIns::isMainChrLoaded()
{
	return accessMultilevelPointer<uintptr_t>(DataBaseAddress::WorldChrMan, 0x80);
}

};
