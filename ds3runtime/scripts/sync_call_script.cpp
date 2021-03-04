/*
	* DS3RuntimeScripting
	* Contributers: Amir
*/

#pragma once
#include "pch.h"
#include "sync_call_script.h"
#include "ds3runtime/ds3runtime.h"

namespace ds3runtime {

void SyncCallScript::execute()
{
	for (auto bulletSpawn : syncBullets) bulletSpawn.launch();
	syncBullets.clear();
}

void SyncCallScript::launchBulletSync(BulletSpawn bulletSpawn) {
	syncBullets.push_back(bulletSpawn);
}

}