#pragma once
#include "pch.h"
#include "npc_mod_test.h"
#include "ds3runtime/ds3runtime.h"
#include <ds3runtime/scripts/param_patcher.h>
#include <ds3runtime/world_chr_man.h>

namespace ds3runtime {

bool NPCModTest::onAttach()
{
	ParamHandler drakeBloodKnight("npc_mod_test", L"CharaInitParam", 28800);
	drakeBloodKnight.patch("equip_Wep_Right", 4070005);
	drakeBloodKnight.patch("equip_Wep_Left", 21100005);
	drakeBloodKnight.patch("equip_Helm", 88000000);
	drakeBloodKnight.patch("equip_Armor", 88001000);
	drakeBloodKnight.patch("equip_Gaunt", 88002000);
	drakeBloodKnight.patch("equip_Leg", 88003000);
	return true;
}

void NPCModTest::execute()
{
}

bool NPCModTest::onDetach()
{
	((ParamPatcher*)ds3runtime_global->accessScript("param_patcher"))->restore("npc_mod_test");
	return true;
}

}