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
#include <ds3runtime/game_data_man.h>
#include <ds3runtime/ds3_debug_variables.h>
#include "ds3runtime/bullet_spawn.h"
#include "ds3runtime/world_chr_man.h"

namespace ds3runtime {

bool AriandarBoss::onAttach() {

	if (!getChrAddress().has_value() || !PlayerIns(getChrAddress().value()).hasHkbCharacter()) return false;
	PlayerIns chr(getChrAddress().value());
	if (!StandardPlayerBoss::onAttach()) return false;
	DS3DebugVariables().setDebugMovementMultiplier(true);
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
		92002000, //Wolf Knight Gauntlets
		-1);
	giveItemAndSwap(InventorySlot::Legs,
		ItemParamIdPrefix::Protector,
		93003000, //Dragonslayer Leggings
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
		119, 1); //Way of White Circlet
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
	dragonslayerSpear.patch<int32_t>(0x194, 200); //Weapon art
	dragonslayerSpear.patch<uint8_t>(0x178, 26); //dualWeaponType -> dual hand
	dragonslayerSpear.patch<float>(0x174, .163f); //ToughnessDamageRate -> ~Greatlance Poise
	ParamHandler dragonSlayerSwordSpear_2h_r1_1_atk("ariandar_boss", L"AtkParam_Pc", 3611200);
	ParamHandler dragonSlayerSpear_2h_r1_3_atk("ariandar_boss", L"AtkParam_Pc", 3600220);
	dragonSlayerSpear_2h_r1_3_atk.patch<float>(0x4, dragonSlayerSwordSpear_2h_r1_1_atk.read<float>(0x4));
	dragonSlayerSpear_2h_r1_3_atk.patch<float>(0x8, dragonSlayerSwordSpear_2h_r1_1_atk.read<float>(0x8));
	dragonSlayerSpear_2h_r1_3_atk.patch<float>(0xC, dragonSlayerSwordSpear_2h_r1_1_atk.read<float>(0xC));
	auto playAnimHook = (PlayAnimationHook*)ds3runtime_global->accessHook("play_anim_hook");
	auto damageModuleHook = (SprjChrDamageModuleHook*)ds3runtime_global->accessHook("sprj_chr_damage_module_hook");
	auto sessionSendHook = (SessionSendHook*)ds3runtime_global->accessHook("session_send_hook");

	playAnimHook->installFilter("ariandar_boss_global_replaces", [this](uintptr_t hkbCharacter, int32_t animationId) -> int32_t {
		if (!getChrAddress().has_value()) return animationId;
		ChrIns bossChr(getChrAddress().value());
		
		if (bossChr.isValid() && hkbCharacter == bossChr.getHkbCharacter()) {
			if (animationId == 1423 || animationId == 1425) return 0; //GuardStart & GuardEnd replaced with nothing
			else if (animationId == 3821 || animationId == 3822) return 3815; //RollingLight & RollingLight_NoTrans replaced with BackStepLight
			else if (animationId == 1024) return 1076; //AttackBothHeavyKick replaced with AttackRightHeavyKick
			else if (animationId == 3775 || animationId == 3393) return 0; //Dash180 replaced with nothing
		}

		return animationId;
		});

	getComboSystem()->registerNeutralCombo(stdcombo::NeutralCombo("Neutral_l1", L"W_AttackBothLeft1", "Phase1_DSSS_2h_r1_1"));

	getComboSystem()->registerNeutralCombo(stdcombo::NeutralCombo("Neutral_running_l1", L"W_AttackBothLeftDash", "Phase1_BKG_2h_r2_1_charged"));

	getComboSystem()->registerBossTask(stdcombo::BossTask("Phase1_BKG_2h_r2_1_charged", -1, [this](stdcombo::BossTask* bossTask) -> std::optional<std::string> {
		if ((!bossTask->comboSource.has_value() || !isAnimationPresent(bossTask->comboSource.value().animationId)) 
			&& !isAnimationPresent(112034320)) {
			return "";
		}

		if (bossTask->tick == 0) {
			updateChainComboMove(bossTask->taskId);
			giveItemAndSwap(InventorySlot::PrimaryRightWep,
				ItemParamIdPrefix::Weapon,
				10150005, //Black Knight Glaive
				-1);
			setSheathState(3);
		}
		else if (bossTask->tick == 3) {
			ChrIns bossChr(getChrAddress().value());
			bossChr.playAnimation(1847);
		}
		else if (bossTask->tick == 6) {
			giveItemAndSwap(InventorySlot::PrimaryRightWep,
				ItemParamIdPrefix::Weapon,
				9220005, //Dragonslayer Spear
				-1);
			setSheathState(3);
		}

		bossTask->tick++;
		return {};
		}, {
			{ L"W_AttackBothHeavy1End", "Phase1_BKG_2h_r2_1_uncharged" },
		}));

