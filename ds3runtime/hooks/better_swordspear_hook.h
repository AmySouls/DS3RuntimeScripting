/*
	* DS3RuntimeScripting
	* Contributers: Amir
*/

#pragma once
#include "pch.h"
#include "ds3runtime/hook.h"
#include "ds3runtime/chr_ins.h"

namespace ds3runtime {

class BetterSwordSpearHook : public Hook
{
public:
	BetterSwordSpearHook();

	std::string getName()
	{
		return "better_swordspear_hook";
	}

	static void onHitBulletLaunch(uintptr_t bulletIns, uintptr_t unk0, uintptr_t unk1, uint32_t unk2, uint8_t unk3);
private:
	static BetterSwordSpearHook* instance;
};

}