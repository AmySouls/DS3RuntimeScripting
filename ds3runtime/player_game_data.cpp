#pragma once
#include "pch.h"
#include "player_game_data.h"
#include "databaseaddress.h"
#include "memory_util.h"

namespace ds3runtime {

PlayerGameData::PlayerGameData(uintptr_t address)
{
	this->address = address;
}

int32_t PlayerGameData::getPlayerNo()
{
	return *accessMultilevelPointer<int32_t>(address + 0x10);
}

PlayerGameData::Voice PlayerGameData::getVoice()
{
	return (Voice)*accessMultilevelPointer<uint8_t>(address + 0xAB);
}

void PlayerGameData::setVoice(Voice voice)
{
	*accessMultilevelPointer<uint8_t>(address + 0xAB) = (uint8_t)voice;
}

PlayerGameData::Class PlayerGameData::getClass()
{
	return (Class)*accessMultilevelPointer<uint8_t>(address + 0xAE);
}

void PlayerGameData::setClass(Class chrClass)
{
	*accessMultilevelPointer<uint8_t>(address + 0xAE) = (uint8_t)chrClass;
}

PlayerGameData::Gender PlayerGameData::getGender()
{
	return (Gender)*accessMultilevelPointer<uint8_t>(address + 0xAA);
}

void PlayerGameData::setGender(Gender gender)
{
	*accessMultilevelPointer<uint8_t>(address + 0xAA) = (uint8_t)gender;
}

PlayerGameData::Age PlayerGameData::getAge()
{
	return (Age)*accessMultilevelPointer<uint8_t>(address + 0x6B8);
}

void PlayerGameData::setAge(Age age)
{
	*accessMultilevelPointer<uint8_t>(address + 0x6B8) = (uint8_t)age;
}

Attributes PlayerGameData::getAttributes()
{
	return *reinterpret_cast<Attributes*>(accessMultilevelPointer<uintptr_t>(address + 0x44));
}

void PlayerGameData::setAttributes(Attributes attributes)
{
	*reinterpret_cast<Attributes*>(accessMultilevelPointer<uintptr_t>(address + 0x44)) = attributes;
}

std::wstring PlayerGameData::getName()
{
	return std::wstring(accessMultilevelPointer<wchar_t>(address + 0x88));
}

void PlayerGameData::setName(std::wstring name)
{
	wchar_t* namePointer = accessMultilevelPointer<wchar_t>(address + 0x88);
	memset(namePointer, 0, 16);
	memcpy(namePointer, name.c_str(), name.length() * 2);
}

PlayerGameData::Covenant PlayerGameData::getCovenant()
{
	return (Covenant)*accessMultilevelPointer<uint32_t>(address + 0xF7);
}

void PlayerGameData::setCovenant(Covenant covenant)
{
	*accessMultilevelPointer<uint32_t>(address + 0xF7) = (uint32_t)covenant;
}

PlayerGameData::InvadeType PlayerGameData::getInvadeType()
{
	return (InvadeType)*accessMultilevelPointer<uint32_t>(address + 0xFC);
}

void PlayerGameData::setInvadeType(PlayerGameData::InvadeType invadeType)
{
	*accessMultilevelPointer<uint32_t>(address + 0xFC) = (uint32_t)invadeType;
}

uintptr_t PlayerGameData::getEquipGameData()
{
	return address + 0x228;
}

uint16_t PlayerGameData::getWeaponSheathData()
{
	uint16_t(*function)(uintptr_t);
	*(uintptr_t*)&function = 0x1409EBA70;
	return function(address + 0x2B8);
}

int32_t PlayerGameData::getRightHandSlot()
{
	return *accessMultilevelPointer<int32_t>(address + 0x2BC);
}

void PlayerGameData::setRightHandSlot(int32_t slot)
{
	*accessMultilevelPointer<int32_t>(address + 0x2BC) = slot;
}

int32_t PlayerGameData::getLeftHandSlot()
{
	return *accessMultilevelPointer<int32_t>(address + 0x2C0);
}

void PlayerGameData::setLeftHandSlot(int32_t slot)
{
	*accessMultilevelPointer<int32_t>(address + 0x2C0) = slot;
}

int32_t PlayerGameData::getWeaponSheathState()
{
	return *accessMultilevelPointer<int32_t>(address + 0x2B8);
}

void PlayerGameData::setWeaponSheathState(int32_t slot)
{
	*accessMultilevelPointer<int32_t>(address + 0x2B8) = slot;
}

}