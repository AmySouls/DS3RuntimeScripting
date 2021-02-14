#pragma once
#include "pch.h"
#include "memory_util.h"
#include "databaseaddress.h"
#include "player.h"

namespace ds3runtime {

enum class Player::OffsetNumber {
	MainCharacter = 0,
	Player1 = 1,
	Player2 = 2,
	Player3 = 3,
	Player4 = 4,
	Player5 = 5
};

enum class Player::Covenant {
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

Player::Player(uintptr_t address) : Entity::Entity(address)
{
}

uintptr_t* Player::getNetworkPointer()
{
	return accessMultilevelPointer<uintptr_t>(address + 0x1FD0, 8);
}

std::wstring Player::getName()
{
	return std::wstring(accessMultilevelPointer<wchar_t>(address + 0x1FA0, 0x88));
}

void Player::setName(std::wstring name)
{
	wchar_t* namePointer = accessMultilevelPointer<wchar_t>(address + 0x1FA0, 0x88);
	memset(namePointer, 0, 16);
	memcpy(namePointer, name.c_str(), name.length() * 2);
}

Player::Covenant Player::getCovenant()
{
	return (Covenant)*accessMultilevelPointer<uint32_t>(address + 0x1FD0, 0xF7);
}

void Player::setCovenant(Covenant covenant)
{
	*accessMultilevelPointer<uint32_t>(address + 0x1FD0, 0xF7) = (uint32_t)covenant;
}

uint32_t Player::getLeftHandWeapon(uint32_t slotNumber)
{
	return *accessMultilevelPointer<uint32_t>(address + 0x1FD0, 0x32C + (slotNumber - 1) * 8);
}

void Player::setLeftHandWeapon(uint32_t slotNumber, uint32_t equipParamWeaponId)
{
	*accessMultilevelPointer<uint32_t>(address + 0x1FD0, 0x32C + (slotNumber - 1) * 8) = equipParamWeaponId;
}

uint32_t Player::getRightHandWeapon(uint32_t slotNumber)
{
	return *accessMultilevelPointer<uint32_t>(address + 0x1FD0, 0x330 + (slotNumber - 1) * 8);
}

void Player::setRightHandWeapon(uint32_t slotNumber, uint32_t equipParamWeaponId)
{
	*accessMultilevelPointer<uint32_t>(address + 0x1FD0, 0x330 + (slotNumber - 1) * 8) = equipParamWeaponId;
}

uint32_t Player::getHead()
{
	return *accessMultilevelPointer<uint32_t>(address + 0x1FD0, 0x35C);
}

void Player::setHead(uint32_t equipParamProtectorId)
{
	*accessMultilevelPointer<uint32_t>(address + 0x1FD0, 0x35C) = equipParamProtectorId;
}

uint32_t Player::getChest()
{
	return *accessMultilevelPointer<uint32_t>(address + 0x1FD0, 0x360);
}

void Player::setChest(uint32_t equipParamProtectorId)
{
	*accessMultilevelPointer<uint32_t>(address + 0x1FD0, 0x360) = equipParamProtectorId;
}

uint32_t Player::getHands()
{
	return *accessMultilevelPointer<uint32_t>(address + 0x1FD0, 0x364);
}

void Player::setHands(uint32_t equipParamProtectorId)
{
	*accessMultilevelPointer<uint32_t>(address + 0x1FD0, 0x364) = equipParamProtectorId;
}

uint32_t Player::getLegs()
{
	return *accessMultilevelPointer<uint32_t>(address + 0x1FD0, 0x368);
}

void Player::setLegs(uint32_t equipParamProtectorId)
{
	*accessMultilevelPointer<uint32_t>(address + 0x1FD0, 0x368) = equipParamProtectorId;
}

uintptr_t Player::getAddressByOffsetNumber(OffsetNumber offsetNumber)
{
	return *accessMultilevelPointer<uintptr_t>(getDataBaseAddress(DataBaseAddress::WorldChrMan), 0x40, (uint32_t)offsetNumber * 0x38);
}

bool Player::isPlayer(uintptr_t address)
{
	bool isPlayer = false;
	for (int i = 0; i < 5; i++) if (getAddressByOffsetNumber((OffsetNumber)i) == address) isPlayer = true;
	return isEntity(address) && isPlayer;
}

};