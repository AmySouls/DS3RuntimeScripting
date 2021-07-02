#pragma once
#include "pch.h"
#include "sprj_chr_damage_module.h"
#include "databaseaddress.h"
#include "player_ins.h"
#include "ds3runtime.h"

namespace ds3runtime {

SprjChrDamageModule::SprjChrDamageModule(uintptr_t address)
{
	this->address = address;
}

void SprjChrDamageModule::applyAttackFromChr(uintptr_t attackerChr, S_ChrDamage* chrDamageData)
{
	auto hook = ds3runtime_global->accessHook("sprj_chr_damage_module_hook");
	void (*function)(...);
	*(uintptr_t*)&function = *accessMultilevelPointer<uintptr_t>(address, 0x40);
	function(address, attackerChr, chrDamageData);
}

};