	getComboSystem()->registerBossTask(stdcombo::BossTask("Phase1_BKG_2h_r2_1_uncharged", -1, [this](stdcombo::BossTask* bossTask) -> std::optional<std::string> {
		if ((!bossTask->comboSource.has_value() || !isAnimationPresent(bossTask->comboSource.value().animationId)) 
			&& !isAnimationPresent(112034321)) {
			return "";
		}

		if (bossTask->tick == 0) {
			updateChainComboMove(bossTask->taskId);
			giveItemAndSwap(InventorySlot::PrimaryRightWep,
				ItemParamIdPrefix::Weapon,
				10150005, //Black Knight Glaive
				-1);
			setSheathState(3);
		}
		else if (bossTask->tick == 3) {
			ChrIns bossChr(getChrAddress().value());
			bossChr.playAnimation(1846);
		}
		else if (bossTask->tick == 6) {
			giveItemAndSwap(InventorySlot::PrimaryRightWep,
				ItemParamIdPrefix::Weapon,
				9220005, //Dragonslayer Spear
				-1);
			setSheathState(3);
		}

		bossTask->tick++;
		return {};
		}, {
			{ L"W_AttackBothLight1", "Phase1_DSSS_2h_r2_1_uncharged" },
		}));

	getComboSystem()->registerBossTask(stdcombo::BossTask("Phase1_DSS_2h_r1_1", 36034000, [this](stdcombo::BossTask* bossTask) -> std::optional<std::string> {
		if ((!bossTask->comboSource.has_value() || !isAnimationPresent(bossTask->comboSource.value().animationId)) 
				&& !isAnimationPresent(bossTask->baseAnimationId)) {
			return "";
		}

		if (bossTask->tick == 0) {
			updateChainComboMove(bossTask->taskId);
		}

		bossTask->tick++;
		return {};
		}, {
			{ L"W_AttackBothHeavy1SubStart", "Phase1_DSSS_2h_r2_1_start" },
			{ L"W_AttackBothLeft2", "Phase1_RGCS_L2" },
		}));

	getComboSystem()->registerBossTask(stdcombo::BossTask("Phase1_DSS_2h_r1_2", 36034010, [this](stdcombo::BossTask* bossTask) -> std::optional<std::string> {
		if ((!bossTask->comboSource.has_value() || !isAnimationPresent(bossTask->comboSource.value().animationId)) 
				&& !isAnimationPresent(bossTask->baseAnimationId)) {
			return "";
		}

		if (bossTask->tick == 0) {
			updateChainComboMove(bossTask->taskId);
		}

		bossTask->tick++;
		return {};
		}, {
			{ L"W_AttackBothLeft3", "Phase1_RGCS_L2" },
			{ L"W_AttackBothLeft3", "Phase1_DSSS_2h_r1_2" },
		}));

	getComboSystem()->registerBossTask(stdcombo::BossTask("Phase1_DSS_2h_r1_3", 36034020, [this](stdcombo::BossTask* bossTask) -> std::optional<std::string> {
		if ((!bossTask->comboSource.has_value() || !isAnimationPresent(bossTask->comboSource.value().animationId)) 
				&& !isAnimationPresent(bossTask->baseAnimationId)) {
			return "";
		}

		if (bossTask->tick == 0) {
			updateChainComboMove(bossTask->taskId);
		}

		bossTask->tick++;
		return {};
		}, {
			{ L"W_AttackBothHeavy1SubStart", "Phase1_DSSS_2h_r2_1_start" },
			{ L"W_AttackBothLeft2", "Phase1_RGCS_L2" },
		}));

	getComboSystem()->registerBossTask(stdcombo::BossTask("Phase1_RGCS_L2", -1, [this](stdcombo::BossTask* bossTask) -> std::optional<std::string> {
		if ((!bossTask->comboSource.has_value() || !isAnimationPresent(bossTask->comboSource.value().animationId)) 
				&& !isAnimationPresent(36034010) && !isAnimationPresent(151036400)) {
			return "";
		}

		if (bossTask->tick == 0) {
			updateChainComboMove(bossTask->taskId);
			giveItemAndSwap(InventorySlot::PrimaryRightWep,
				ItemParamIdPrefix::Weapon,
				2120000, //Rotten Ghru Curved Sword
				-1);
			setSheathState(3);
		}
		else if (bossTask->tick == 3) {
			ChrIns bossChr(getChrAddress().value());
			bossChr.playAnimation(2314);
			bossChr.playAnimation(3446);
		}
		else if (bossTask->tick == 6) {
			giveItemAndSwap(InventorySlot::PrimaryRightWep,
				ItemParamIdPrefix::Weapon,
				9220005, //Dragonslayer Spear
				-1);
			setSheathState(3);
		}

		bossTask->tick++;
		return {};
		}, {
			{ L"W_AttackSpinHeavy", "Phase1_RGCS_L2_R2" },
		}));

