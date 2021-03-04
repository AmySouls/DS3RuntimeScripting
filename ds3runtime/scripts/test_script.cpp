/*
	* DS3RuntimeScripting
	* Contributers: Amir
*/

#pragma once
#include "pch.h"
#include "test_script.h"
#include "ds3runtime/ds3runtime.h"
#include "ds3runtime/bullet_spawn.h"
#include "sync_call_script.h"
#include "ds3runtime/hooks/animation_id_hook.h"

namespace ds3runtime {

void TestScript::execute()
{
	uintptr_t* chrAddrPtr = accessMultilevelPointer<uintptr_t>(getDataBaseAddress(DataBaseAddress::WorldChrMan), 0x80);
	if (!chrAddrPtr) return;
	uintptr_t chrAddr = PlayerIns::getMainChrAddress();
	if (!PlayerIns::isMainChr(chrAddr)) return;
	PlayerIns chr(chrAddr);
	const int32_t animationId = ((AnimationIdHook*)ds3runtime_global->accessHook("animation_id_hook").get())->getAnimationId(chr);
	
	if (false) {
		BulletSpawn bulletSpawn = BulletSpawn();
		bulletSpawn.setOwner((int)ChrIns::Handle::MainChr);
		bulletSpawn.setBulletId(30);
		bulletSpawn.setHomingFlag(-1);
		bulletSpawn.setCoordinates(chr.getPosition());
		bulletSpawn.setDirection(std::vector<float>({ -sin(chr.getAngle()), 0, -cos(chr.getAngle()) }));
		((SyncCallScript*)ds3runtime_global->accessScript("sync_call_script").get())->launchBulletSync(bulletSpawn);
	}

	lastAnimationString = chr.getAnimationString();
}

}