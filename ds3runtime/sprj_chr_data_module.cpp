#pragma once
#include "pch.h"
#include "sprj_chr_data_module.h"
#include "databaseaddress.h"
#include "memory_util.h"

namespace ds3runtime {

SprjChrDataModule::SprjChrDataModule(uintptr_t address)
{
	this->address = address;
}

uint32_t SprjChrDataModule::getHealth()
{
	return *accessMultilevelPointer<uint32_t>(address + 0xD8);
}

void SprjChrDataModule::setHealth(uint32_t health)
{
	*accessMultilevelPointer<uint32_t>(address + 0xD8) = health;
}

uint32_t SprjChrDataModule::getStamina()
{
	return *accessMultilevelPointer<uint32_t>(address + 0xF0);
}

void SprjChrDataModule::setStamina(uint32_t stamina)
{
	*accessMultilevelPointer<uint32_t>(address + 0xF0) = stamina;
}

uint32_t SprjChrDataModule::getFP()
{
	return *accessMultilevelPointer<uint32_t>(address + 0xE4);
}

void SprjChrDataModule::setFP(uint32_t fp)
{
	*accessMultilevelPointer<uint32_t>(address + 0xE4) = fp;
}

uint32_t SprjChrDataModule::getMaxHealth()
{
	return *accessMultilevelPointer<uint32_t>(address + 0xDC);
}

void SprjChrDataModule::setMaxHealth(uint32_t maxHealth)
{
	*accessMultilevelPointer<uint32_t>(address + 0xDC) = maxHealth;
}

uint32_t SprjChrDataModule::getMaxStamina()
{
	return *accessMultilevelPointer<uint32_t>(address + 0xF4);
}

void SprjChrDataModule::setMaxStamina(uint32_t maxStamina)
{
	*accessMultilevelPointer<uint32_t>(address + 0xF4) = maxStamina;
}

uint32_t SprjChrDataModule::getMaxFP()
{
	return *accessMultilevelPointer<uint32_t>(address + 0xE8);
}

void SprjChrDataModule::setMaxFP(uint32_t maxFP)
{
	*accessMultilevelPointer<uint32_t>(address + 0xE8) = maxFP;
}

uint32_t SprjChrDataModule::getBaseMaxHealth()
{
	return *accessMultilevelPointer<uint32_t>(address + 0xE0);
}

void SprjChrDataModule::setBaseMaxHealth(uint32_t baseMaxHealth)
{
	*accessMultilevelPointer<uint32_t>(address + 0xE0) = baseMaxHealth;
}

uint32_t SprjChrDataModule::getBaseMaxStamina()
{
	return *accessMultilevelPointer<uint32_t>(address + 0xF8);
}

void SprjChrDataModule::setBaseMaxStamina(uint32_t baseMaxStamina)
{
	*accessMultilevelPointer<uint32_t>(address + 0xF8) = baseMaxStamina;
}

uint32_t SprjChrDataModule::getBaseMaxFP()
{
	return *accessMultilevelPointer<uint32_t>(address + 0xEC);
}

void SprjChrDataModule::setBaseMaxFP(uint32_t baseMaxFP)
{
	*accessMultilevelPointer<uint32_t>(address + 0xEC) = baseMaxFP;
}

bool SprjChrDataModule::isNoDamage()
{
	return (*accessMultilevelPointer<uint8_t>(address + 0x1C0) & (uint8_t)pow(2, 1)) == pow(2, 1);
}

void SprjChrDataModule::setNoDamage(bool value)
{
	uint8_t* newByte = accessMultilevelPointer<uint8_t>(address + 0x1C0);
	if (value) *newByte = *newByte | (uint8_t)pow(2, 1);
	else *newByte = (*newByte & ~(uint8_t)pow(2, 1));
}

bool SprjChrDataModule::isNoDead()
{
	return (*accessMultilevelPointer<uint8_t>(address + 0x1C0) & (uint8_t)pow(2, 2)) == pow(2, 2);
}

void SprjChrDataModule::setNoDead(bool value)
{
	uint8_t* newByte = accessMultilevelPointer<uint8_t>(address + 0x1C0);
	if (value) *newByte = *newByte | (uint8_t)pow(2, 2);
	else *newByte = (*newByte & ~(uint8_t)pow(2, 2));
}

bool SprjChrDataModule::isNoStaminaConsumption()
{
	return (*accessMultilevelPointer<uint8_t>(address + 0x1C0) & (uint8_t)pow(2, 4)) == pow(2, 4);
}

void SprjChrDataModule::setNoStaminaConsumption(bool value)
{
	uint8_t* newByte = accessMultilevelPointer<uint8_t>(address + 0x1C0);
	if (value) *newByte = *newByte | (uint8_t)pow(2, 4);
	else *newByte = (*newByte & ~(uint8_t)pow(2, 4));
}

bool SprjChrDataModule::isNoFPConsumption()
{
	return (*accessMultilevelPointer<uint8_t>(address + 0x1C0) & (uint8_t)pow(2, 5)) == pow(2, 5);
}

void SprjChrDataModule::setNoFPConsumption(bool value)
{
	uint8_t* newByte = accessMultilevelPointer<uint8_t>(address + 0x1C0);
	if (value) *newByte = *newByte | (uint8_t)pow(2, 5);
	else *newByte = (*newByte & ~(uint8_t)pow(2, 5));
}

}