	getComboSystem()->registerBossTask(stdcombo::BossTask("Phase1_RGCS_L2_R2", -1, [this](stdcombo::BossTask* bossTask) -> std::optional<std::string> {
		if ((!bossTask->comboSource.has_value() || !isAnimationPresent(bossTask->comboSource.value().animationId)) 
				&& !isAnimationPresent(151036420)) {
			return "";
		}

		if (bossTask->tick == 0) {
			updateChainComboMove(bossTask->taskId);
			giveItemAndSwap(InventorySlot::PrimaryRightWep,
				ItemParamIdPrefix::Weapon,
				2120000, //Rotten Ghru Curved Sword
				-1);
			setSheathState(3);
		}
		else if (bossTask->tick == 3) {
			ChrIns bossChr(getChrAddress().value());
			bossChr.playAnimation(2312);
		}
		else if (bossTask->tick == 6) {
			giveItemAndSwap(InventorySlot::PrimaryRightWep,
				ItemParamIdPrefix::Weapon,
				9220005, //Dragonslayer Spear
				-1);
			setSheathState(3);
		}

		ChrIns bossChr(getChrAddress().value());
		auto position = bossChr.getPosition();

		if (bossTask->tick >= 16 && bossTask->tick < 32) {
			const float verticalMoveScale = sin((bossTask->tick - 16) / 32.0f * static_cast<float>(M_PI) * 2) * .1f;
			position[1] += verticalMoveScale;
			bossChr.setPosition(position);
		}
		if (bossTask->tick >= 50 && bossTask->tick < 60) {
			const float verticalMoveScale = sin((bossTask->tick - 40) / 20.0f * static_cast<float>(M_PI) * 2) * .16f;
			position[1] += verticalMoveScale;
			bossChr.setPosition(position);
		}

		bossTask->tick++;
		return {};
		}, {
			{ L"W_AttackBothHeavy2Start", "Phase1_RKS_L2_R2_2" },
		}));

	getComboSystem()->registerBossTask(stdcombo::BossTask("Phase1_DSSS_2h_r2_1_start", -1, [this](stdcombo::BossTask* bossTask) -> std::optional<std::string> {
		spdlog::debug("Combo source?: {} | Is anim present 1?: {} | Is Anim present 2?: {}", bossTask->comboSource.has_value(), isAnimationPresent(bossTask->comboSource.value().animationId), isAnimationPresent(36034320));

		if ((!bossTask->comboSource.has_value() || !isAnimationPresent(bossTask->comboSource.value().animationId)) 
				&& !isAnimationPresent(36034320)) {
			return "";
		}

		if (bossTask->tick == 0) {
			updateChainComboMove(bossTask->taskId);
			giveItemAndSwap(InventorySlot::PrimaryRightWep,
				ItemParamIdPrefix::Weapon,
				9160000, //Dragonslayer Swordspear
				-1);
			setSheathState(3);
		}
		else if (bossTask->tick == 3) {
			ChrIns bossChr(getChrAddress().value());
			bossChr.playAnimation(1847);
		}
		else if (bossTask->tick == 6) {
			giveItemAndSwap(InventorySlot::PrimaryRightWep,
				ItemParamIdPrefix::Weapon,
				9220005, //Dragonslayer Spear
				-1);
			setSheathState(3);
		}

		bossTask->tick++;
		return {};
		}, {
			{ L"W_AttackBothLight1", "Phase1_DSSS_2h_r1_2" },
			{ L"W_AttackBothHeavy1End", "Phase1_DSSS_2h_r2_1_end" },
			{ L"W_AttackBothHeavy2Start", "Phase1_RKS_2h_r2_1_start" },
			{ L"W_AttackBothLeft1", "Phase1_GUGS_2h_r1_3" },
		}));

	getComboSystem()->registerBossTask(stdcombo::BossTask("Phase1_DSSS_2h_r2_1_end", -1, [this](stdcombo::BossTask* bossTask) -> std::optional<std::string> {
		if ((!bossTask->comboSource.has_value() || !isAnimationPresent(bossTask->comboSource.value().animationId)) 
				&& !isAnimationPresent(36034321) && !isAnimationPresent(36034321)) {
			return "";
		}

		if (bossTask->tick == 0) {
			updateChainComboMove(bossTask->taskId);
			giveItemAndSwap(InventorySlot::PrimaryRightWep,
				ItemParamIdPrefix::Weapon,
				9160000, //Dragonslayer Swordspear
				-1);
			setSheathState(3);
		}
		else if (bossTask->tick == 3) {
			ChrIns bossChr(getChrAddress().value());
			bossChr.playAnimation(1846);
		}
		else if (bossTask->tick == 6) {
			giveItemAndSwap(InventorySlot::PrimaryRightWep,
				ItemParamIdPrefix::Weapon,
				9220005, //Dragonslayer Spear
				-1);
			setSheathState(3);
		}

		bossTask->tick++;
		return {};
		}, {
			{ L"W_AttackBothLight1", "Phase1_DSSS_2h_r1_2" },
			{ L"W_AttackBothHeavy2Start", "Phase1_RKS_2h_r2_1_start" },
			{ L"W_AttackBothLeft1", "Phase1_GUGS_2h_r1_3" },
		}));

