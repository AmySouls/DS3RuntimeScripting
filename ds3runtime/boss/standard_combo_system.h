/*
* DS3RuntimeScripting
* Contributers: Amir
*/

#pragma once
#include "pch.h"
#include "ds3runtime/ds3runtime.h"
#include <ds3runtime/hooks/play_animation_hook.h>
#include <ds3runtime/boss/boss.h>

namespace ds3runtime::stdcombo {

struct ComboSource
{
	ComboSource(int32_t animationPlayId, int32_t animationId)
	{
		this->animationPlayId = animationPlayId;
		this->animationId = animationId;
	}

	int32_t animationPlayId;
	int32_t animationId;
};

struct BossTask
{
	BossTask(
		std::string taskId,
		int32_t baseAnimationId,
		std::function<std::optional<std::string>(BossTask* bossTask)> task,
		std::unordered_map<std::wstring, std::string> combos)
	{
		this->taskId = taskId;
		this->baseAnimationId = baseAnimationId;
		this->task = task;
		this->tick = 0;
		this->combos = combos;
	}

	int32_t baseAnimationId;
	std::string taskId;
	int64_t tick;
	std::unordered_map<std::wstring, std::string> combos;
	std::function<std::optional<std::string>(BossTask*)> task;
	std::unordered_map<std::string, float> floatVariables;
	std::unordered_map<std::string, int64_t> signedIntVariables;
	std::unordered_map<std::string, uint64_t> unsignedIntVariables;
	std::unordered_map<std::string, bool> boolVariables;
	std::unordered_map<std::string, std::string> stringVariables;
	std::unordered_map<std::string, std::wstring> wideStringVariables;
	std::unordered_map<std::string, std::vector<float>> floatVectorVariables;
	std::unordered_map<std::string, uintptr_t> addressVariables;
	std::optional<ComboSource> comboSource;
};

struct NeutralCombo
{
	NeutralCombo(std::string comboId, std::wstring animationPlayString, std::string taskId)
	{
		this->comboId = comboId;
		this->animationPlayString = animationPlayString;
		this->taskId = taskId;
	}

	std::string comboId;
	std::wstring animationPlayString;
	std::string taskId;
};

class ComboSystem
{
public:
	ComboSystem(Boss* boss)
	{
		this->boss = boss;
	}

	void install();

	void uninstall();

	void registerBossTask(const BossTask& bossTask);

	void registerNeutralCombo(const NeutralCombo& neutralCombo);

	void unregisterBossTask(const std::string& taskId);

	void unregisterNeutralCombo(const std::string& comboId);

	std::vector<BossTask> getBossTasks();

	std::optional<BossTask> getBossTaskByTaskId(std::string taskId);

	void setCurrentMoveTask(std::optional<stdcombo::BossTask> bossTask);

	std::optional<stdcombo::BossTask> getCurrentMoveTask();
private:
	ds3runtime::Boss* boss;
	std::vector<BossTask> bossTasks;
	std::vector<NeutralCombo> neutralCombos;
	std::optional<stdcombo::BossTask> currentMoveTask;
};

}