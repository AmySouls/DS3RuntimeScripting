#pragma once
#include "pch.h"
#include "sprj_chr_time_act_module.h"
#include "databaseaddress.h"
#include "memory_util.h"

namespace ds3runtime {

SprjChrTimeActModule::SprjChrTimeActModule(uintptr_t address)
{
	this->address = address;
}

uint32_t SprjChrTimeActModule::getHealth()
{
	return *accessMultilevelPointer<uint32_t>(address + 0x1F90, 0x18, 0xD8);
}

void SprjChrTimeActModule::setHealth(uint32_t health)
{
	*accessMultilevelPointer<uint32_t>(address + 0x1F90, 0x18, 0xD8) = health;
}

uint32_t SprjChrTimeActModule::getStamina()
{
	return *accessMultilevelPointer<uint32_t>(address + 0x1F90, 0x18, 0xF0);
}

void SprjChrTimeActModule::setStamina(uint32_t stamina)
{
	*accessMultilevelPointer<uint32_t>(address + 0x1F90, 0x18, 0xF0) = stamina;
}

uint32_t SprjChrTimeActModule::getFP()
{
	return *accessMultilevelPointer<uint32_t>(address + 0x1F90, 0x18, 0xE4);
}

void SprjChrTimeActModule::setFP(uint32_t fp)
{
	*accessMultilevelPointer<uint32_t>(address + 0x1F90, 0x18, 0xE4) = fp;
}

uint32_t SprjChrTimeActModule::getMaxHealth()
{
	return *accessMultilevelPointer<uint32_t>(address + 0x1F90, 0x18, 0xDC);
}

void SprjChrTimeActModule::setMaxHealth(uint32_t maxHealth)
{
	*accessMultilevelPointer<uint32_t>(address + 0x1F90, 0x18, 0xDC) = maxHealth;
}

uint32_t SprjChrTimeActModule::getMaxStamina()
{
	return *accessMultilevelPointer<uint32_t>(address + 0x1F90, 0x18, 0xF4);
}

void SprjChrTimeActModule::setMaxStamina(uint32_t maxStamina)
{
	*accessMultilevelPointer<uint32_t>(address + 0x1F90, 0x18, 0xF4) = maxStamina;
}

uint32_t SprjChrTimeActModule::getMaxFP()
{
	return *accessMultilevelPointer<uint32_t>(address + 0x1F90, 0x18, 0xE8);
}

void SprjChrTimeActModule::setMaxFP(uint32_t maxFP)
{
	*accessMultilevelPointer<uint32_t>(address + 0x1F90, 0x18, 0xE8) = maxFP;
}

uint32_t SprjChrTimeActModule::getBaseMaxHealth()
{
	return *accessMultilevelPointer<uint32_t>(address + 0x1F90, 0x18, 0xE0);
}

void SprjChrTimeActModule::setBaseMaxHealth(uint32_t baseMaxHealth)
{
	*accessMultilevelPointer<uint32_t>(address + 0x1F90, 0x18, 0xE0) = baseMaxHealth;
}

uint32_t SprjChrTimeActModule::getBaseMaxStamina()
{
	return *accessMultilevelPointer<uint32_t>(address + 0x1F90, 0x18, 0xF8);
}

void SprjChrTimeActModule::setBaseMaxStamina(uint32_t baseMaxStamina)
{
	*accessMultilevelPointer<uint32_t>(address + 0x1F90, 0x18, 0xF8) = baseMaxStamina;
}

uint32_t SprjChrTimeActModule::getBaseMaxFP()
{
	return *accessMultilevelPointer<uint32_t>(address + 0x1F90, 0x18, 0xEC);
}

void SprjChrTimeActModule::setBaseMaxFP(uint32_t baseMaxFP)
{
	*accessMultilevelPointer<uint32_t>(address + 0x1F90, 0x18, 0xEC) = baseMaxFP;
}

}