	getComboSystem()->registerBossTask(stdcombo::BossTask("Phase1_DSS_2h_running_r1", 36034500, [this](stdcombo::BossTask* bossTask) -> std::optional<std::string> {
		if ((!bossTask->comboSource.has_value() || !isAnimationPresent(bossTask->comboSource.value().animationId)) 
				&& !isAnimationPresent(bossTask->baseAnimationId)) {
			return "";
		}

		if (bossTask->tick == 0) {
			updateChainComboMove(bossTask->taskId);
		}

		bossTask->tick++;
		return {};
		}, {
			{ L"W_AttackBothLight1", "Phase1_RKS_l2_r2_1" },
			{ L"W_AttackBothHeavy1Start", "Phase1_RKS_2h_r2_1_start" },
			{ L"W_AttackBothLeft1", "Phase1_GUGS_2h_r1_3" },
		}));

	getComboSystem()->registerBossTask(stdcombo::BossTask("Phase1_RKS_2h_r2_1_start", -1, [this](stdcombo::BossTask* bossTask) -> std::optional<std::string> {
		if ((!bossTask->comboSource.has_value() || !isAnimationPresent(bossTask->comboSource.value().animationId)) 
				&& !isAnimationPresent(252034320)) {
			return "";
		}

		if (bossTask->tick == 0) {
			updateChainComboMove(bossTask->taskId);
			giveItemAndSwap(InventorySlot::PrimaryRightWep,
				ItemParamIdPrefix::Weapon,
				9240000, //Ringed Knight Spear
				-1);
			setSheathState(3);
		}
		else if (bossTask->tick == 3) {
			ChrIns bossChr(getChrAddress().value());
			bossChr.playAnimation(1847);
		}
		else if (bossTask->tick == 6) {
			giveItemAndSwap(InventorySlot::PrimaryRightWep,
				ItemParamIdPrefix::Weapon,
				9220005, //Dragonslayer Spear
				-1);
			setSheathState(3);
		}

		bossTask->tick++;
		return {};
		}, {
			{ L"W_AttackBothLight1", "Phase1_DSSS_2h_r1_2" },
			{ L"W_AttackBothHeavy1End", "Phase1_RKS_2h_r2_1_end" },
			{ L"W_AttackBothHeavy2Start", "Phase1_RKS_2h_r2_2_start" },
		}));

	getComboSystem()->registerBossTask(stdcombo::BossTask("Phase1_RKS_2h_r2_1_end", -1, [this](stdcombo::BossTask* bossTask) -> std::optional<std::string> {
		if ((!bossTask->comboSource.has_value() || !isAnimationPresent(bossTask->comboSource.value().animationId)) 
				&& !isAnimationPresent(252034321)) {
			return "";
		}

		if (bossTask->tick == 0) {
			updateChainComboMove(bossTask->taskId);
			giveItemAndSwap(InventorySlot::PrimaryRightWep,
				ItemParamIdPrefix::Weapon,
				9240000, //Ringed Knight Spear
				-1);
			setSheathState(3);
		}
		else if (bossTask->tick == 3) {
			ChrIns bossChr(getChrAddress().value());
			bossChr.playAnimation(1846);
		}
		else if (bossTask->tick == 6) {
			giveItemAndSwap(InventorySlot::PrimaryRightWep,
				ItemParamIdPrefix::Weapon,
				9220005, //Dragonslayer Spear
				-1);
			setSheathState(3);
		}

		bossTask->tick++;
		return {};
		}, {
			{ L"W_AttackBothLight1", "Phase1_DSSS_2h_r1_2" },
			{ L"W_AttackBothHeavy2Start", "Phase1_RKS_2h_r2_2_start" },
		}));

	getComboSystem()->registerBossTask(stdcombo::BossTask("Phase1_RKS_2h_r2_2_start", -1, [this](stdcombo::BossTask* bossTask) -> std::optional<std::string> {
		if ((!bossTask->comboSource.has_value() || !isAnimationPresent(bossTask->comboSource.value().animationId)) 
				&& !isAnimationPresent(252034340)) {
			return "";
		}

		if (bossTask->tick == 0) {
			updateChainComboMove(bossTask->taskId);
			giveItemAndSwap(InventorySlot::PrimaryRightWep,
				ItemParamIdPrefix::Weapon,
				9240000, //Ringed Knight Spear
				-1);
			setSheathState(3);
		}
		else if (bossTask->tick == 3) {
			ChrIns bossChr(getChrAddress().value());
			bossChr.playAnimation(1849);
		}
		else if (bossTask->tick == 6) {
			giveItemAndSwap(InventorySlot::PrimaryRightWep,
				ItemParamIdPrefix::Weapon,
				9220005, //Dragonslayer Spear
				-1);
			setSheathState(3);
		}

		bossTask->tick++;
		return {};
		}, {
			{ L"W_AttackBothHeavy2End", "Phase1_RKS_2h_r2_2_end" },
		}));

