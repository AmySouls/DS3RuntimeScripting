#pragma once
#include "pch.h"
#include "ds1_passive_poise.h"
#include "ds3runtime/ds3runtime.h"
#include "param_patcher.h"
#include <ds3runtime/world_chr_man.h>

namespace ds3runtime {

bool DS1PassivePoise::onAttach()
{
	for (int32_t id : ((ParamPatcher*)ds3runtime_global->accessScript("param_patcher"))->getIdsInParam(L"EquipParamProtector")) {
		const static float GLOBAL_POISE_MULTIPLYER = 1.13f;
		ParamHandler armor("ds1_passive_poise", L"EquipParamProtector", id);
		uint16_t newValue = static_cast<uint16_t>(GLOBAL_POISE_MULTIPLYER * 100 * (1 - armor.read<float>(0x110)));
		armor.patch<uint16_t>(0xAE, newValue); //Write poise value into saDurablity value
	}

	for (int32_t id : ((ParamPatcher*)ds3runtime_global->accessScript("param_patcher"))->getIdsInParam(L"EquipParamWeapon")) {
		ParamHandler weapon("ds1_passive_poise", L"EquipParamWeapon", id);
		weapon.patch<float>(0x174, 0); //Write base poise value to nothing
	}

	return true;
}

void DS1PassivePoise::execute()
{
	if (!WorldChrMan::hasInstance() || !PlayerIns::isMainChrLoaded() || !PlayerIns(PlayerIns::getMainChrAddress()).hasHkbCharacter()) return;
	PlayerIns mainChr(PlayerIns::getMainChrAddress());
	WorldChrMan worldChrMan(WorldChrMan::getInstance());

	if (!worldChrMan.doesChrHaveSpEffect(mainChr.getAddress(), 400000)) {
		mainChr.applySpEffect(400000);
	}
}

bool DS1PassivePoise::onDetach()
{
	((ParamPatcher*)ds3runtime_global->accessScript("param_patcher"))->restore("ds1_passive_poise");
	return true;
}

}