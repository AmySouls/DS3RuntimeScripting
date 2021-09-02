#pragma once
#include "pch.h"
#include "ds1_passive_poise.h"
#include "ds3runtime/ds3runtime.h"
#include "param_patcher.h"
#include <ds3runtime/world_chr_man.h>

namespace ds3runtime {

bool DS1PassivePoise::onAttach()
{
	//for (int32_t id : ((ParamPatcher*)ds3runtime_global->accessHook("param_patcher"))->getIdsInParam(L"EquipParamProtector")) {
		//ParamHandler drakeBloodKnight("ds1_passive_poise", L"EquipParamProtector", id);
		//drakeBloodKnight.patch<uint16_t>(0xAE, 99); 
	//}

	return true;
}

void DS1PassivePoise::execute()
{

}

bool DS1PassivePoise::onDetach()
{
	//((ParamPatcher*)ds3runtime_global->accessHook("param_patcher"))->restore("ds1_passive_poise");
	return true;
}

}