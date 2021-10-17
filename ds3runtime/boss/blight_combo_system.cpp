#pragma once
#include "pch.h"
#include "blight_combo_system.h"
#include <ds3runtime/hooks/jmp_table_status.h>

namespace ds3runtime::blight {

BlightComboSystem::BlightComboSystem(Boss* boss, std::string recoveryTaskId, int maxComboCharges, float comboChargesRegenRate) 
	: stdcombo::ComboSystem(boss)
{
	this->recoveryTaskId = recoveryTaskId;
	this->maxComboCharges = maxComboCharges;
	spdlog::debug("Combo charge regen: {}", comboChargesRegenRate);
	this->comboChargeRegenRate = comboChargeRegenRate;
	this->comboCharges = this->maxComboCharges;
}

void BlightComboSystem::install()
{
	auto playAnimHook = (PlayAnimationHook*)ds3runtime_global->accessHook("play_anim_hook");

	playAnimHook->installFilter("boss_combo_moves_" + std::to_string(getBoss()->getUniqueId()),
		[this](uintptr_t hkbCharacter, int32_t animationPlayId) -> int32_t {
			if (!getBoss()->getChrAddress().has_value()) return animationPlayId;
			ChrIns bossChr(getBoss()->getChrAddress().value());
			if (!bossChr.isValid() || !bossChr.hasHkbCharacter() || hkbCharacter != bossChr.getHkbCharacter()) return animationPlayId;

			JmpTableStatus* jmpTableStatus = reinterpret_cast<JmpTableStatus*>(ds3runtime_global->accessHook("jmp_table_status"));
			std::wstring animationPlayIdString = bossChr.getHkbStringFromId(animationPlayId);

			if (isInComboChain && (animationPlayIdString.find(L"Damage") != std::wstring::npos
					|| animationPlayIdString.find(L"Throw") != std::wstring::npos
					|| animationPlayIdString.find(L"Ladder") != std::wstring::npos
					|| animationPlayIdString.find(L"Death") != std::wstring::npos)) {
				isInComboChain = false;
			}
			
			if (getCurrentMoveTask().has_value()) {
				if (isInComboChain && comboCharges < 1 && jmpTableStatus->getJmpTableStatus(11)) {
					for (stdcombo::BossTask bossTask : getBossTasks()) if (bossTask.taskId.compare(recoveryTaskId) == 0) {
						isInComboChain = false;
						setCurrentMoveTask(bossTask);
						break;
					}

					return 0;
				}

				for (auto entry : getCurrentMoveTask()->combos) {
					const int32_t comboAnimId = bossChr.getHkbIdFromString(entry.first);

					if (animationPlayId != comboAnimId) continue;
					if (getCurrentMoveTask().value().taskId == entry.second) continue;
					std::optional<stdcombo::BossTask> matchingBossTask;

					for (stdcombo::BossTask bossTask : getBossTasks()) if (bossTask.taskId.compare(entry.second) == 0) {
						matchingBossTask = bossTask;
						break;
					}

					if (!matchingBossTask.has_value()) break;
					stdcombo::BossTask newBossTask = matchingBossTask.value(); 
					newBossTask.comboSource = stdcombo::ComboSource(animationPlayId, getBoss()->getAnimationId().value());
					lastTaskIdInChain = getCurrentMoveTask().value().taskId;

					if (isInComboChain) {
						comboCharges = maxComboCharges;
					}
					else {
						comboCharges -= 1;
					}
					
					setCurrentMoveTask(newBossTask);
					return 0;
				}

				if (getCurrentMoveTask()->comboSource.has_value() 
					&& animationPlayId == getCurrentMoveTask()->comboSource.value().animationPlayId
					&& getBoss()->getAnimationId().value() == getCurrentMoveTask()->comboSource.value().animationId) {
					return 0;
				}
			} else {
				if (isInComboChain && comboCharges < 1) {
					for (stdcombo::BossTask bossTask : getBossTasks()) if (bossTask.taskId.compare(recoveryTaskId) == 0) {
						isInComboChain = false;
						setCurrentMoveTask(bossTask);
						break;
					}

					return 0;
				}
				else if (isInComboChain) {
					std::optional<stdcombo::BossTask> lastChainBossTask;

					for (stdcombo::BossTask bossTask : getBossTasks()) if (bossTask.taskId.compare(lastTaskIdInChain) == 0) {
						lastChainBossTask = bossTask;
						break;
					}

					if (lastChainBossTask.has_value()) {
						for (auto entry : lastChainBossTask->combos) {
							const int32_t comboAnimId = bossChr.getHkbIdFromString(entry.first);

							if (animationPlayId != comboAnimId) continue;
							if (getCurrentMoveTask().value().taskId == entry.second) continue;
							std::optional<stdcombo::BossTask> matchingBossTask;

							for (stdcombo::BossTask bossTask : getBossTasks()) if (bossTask.taskId.compare(entry.second) == 0) {
								matchingBossTask = bossTask;
								break;
							}

							if (!matchingBossTask.has_value()) break;
							stdcombo::BossTask newBossTask = matchingBossTask.value(); 
							newBossTask.comboSource = stdcombo::ComboSource(animationPlayId, getBoss()->getAnimationId().value());
							lastTaskIdInChain = getCurrentMoveTask().value().taskId;
							comboCharges -= 1;
							setCurrentMoveTask(newBossTask);
							return 0;
						}
					}
				}

				for (stdcombo::NeutralCombo combo : getNeutralCombos()) {
					const int32_t comboAnimId = bossChr.getHkbIdFromString(combo.animationPlayString);
					if (animationPlayId != comboAnimId) continue;
					std::optional<stdcombo::BossTask> matchingBossTask;

					for (stdcombo::BossTask bossTask : getBossTasks()) if (bossTask.taskId.compare(combo.taskId) == 0) {
						matchingBossTask = bossTask;
						break;
					}

					if (!matchingBossTask.has_value()) break;
					stdcombo::BossTask newBossTask = matchingBossTask.value(); 
					newBossTask.comboSource = stdcombo::ComboSource(animationPlayId, getBoss()->getAnimationId().value());
					setCurrentMoveTask(newBossTask);
					spdlog::debug("Set to max from neutral");
					comboCharges = maxComboCharges;
					isInComboChain = true;
					return 0;
				}
			}

			return animationPlayId;
		});
}

bool BlightComboSystem::isExecutingComboChain()
{
	return isInComboChain;
}

float BlightComboSystem::getComboCharges()
{
	return comboCharges;
}

void BlightComboSystem::setComboCharges(const float& comboCharges)
{
	this->comboCharges = comboCharges;
}

int BlightComboSystem::getMaxComboCharges()
{
	return maxComboCharges;
}

void BlightComboSystem::setMaxComboCharges(const int& maxComboCharges)
{
	this->maxComboCharges = maxComboCharges;
}

float BlightComboSystem::getComboChargeRegenRate()
{
	return comboChargeRegenRate;
}

void BlightComboSystem::setComboChargeRegenRate(const float& comboChargeRegenRate)
{
	this->comboChargeRegenRate = comboChargeRegenRate;
}

}