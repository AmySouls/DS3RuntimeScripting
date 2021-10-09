#pragma once
#include "pch.h"
#include "standard_combo_system.h"

namespace ds3runtime::stdcombo {

void ComboSystem::install()
{
	auto playAnimHook = (PlayAnimationHook*)ds3runtime_global->accessHook("play_anim_hook");

	playAnimHook->installFilter("boss_combo_moves_" + std::to_string(boss->getUniqueId()),
		[this](uintptr_t hkbCharacter, int32_t animationPlayId) -> int32_t {
			if (!boss->getChrAddress().has_value()) return animationPlayId;
			ChrIns bossChr(boss->getChrAddress().value());
			if (!bossChr.isValid() || !bossChr.hasHkbCharacter() || hkbCharacter != bossChr.getHkbCharacter()) return animationPlayId;

			if (currentMoveTask.has_value()) {
				for (auto entry : currentMoveTask->combos) {
					const int32_t comboAnimId = bossChr.getHkbIdFromString(entry.first);

					if (animationPlayId != comboAnimId) continue;
					if (currentMoveTask.has_value()
						&& currentMoveTask.value().taskId == entry.second) continue;
					std::optional<BossTask> matchingBossTask;

					for (BossTask bossTask : bossTasks) if (bossTask.taskId.compare(entry.second) == 0) {
						matchingBossTask = bossTask;
						break;
					}

					if (!matchingBossTask.has_value()) break;
					BossTask newBossTask = matchingBossTask.value(); 
					newBossTask.comboSource = ComboSource(animationPlayId, boss->getAnimationId().value());
					currentMoveTask.emplace(newBossTask);
					return 0;
				}

				if (currentMoveTask->comboSource.has_value() 
					&& animationPlayId == currentMoveTask->comboSource.value().animationPlayId
					&& boss->getAnimationId().value() == currentMoveTask->comboSource.value().animationId) {
					return 0;
				}
			}
			else {
				for (NeutralCombo combo : neutralCombos) {
					const int32_t comboAnimId = bossChr.getHkbIdFromString(combo.animationPlayString);
					if (animationPlayId != comboAnimId) continue;
					std::optional<BossTask> matchingBossTask;

					for (BossTask bossTask : bossTasks) if (bossTask.taskId.compare(combo.taskId) == 0) {
						matchingBossTask = bossTask;
						break;
					}

					if (!matchingBossTask.has_value()) break;
					BossTask newBossTask = matchingBossTask.value(); 
					newBossTask.comboSource = ComboSource(animationPlayId, boss->getAnimationId().value());
					currentMoveTask.emplace(newBossTask);
					return 0;
				}
			}

			return animationPlayId;
		});
}

void ComboSystem::uninstall()
{
	auto playAnimHook = (PlayAnimationHook*)ds3runtime_global->accessHook("play_anim_hook");
	playAnimHook->uninstallFilter("boss_combo_moves_" + std::to_string(boss->getUniqueId()));
}

void ComboSystem::registerBossTask(const BossTask& bossTask)
{
	bossTasks.push_back(bossTask);
}

void ComboSystem::registerNeutralCombo(const NeutralCombo& neutralCombo)
{
	neutralCombos.push_back(neutralCombo);
}

void ComboSystem::unregisterBossTask(const std::string& taskId)
{
	bossTasks.erase(std::remove_if(bossTasks.begin(), bossTasks.end(), [&](auto task) -> bool {
		return task.taskId == taskId;
		}));
}

void ComboSystem:: unregisterNeutralCombo(const std::string& comboId)
{
	neutralCombos.erase(std::remove_if(neutralCombos.begin(), neutralCombos.end(), [&](auto neutralCombo) -> bool {
		return neutralCombo.comboId == comboId;
		}));
}

std::vector<BossTask> ComboSystem::getBossTasks()
{
	return bossTasks;
}

std::optional<BossTask> ComboSystem::getBossTaskByTaskId(std::string taskId)
{
	std::optional<BossTask> bossTask = {};

	for (BossTask task : bossTasks) if (task.taskId == taskId) {
		bossTask = task;
		break;
	}

	return bossTask;
}

void ComboSystem::setCurrentMoveTask(std::optional<stdcombo::BossTask> bossTask)
{
	currentMoveTask = bossTask;
}

std::optional<stdcombo::BossTask> ComboSystem::getCurrentMoveTask()
{
	return currentMoveTask;
}

}