	getComboSystem()->registerBossTask(stdcombo::BossTask("Phase1_RKS_2h_r2_2_end", -1, [this](stdcombo::BossTask* bossTask) -> std::optional<std::string> {
		if ((!bossTask->comboSource.has_value() || !isAnimationPresent(bossTask->comboSource.value().animationId)) 
				&& !isAnimationPresent(252034341)) {
			return "";
		}

		if (bossTask->tick == 0) {
			updateChainComboMove(bossTask->taskId);
			giveItemAndSwap(InventorySlot::PrimaryRightWep,
				ItemParamIdPrefix::Weapon,
				9240000, //Ringed Knight Spear
				-1);
			setSheathState(3);
		}
		else if (bossTask->tick == 3) {
			ChrIns bossChr(getChrAddress().value());
			bossChr.playAnimation(1848);
		}
		else if (bossTask->tick == 6) {
			giveItemAndSwap(InventorySlot::PrimaryRightWep,
				ItemParamIdPrefix::Weapon,
				9220005, //Dragonslayer Spear
				-1);
			setSheathState(3);
		}

		bossTask->tick++;
		return {};
		}, {}));

	getComboSystem()->registerBossTask(stdcombo::BossTask("Phase1_2h_kick_r2", 36030600, [this](stdcombo::BossTask* bossTask) -> std::optional<std::string> {
		if ((!bossTask->comboSource.has_value() || !isAnimationPresent(bossTask->comboSource.value().animationId)) 
				&& !isAnimationPresent(bossTask->baseAnimationId)) {
			return "";
		}

		ChrIns bossChr(getChrAddress().value());
		auto position = bossChr.getPosition();

		if (bossTask->tick == 0) {
			updateChainComboMove(bossTask->taskId);
		}

		if (bossTask->tick >= 16 && bossTask->tick < 32) {
			const float verticalMoveScale = sin((bossTask->tick - 16) / 32.0f * static_cast<float>(M_PI) * 2) * .15f;
			position[1] += verticalMoveScale;
			bossChr.setPosition(position);
		}
		else if (bossTask->tick >= 32 && bossTask->tick < 44) {
			const float verticalMoveScale = sin((bossTask->tick - 32) / 24.0f * static_cast<float>(M_PI) * 2) * .04f;
			position[1] += verticalMoveScale;
			bossChr.setPosition(position);
		}
		else if (bossTask->tick >= 44 && bossTask->tick < 56) {
			const float verticalMoveScale = sin((bossTask->tick - 32) / 24.0f * static_cast<float>(M_PI) * 2) * .4f;
			position[1] += verticalMoveScale;
			bossChr.setPosition(position);
		}

		bossTask->tick++;
		return {};
		}, {}));

	getComboSystem()->registerBossTask(stdcombo::BossTask("Phase1_DSSS_2h_r1_1", -1, [this](stdcombo::BossTask* bossTask) -> std::optional<std::string> {
		ChrIns bossChr(getChrAddress().value());

		if ((!bossTask->comboSource.has_value() || !isAnimationPresent(bossTask->comboSource.value().animationId)) 
				&& (bossTask->tick <= 2 && bossChr.getAnimationString().find(L"Damage") != std::wstring::npos)
				|| (bossTask->tick > 2 && !isAnimationPresent(176034000))) {
			giveItemAndSwap(InventorySlot::PrimaryRightWep,
				ItemParamIdPrefix::Weapon,
				9220005, //Dragonslayer Spear
				-1);
			setSheathState(3);
			return "";
		}

		if (bossTask->tick == 0) {
			updateChainComboMove(bossTask->taskId);
			giveItemAndSwap(InventorySlot::PrimaryRightWep,
				ItemParamIdPrefix::Weapon,
				9160000, //Dragonslayer Swordspear
				-1);
			setSheathState(3);
		}
		else if (bossTask->tick == 2) {
			ChrIns bossChr(getChrAddress().value());
			bossChr.playAnimation(L"W_AttackBothLight1");
		}
		else if (bossTask->tick == 3) {
			giveItemAndSwap(InventorySlot::PrimaryRightWep,
				ItemParamIdPrefix::Weapon,
				9220005, //Dragonslayer Spear
				-1);
			setSheathState(3);
		}

		bossTask->tick++;
		return {};
		}, {
			{ L"W_AttackBothHeavy1SubStart", "Phase1_RKS_2h_r2_1_start" },
			{ L"W_AttackBothLeft2", "Phase1_DSSS_2h_r1_2" },
		}));
	
