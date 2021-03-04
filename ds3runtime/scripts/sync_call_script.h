/*
	* DS3RuntimeScripting
	* Contributers: Amir
*/

#pragma once
#include "pch.h"
#include "ds3runtime/script_module.h"
#include "ds3runtime/bullet_spawn.h"

namespace ds3runtime {

class SyncCallScript : public ScriptModule
{
public:
	void execute();

	std::string getName()
	{
		return "sync_call_script";
	}

	void launchBulletSync(BulletSpawn bulletSpawn);
private:
	std::vector<BulletSpawn> syncBullets;
};

}