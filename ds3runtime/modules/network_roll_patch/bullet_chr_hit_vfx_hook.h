/*
	* DS3RuntimeScripting
	* Contributers: Amir
*/

#pragma once
#include "pch.h"
#include "ds3runtime/hook.h"

namespace ds3runtime::ds3IFramePatch {

class BulletChrHitVfxHook : public Hook
{
public:
	BulletChrHitVfxHook();

	std::string getName()
	{
		return "bullet_chr_hit_vfx_hook";
	}

	static void onSpawnVfx(uintptr_t sprjBulletIns, uintptr_t unk0, uintptr_t chrIns);
private:
	static BulletChrHitVfxHook* instance;
};

}