	getComboSystem()->registerBossTask(stdcombo::BossTask("Phase1_GUGS_2h_r1_3", -1, [this](stdcombo::BossTask* bossTask) -> std::optional<std::string> {
		ChrIns bossChr(getChrAddress().value());

		if ((!bossTask->comboSource.has_value() 
				|| !isAnimationPresent(bossTask->comboSource.value().animationId))
				&& !isAnimationPresent(193034020)) {
			giveItemAndSwap(InventorySlot::PrimaryRightWep,
				ItemParamIdPrefix::Weapon,
				9220005, //Dragonslayer Spear
				-1);
			setSheathState(3);
			return "";
		}

		if (bossTask->tick == 0) {
			updateChainComboMove(bossTask->taskId);
			giveItemAndSwap(InventorySlot::PrimaryRightWep,
				ItemParamIdPrefix::Weapon,
				6050010, //Greatsword
				-1);
			setSheathState(3);
		}
		else if (bossTask->tick == 3) {
			ChrIns bossChr(getChrAddress().value());
			bossChr.playAnimation(L"W_AttackBothLight3");
		}
		else if (bossTask->tick == 6) {
			giveItemAndSwap(InventorySlot::PrimaryRightWep,
				ItemParamIdPrefix::Weapon,
				9220005, //Dragonslayer Spear
				-1);
			setSheathState(3);
		}

		bossTask->tick++;
		return {};
		}, {
			{ L"W_AttackBothLeft2", "Phase1_DSSS_2h_r1_2" }
		}));
	
	getComboSystem()->registerBossTask(stdcombo::BossTask("Phase1_DSSS_2h_r1_2", -1, [this](stdcombo::BossTask* bossTask) -> std::optional<std::string> {
		if ((!bossTask->comboSource.has_value() || !isAnimationPresent(bossTask->comboSource.value().animationId)) 
				&& !isAnimationPresent(176034010)) {
			return "";
		}

		if (bossTask->tick == 0) {
			updateChainComboMove(bossTask->taskId);
			giveItemAndSwap(InventorySlot::PrimaryRightWep,
				ItemParamIdPrefix::Weapon,
				9160000, //Dragonslayer Swordspear
				-1);
			setSheathState(3);
		}
		else if (bossTask->tick == 3) {
			ChrIns bossChr(getChrAddress().value());
			bossChr.playAnimation(L"W_AttackBothLight2");
		}
		else if (bossTask->tick == 6) {
			giveItemAndSwap(InventorySlot::PrimaryRightWep,
				ItemParamIdPrefix::Weapon,
				9220005, //Dragonslayer Spear
				-1);
			setSheathState(3);
		}

		bossTask->tick++;
		return {};
		}, {
			{ L"W_AttackBothHeavy1Start", "Phase1_RKS_2h_r2_1_start" },
			{ L"W_AttackBothLeft1", "Phase1_DSSS_2h_r1_1" },
			{ L"W_AttackBothLeft3", "Phase1_GUGS_2h_r1_3" },
		}));

	getComboSystem()->registerBossTask(stdcombo::BossTask("Phase1_RKS_l2", 36036240, [this](stdcombo::BossTask* bossTask) -> std::optional<std::string> {
		if (!isAnimationPresent(bossTask->baseAnimationId) && !isAnimationPresent(252036701)) {
			return "";
		}

		if (bossTask->tick == 0) {
			updateChainComboMove(bossTask->taskId);
			giveItemAndSwap(InventorySlot::PrimaryRightWep,
				ItemParamIdPrefix::Weapon,
				9240000, //Ringed Knight Spear
				-1);
			setSheathState(3);
		}
		else if (bossTask->tick == 3) {
			ChrIns bossChr(getChrAddress().value());
			bossChr.playAnimation(3336);
			bossChr.playAnimation(3447);
		}
		else if (bossTask->tick == 6) {
			giveItemAndSwap(InventorySlot::PrimaryRightWep,
				ItemParamIdPrefix::Weapon,
				9220005, //Dragonslayer Spear
				-1);
			setSheathState(3);
		}
		else if (bossTask->tick == 9) {
			ChrIns bossChr(getChrAddress().value());
			BulletSpawn::launchBullet((int32_t)bossChr.getHandle(),
				13490000,
				-1,
				bossChr.getPosition(),
				std::vector<float>(3));
		}
		else if (bossTask->tick == 51) {
			ChrIns bossChr(getChrAddress().value());
			const float angle = bossChr.getAngle();
			std::vector<float> spawnEffectPosition = bossChr.getPosition();
			spawnEffectPosition[1] += .5;
			BulletSpawn::launchBullet((int32_t)bossChr.getHandle(),
				13550000,
				-1,
				spawnEffectPosition,
				{ -sin(angle), 0, -cos(angle) });

			for (int i = 0; i < 8; i++) {
				const float displaceScale = (i > 3 ? i - 4 : i) / 4.0f;
				const float angle = bossChr.getAngle();
				std::vector<float> itrDirection = { -sin(angle), 0, -cos(angle) };
				auto position = bossChr.getPosition();
				position[0] += -sin(angle - static_cast<float>(M_PI) / (i > 3 ? 2.0f : -2.0f)) * displaceScale;
				position[1] += .7f;
				position[2] += -cos(angle - static_cast<float>(M_PI) / (i > 3 ? 2.0f : -2.0f)) * displaceScale;
				itrDirection[0] += -sin(angle - static_cast<float>(M_PI) / (i > 3 ? 8.0f : -8.0f)) * displaceScale;
				itrDirection[2] += -cos(angle - static_cast<float>(M_PI) / (i > 3 ? 8.0f : -8.0f)) * displaceScale;
				BulletSpawn::launchBullet((int32_t)bossChr.getHandle(),
					13550100,
					-1,
					position,
					itrDirection);
			}
		}

		bossTask->tick++;
		return {};
		}, {
			{ L"W_AttackBothHeavy1Start", "Phase1_DSSS_2h_r2_1_start" },
			{ L"W_ChargeContinue", "Phase1_RKS_l2_r2_1" },
			{ L"W_AttackBothLeft1", "Phase1_RGCS_L2" },
		}));

