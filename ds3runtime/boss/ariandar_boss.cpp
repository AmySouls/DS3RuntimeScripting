#pragma once
#include "pch.h"
#include "ariandar_boss.h"
#include <ds3runtime/sprj_gaitem_imp.h>
#include "ds3runtime/sprj_lua_event_condition_item.h"
#include "ds3runtime/databaseaddress.h"
#include <ds3runtime/sprj_chr_data_module.h>
#include <ds3runtime/scripts/param_patcher.h>
#include <ds3runtime/hooks/play_animation_hook.h>
#include <ds3runtime/hooks/sprj_chr_damage_module_hook.h>
#include <ds3runtime/hooks/session_send_hook.h>

namespace ds3runtime {

AriandarBoss::AriandarBoss(std::shared_ptr<PlayerIns> playerIns) : StandardPlayerBoss(playerIns)
{
}

bool AriandarBoss::onAttach() {
	if (!StandardPlayerBoss::onAttach()) return false;
	/*
	//replacePlayerAnibndFile(std::filesystem::current_path().append("DS3RuntimeScripting\\mods\\boss_ariandar\\"));
	giveItemAndSwap(InventorySlot::PrimaryLeftWep,
		ItemParamIdPrefix::Weapon,
		13270010, //Canvas Talisman
		-1);
	giveItemAndSwap(InventorySlot::PrimaryRightWep,
		ItemParamIdPrefix::Weapon,
		9220005, //Dragonslayer Spear
		-1);
	giveItemAndSwap(InventorySlot::Head,
		ItemParamIdPrefix::Protector,
		67500000, //Iron Dragonslayer Helm
		-1);
	giveItemAndSwap(InventorySlot::Chest,
		ItemParamIdPrefix::Protector,
		26001000, //Sunless Armor
		-1);
	giveItemAndSwap(InventorySlot::Hands,
		ItemParamIdPrefix::Protector,
		23001000, //Black Hand Armor
		-1);
	giveItemAndSwap(InventorySlot::Legs,
		ItemParamIdPrefix::Protector,
		60500000, //Millwood Knight Helm
		-1);
	giveItemAndSwap(InventorySlot::Ring1,
		ItemParamIdPrefix::Accessory,
		20251, //Ring of the Sun's First Born
		-1);
	giveItemAndSwap(InventorySlot::Ring2,
		ItemParamIdPrefix::Accessory,
		20410, //Sun Princess Ring
		-1);
	giveItemAndSwap(InventorySlot::Ring3,
		ItemParamIdPrefix::Accessory,
		20450, //Carthus Milkring
		-1);
	giveItemAndSwap(InventorySlot::Ring4,
		ItemParamIdPrefix::Accessory,
		20200, //Leo Ring
		-1);
	giveItemAndSwap(InventorySlot::Covenant,
		ItemParamIdPrefix::Accessory,
		10090, //Spears of the Church
		-1);
	giveGoodsAndSwap(GoodsSlot::QuickItem1,
		119); //Way of White Circlet
	giveGoodsAndSwap(GoodsSlot::QuickItem2,
		296); //Undead Hunter Charm
	PlayerIns chr(*((PlayerIns*)getChr().get()));
	PlayerGameData playerGameData(chr.getPlayerGameData());
	playerGameData.setGender(PlayerGameData::Gender::Male);
	playerGameData.setClass(PlayerGameData::Class::Cleric);
	playerGameData.setAge(PlayerGameData::Age::Young);
	playerGameData.setVoice(PlayerGameData::Voice::None);
	Attributes attributes(playerGameData.getAttributes());
	attributes.vigor = 40;
	attributes.attunement = 24;
	attributes.endurance = 30;
	attributes.vitality = 17;
	attributes.strength = 20;
	attributes.dexterity = 20;
	attributes.intelligence = 10;
	attributes.faith = 40;
	attributes.luck = 13;
	attributes.soulLevel = 125;
	playerGameData.setAttributes(attributes);
	ParamHandler dragonslayerSpear("ariandar_boss", L"EquipParamWeapon", 9220000);
	dragonslayerSpear.patch(0x194, 200); //Weapon art
	auto playAnimHookSharedPtr = ds3runtime_global->accessHook("play_anim_hook");
	auto playAnimHook = (PlayAnimationHook*)playAnimHookSharedPtr.get();
	auto damageModuleHookSharedPtr = ds3runtime_global->accessHook("sprj_chr_damage_module_hook");
	auto damageModuleHook = (SprjChrDamageModuleHook*)damageModuleHookSharedPtr.get();
	auto sessionSendHookSharedPtr = ds3runtime_global->accessHook("session_send_hook");
	auto sessionSendHook = (SessionSendHook*)sessionSendHookSharedPtr.get();

	playAnimHook->installFilter("ariandar_boss_global_replaces", [this](uintptr_t hkbCharacter, int32_t animationId) -> int32_t {
		ChrIns bossChr = *getChr().get();
		
		if (bossChr.isValid() && hkbCharacter == bossChr.getHkbCharacter()) {
				if (animationId == 3821 || animationId == 3822) return 3815; //RollingLight & RollingLight_NoTrans replaced with BackStepLight
				else if (animationId == 1024) return 1076; //AttackBothHeavyKick replaced with AttackRightHeavyKick
			}

			return animationId;
			});

	playAnimHook->installFilter("ariandar_boss_phase1_combos", [this](uintptr_t hkbCharacter, int32_t animationId) -> int32_t {
		ChrIns bossChr = *getChr().get();
		
		if (bossChr.isValid() && hkbCharacter == bossChr.getHkbCharacter()) {
			if (animationId == 1847 
					&& (currentMoveTask.has_value()
						&& currentMoveTask->taskId == dragonSlayerSpear_2h_running_r1.taskId)) {
				currentMoveTask = phase1_2h_running_r1_followup_r2_charged;
				return 0;
			}
			if (animationId == 1846 
				&& (currentMoveTask.has_value()
					&& currentMoveTask->taskId == phase1_2h_running_r1_followup_r2_charged.taskId)) {
				currentMoveTask = phase1_2h_running_r1_followup_r2_uncharged;
				return 0;
			}
		}

		return animationId;
		});

	damageModuleHook->installFilter("ariandar_boss_global_replaces", [&](uintptr_t sprjChrDamageModule, uintptr_t attackerChr, char* attackDataBuffer) -> bool {
		int32_t* attackIdPtr = (int32_t*)(attackDataBuffer + 0x44);
		if (*attackIdPtr == 3600030) *attackIdPtr = 2613305; //dragon slayer spear 2h kick r2 replaced with lothric knight greatsword 2h r2 1 charged
		else if (*attackIdPtr == 3600530) *attackIdPtr = 2600000; //dragon slayer spear 2h running r1 replaced with lothric knight greatsword 2h r1 1
		else if (*attackIdPtr == 3600110) *attackIdPtr = 2600701; //dragon slayer spear 2h l2 r1 replaced with lothric knight greatsword l2 r2
		else if (*attackIdPtr == 3603300) *attackIdPtr = 2613300; //dragon slayer spear 2h r2 1 uncharged replaced with lothric knight greatsword 2h r2 1 uncharged
		else if (*attackIdPtr == 3603305) *attackIdPtr = 2600010; //dragon slayer spear 2h r2 1 charged replaced with lothric knight greatsword 2h r1 2
		return true;
		});

	*/
	return true;
}

void AriandarBoss::onDetach()
{
	/*
	auto playAnimHookSharedPtr = ds3runtime_global->accessHook("play_anim_hook");
	auto playAnimHook = (PlayAnimationHook*)playAnimHookSharedPtr.get();
	auto damageModuleHookSharedPtr = ds3runtime_global->accessHook("sprj_chr_damage_module_hook");
	auto damageModuleHook = (SprjChrDamageModuleHook*)damageModuleHookSharedPtr.get();
	auto sessionSendHookSharedPtr = ds3runtime_global->accessHook("session_send_hook");
	auto sessionSendHook = (SessionSendHook*)damageModuleHookSharedPtr.get();
	playAnimHook->uninstallFilter("ariandar_boss_global_replaces");
	playAnimHook->uninstallFilter("ariandar_boss_phase1_combos");
	damageModuleHook->uninstallFilter("ariandar_boss_global_replaces");
	PlayerIns chr(*((PlayerIns*)getChr().get()));

	if (chr.isValid()) {
		SprjChrDataModule chrData(chr.getSprjChrDataModule());
		if (chrData.isNoStaminaConsumption()) chrData.setNoStaminaConsumption(false);
	}
	
	((ParamPatcher*)ds3runtime_global->accessScript("param_patcher").get())->restore("ariandar_boss");
	//restoreVannilaPlayerAnibndFile();
	*/
	StandardPlayerBoss::onDetach();
}

void AriandarBoss::logic()
{
	/*
	std::optional<int32_t> animationId = getAnimationId();

	if (!currentMoveTask.has_value() && animationId.has_value()) {
		for (auto* task : allBossTasks) {
			if (animationId.value() != -1 && animationId.value() != task->baseAnimationId) continue;
			currentMoveTask = *task;
			break;
		}
	}

	if (currentMoveTask.has_value()) {
		std::optional<std::string> taskTransfer = currentMoveTask.value().task(this, &currentMoveTask.value());

		if (taskTransfer.has_value() && taskTransfer.value() == "") {
			currentMoveTask.reset();
		}
		else if (taskTransfer.has_value()) {
			for (auto* task : allBossTasks) {
				if (taskTransfer.value() != task->taskId) continue;
				currentMoveTask = *task;
				break;
			}
		}
	}
	*/
}

void AriandarBoss::checks()
{
	/*
	PlayerIns chr(*((PlayerIns*)getChr().get()));
	if (!chr.isValid()) return;
	SprjChrDataModule chrData(chr.getSprjChrDataModule());

	if (chr.getWeightIndex() != 1) chr.setWeightIndex(1);
	if (chrData.getFP() < chrData.getMaxFP()) chrData.setFP(fmin((float)chrData.getMaxFP(), chrData.getFP() + fmax(1.0f, chrData.getMaxFP() / 100.0f)));
	if (chrData.getBaseMaxHealth() != 8000) chrData.setBaseMaxHealth(8000);
	if (!chrData.isNoStaminaConsumption()) chrData.setNoStaminaConsumption(true);
	*/
}

/*
BossTask AriandarBoss::dragonSlayerSpear_2h_running_r1 = BossTask("phase1_2h_running_r1", 36034500, [&](StandardPlayerBoss* boss, BossTask* bossTask) -> std::optional<std::string> {
	if (!boss->isAnimationPresent(bossTask->baseAnimationId)) {
		return "";
	}

	bossTask->tick++;
	return {};
	});

BossTask AriandarBoss::phase1_2h_running_r1_followup_r2_charged = BossTask("phase1_2h_running_r1_followup_r2_charged", -1, [&](StandardPlayerBoss* boss, BossTask* bossTask) -> std::optional<std::string> {
	if (!boss->isAnimationPresent(36034500) && !boss->isAnimationPresent(252034320)) {
		return "";
	}
	
	if (bossTask->tick == 0) {
		boss->giveItemAndSwap(InventorySlot::PrimaryRightWep,
			ItemParamIdPrefix::Weapon,
			9240000, //Ringed Knight Spear
			-1);
	}
	else if (bossTask->tick == 3) {
		ChrIns bossChr = *boss->getChr();
		bossChr.playAnimation(L"W_AttackBothHeavy1Start");
	}
	else if (bossTask->tick == 6) {
		boss->giveItemAndSwap(InventorySlot::PrimaryRightWep,
			ItemParamIdPrefix::Weapon,
			9220005, //Dragonslayer Spear
			-1);
	}

	bossTask->tick++;
	return {};
	});

BossTask AriandarBoss::phase1_2h_running_r1_followup_r2_uncharged = BossTask("phase1_2h_running_r1_followup_r2_uncharged", -1, [&](StandardPlayerBoss* boss, BossTask* bossTask) -> std::optional<std::string> {
	if (!boss->isAnimationPresent(252034320) && !boss->isAnimationPresent(252034321)) {
		return "";
	}

	if (bossTask->tick == 0) {
		boss->giveItemAndSwap(InventorySlot::PrimaryRightWep,
			ItemParamIdPrefix::Weapon,
			9240000, //Ringed Knight Spear
			-1);
	}
	else if (bossTask->tick == 3) {
		ChrIns bossChr = *boss->getChr();
		bossChr.playAnimation(L"W_AttackBothHeavy1End");
	}
	else if (bossTask->tick == 6) {
		boss->giveItemAndSwap(InventorySlot::PrimaryRightWep,
			ItemParamIdPrefix::Weapon,
			9220005, //Dragonslayer Spear
			-1);
	}

	bossTask->tick++;
	return {};
	});

BossTask AriandarBoss::dragonSlayerSpear_2h_kick_r2 = BossTask("phase1_2h_kick_r2", 36030600, [&](StandardPlayerBoss* boss, BossTask* bossTask) -> std::optional<std::string> {
	if (!boss->isAnimationPresent(bossTask->baseAnimationId)) {
		return "";
	}

	ChrIns bossChr = *boss->getChr();
	auto position = bossChr.getPosition();

	if (bossTask->tick >= 16 && bossTask->tick < 32) {
		const float verticalMoveScale = sin((bossTask->tick - 16) / 32.0f * M_PI * 2) * .15;
		position[1] += verticalMoveScale;
		bossChr.setPosition(position);
	}
	else if (bossTask->tick >= 32 && bossTask->tick < 44) {
		const float verticalMoveScale = sin((bossTask->tick - 32) / 24.0f * M_PI * 2) * .04;
		position[1] += verticalMoveScale;
		bossChr.setPosition(position);
	}
	else if (bossTask->tick >= 44 && bossTask->tick < 56) {
		const float verticalMoveScale = sin((bossTask->tick - 32) / 24.0f * M_PI * 2) * .4;
		position[1] += verticalMoveScale;
		bossChr.setPosition(position);
	}

	bossTask->tick++;
	return {};
	});

std::vector<BossTask*> AriandarBoss::allBossTasks = {
	&dragonSlayerSpear_2h_running_r1,
	&phase1_2h_running_r1_followup_r2_charged,
	&phase1_2h_running_r1_followup_r2_uncharged,
	&dragonSlayerSpear_2h_kick_r2,
};
*/

}