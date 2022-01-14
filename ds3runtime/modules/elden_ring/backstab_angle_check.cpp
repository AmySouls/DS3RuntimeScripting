#pragma once
#include "pch.h"
#include "backstab_angle_check.h"
#include "ds3runtime/ds3runtime.h"
#include <ds3runtime/world_chr_man.h>
#include <ds3runtime/hooks/throw_hook.h>

namespace ds3runtime::eldenring {

bool BackstabAngleCheck::onAttach()
{
	((ThrowHook*)ds3runtime_global
		->accessHook("throw_hook"))
		->installFilter("backstab_angle_check", [&](uintptr_t, uintptr_t throwerChr, uintptr_t victimChr, int32_t id) -> int32_t {
		if (id != 100 && id != 1000100) return id;
		ChrIns throwerChrIns(*reinterpret_cast<uintptr_t*>(throwerChr));
		ChrIns victimChrIns(*reinterpret_cast<uintptr_t*>(victimChr));
		if (!throwerChrIns.isValid() || !victimChrIns.isValid()) return id;
		const float angle1 = throwerChrIns.getAngle();
		const float angle2 = victimChrIns.getAngle();
		const float difference = static_cast<float>(fmod(((static_cast<double>(angle1) - static_cast<double>(angle2)) + M_PI), (M_PI * 2)) - M_PI);

		if (std::abs(difference) * (180 / M_PI) > 35) {
			return 0;
		}

		return id;
		});

	return true;
}

void BackstabAngleCheck::execute()
{
}

bool BackstabAngleCheck::onDetach()
{
	((ThrowHook*)ds3runtime_global
		->accessHook("throw_hook"))
		->uninstallFilter("backstab_angle_check");
	return true;
}

}