	getComboSystem()->registerBossTask(stdcombo::BossTask("Phase1_RKS_l2_r2_1", -1, [this](stdcombo::BossTask* bossTask) -> std::optional<std::string> {
		if ((!bossTask->comboSource.has_value() || !isAnimationPresent(bossTask->comboSource.value().animationId)) 
				&& !isAnimationPresent(252036711)) {
			return "";
		}

		if (bossTask->tick == 0) {
			updateChainComboMove(bossTask->taskId);
			giveItemAndSwap(InventorySlot::PrimaryRightWep,
				ItemParamIdPrefix::Weapon,
				9240000, //Ringed Knight Spear
				-1);
			setSheathState(3);
		}
		else if (bossTask->tick == 3) {
			ChrIns bossChr(getChrAddress().value());
			bossChr.playAnimation(3544);
		}
		else if (bossTask->tick == 16) {
			giveItemAndSwap(InventorySlot::PrimaryRightWep,
				ItemParamIdPrefix::Weapon,
				9220005, //Dragonslayer Spear
				-1);
			setSheathState(3);
		}

		bossTask->tick++;
		return {};
		}, {
			{ L"W_ChargeContinue2", "Phase1_RKS_l2_r2_2" },
		}));

	getComboSystem()->registerBossTask(stdcombo::BossTask("Phase1_RKS_l2_r2_2", -1, [this](stdcombo::BossTask* bossTask) -> std::optional<std::string> {
		if ((!bossTask->comboSource.has_value() || !isAnimationPresent(bossTask->comboSource.value().animationId)) 
				&& !isAnimationPresent(252036721)) {
			return "";
		}

		if (bossTask->tick == 0) {
			updateChainComboMove(bossTask->taskId);
			giveItemAndSwap(InventorySlot::PrimaryRightWep,
				ItemParamIdPrefix::Weapon,
				9240000, //Ringed Knight Spear
				-1);
			setSheathState(3);
		}
		else if (bossTask->tick == 3) {
			ChrIns bossChr(getChrAddress().value());
			bossChr.playAnimation(4203);
		}
		else if (bossTask->tick == 16) {
			giveItemAndSwap(InventorySlot::PrimaryRightWep,
				ItemParamIdPrefix::Weapon,
				9220005, //Dragonslayer Spear
				-1);
			setSheathState(3);
		}

		bossTask->tick++;
		return {};
		}, {}));

	damageModuleHook->installFilter("ariandar_boss_global_replaces", [this](uintptr_t sprjChrDamageModule, uintptr_t attackerChr, char* attackDataBuffer) -> bool {
		if (getChrAddress().has_value() && getChrAddress().value() == attackerChr) {
			int32_t* attackIdPtr = (int32_t*)(attackDataBuffer + 0x44);
			spdlog::debug("Attack id: {}", *attackIdPtr);
			if (*attackIdPtr == 3600030) *attackIdPtr = 2613305; //dragon slayer spear 2h kick r2 replaced with lothric knight greatsword 2h r2 1 charged
			else if (*attackIdPtr == 3600530) *attackIdPtr = 3600200; //dragon slayer spear 2h running r1 replaced with dragon slayer spear 2h r1 1
			else if (*attackIdPtr == 3603315 || *attackIdPtr == 3603310) *attackIdPtr = 2600701; //dragon slayer spear 2h r2 2 charged & uncharged replaced with lothric knight greatsword l2 r2
			else if (*attackIdPtr == 3603300) *attackIdPtr = 2613300; //dragon slayer spear 2h r2 1 uncharged replaced with lothric knight greatsword 2h r2 1 uncharged
			else if (*attackIdPtr == 3603305) *attackIdPtr = 2613300; //dragon slayer spear 2h r2 1 charged replaced with lothric knight greatsword 2h r2 1 uncharged
			else if (*attackIdPtr == 3600105) *attackIdPtr = 2600010; //dragon slayer spear 1h r2 1 charged replaced with lothric knight greatsword 2h r1 2
																	  //2600010 lothric knight greatsword 2h r1 2
			spdlog::debug("Attack after id: {}", *attackIdPtr);
		}

		return true;
		});

