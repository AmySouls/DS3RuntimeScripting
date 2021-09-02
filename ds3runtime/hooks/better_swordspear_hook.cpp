/*
	* DS3RuntimeScripting
	* Contributers: Amir
*/

#pragma once
#include "pch.h"
#include "better_swordspear_hook.h"
#include "ds3runtime/ds3runtime.h"
#include "ds3runtime/memory_util.h"
#include "ds3runtime/bullet_spawn.h"
#include <ds3runtime/world_chr_man.h>

namespace ds3runtime {

BetterSwordSpearHook::BetterSwordSpearHook()
	: Hook(0x1409736C0, (uintptr_t)onHitBulletLaunch)
{
	instance = this;
}

void BetterSwordSpearHook::onHitBulletLaunch(uintptr_t bulletIns, uintptr_t unk0, uintptr_t unk1, uint32_t unk2, uint8_t unk3)
{
	if (*reinterpret_cast<int32_t*>(bulletIns + 0xCB8) == 100091610 && PlayerIns::isMainChrLoaded()) {
		PlayerIns chr(PlayerIns::getMainChrAddress());
		std::vector<float> playerPos = chr.getPosition();
		playerPos[1] += 2;

		WorldChrMan worldChrMan(WorldChrMan::getInstance());
		const auto targetVector = worldChrMan.getCamVector();
		
		BulletSpawn bolt;
		bolt.setOwner(0x10068000);
		bolt.setBulletId(100091610);
		bolt.setNetworkFlag(9);
		bolt.setHomingFlag(-1);
		bolt.setCoordinates(playerPos);
		bolt.setDirection(std::vector({ targetVector[0], targetVector[1], targetVector[2] }));
		bolt.launch();
		return;
	}

	int32_t (*originalFunction)(...);
	*(uintptr_t*)&originalFunction = *instance->original;
	originalFunction(bulletIns);
}

BetterSwordSpearHook* BetterSwordSpearHook::instance = nullptr;

}