	replacePlayerAnibndFile(std::filesystem::current_path().append("DS3RuntimeScripting\\mods\\boss_ariandar\\"));
	return true;
}

bool AriandarBoss::onDetach()
{
	if (!PlayerIns::isMainChrLoaded()) return false;
	PlayerIns mainChr(PlayerIns::getMainChrAddress());
	restoreVannilaPlayerAnibndFile();
	auto playAnimHook = (PlayAnimationHook*)ds3runtime_global->accessHook("play_anim_hook");
	auto damageModuleHook = (SprjChrDamageModuleHook*)ds3runtime_global->accessHook("sprj_chr_damage_module_hook");
	auto sessionSendHook = (SessionSendHook*)ds3runtime_global->accessHook("session_send_hook");
	playAnimHook->uninstallFilter("ariandar_boss_global_replaces");
	playAnimHook->uninstallFilter("ariandar_boss_phase1_combos");
	damageModuleHook->uninstallFilter("ariandar_boss_global_replaces");
	((ParamPatcher*)ds3runtime_global->accessScript("param_patcher"))->restore("ariandar_boss");
	auto chrAddress = getChrAddress();
	if (!chrAddress.has_value()) return false;
	PlayerIns chr(chrAddress.value());
	if (!chr.isValid()) return false;
	SprjChrDataModule chrData(chr.getSprjChrDataModule());
	if (chrData.isNoStaminaConsumption()) chrData.setNoStaminaConsumption(false);
	return StandardPlayerBoss::onDetach();
}

void AriandarBoss::logic()
{
	if (!getChrAddress().has_value()) return;
	PlayerIns chr(getChrAddress().value());
	if (!chr.isValid()) return;
	std::optional<int32_t> animationId = getAnimationId();
	
	if (!getComboSystem()->getCurrentMoveTask().has_value() && animationId.has_value()) {
		for (auto task : getComboSystem()->getBossTasks()) {
			if (animationId.value() != -1 && animationId.value() != task.baseAnimationId) continue;
			getComboSystem()->setCurrentMoveTask(task);
			break;
		}
	}

	if (getComboSystem()->getCurrentMoveTask().has_value()) {
		stdcombo::BossTask bossTask = getComboSystem()->getCurrentMoveTask().value();
		std::optional<std::string> taskTransfer = bossTask.task(&bossTask);
		getComboSystem()->setCurrentMoveTask(bossTask);
		
		if (taskTransfer.has_value() && taskTransfer.value() == "") {
			getComboSystem()->setCurrentMoveTask({});
		}
		else if (taskTransfer.has_value()) {
			for (auto task : getComboSystem()->getBossTasks()) {
				if (taskTransfer.value() != task.taskId) continue;
				getComboSystem()->setCurrentMoveTask(task);
				break;
			}
		}
	}
}

void AriandarBoss::checks()
{
	if (!getChrAddress().has_value()) return;
	PlayerIns chr(getChrAddress().value());
	if (!chr.isValid()) return;
	WorldChrMan worldChrMan(WorldChrMan::getInstance());
	uintptr_t paintedGuardian1Ptr = worldChrMan.getInsByHandle(10058001);
	uintptr_t paintedGuardian2Ptr = worldChrMan.getInsByHandle(10058002);

	if (paintedGuardian1Ptr) {
		ChrIns paintedGuardian(paintedGuardian1Ptr);
		paintedGuardian.setPosition(std::vector<float>({ 0, 0, 0 }));
		SprjChrDataModule(paintedGuardian.getSprjChrDataModule()).setHealth(0);
	}

	if (paintedGuardian2Ptr) {
		ChrIns paintedGuardian(paintedGuardian2Ptr);
		paintedGuardian.setPosition(std::vector<float>({ 0, 0, 0 }));
		SprjChrDataModule(paintedGuardian.getSprjChrDataModule()).setHealth(0);
	}

	SprjChrDataModule chrData(chr.getSprjChrDataModule());
	if (chr.getWeightIndex() != 1) chr.setWeightIndex(1);
	if (chrData.getFP() < chrData.getMaxFP()) chrData.setFP(static_cast<uint32_t>(fmin(static_cast<float>(chrData.getMaxFP()), chrData.getFP() + fmax(1.0f, chrData.getMaxFP() / 100.0f))));
	if (chrData.getBaseMaxHealth() != 8000) chrData.setBaseMaxHealth(8000);
	if (!chrData.isNoStaminaConsumption()) chrData.setNoStaminaConsumption(true);
}

void AriandarBoss::updateChainComboMove(const std::string& taskId) {
	this->chainComboLastMove = taskId;
}

}