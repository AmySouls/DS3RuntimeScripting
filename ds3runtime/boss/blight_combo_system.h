/*
	* DS3RuntimeScripting
	* Contributers: Amir
*/

#pragma once
#include "pch.h"
#include "ds3runtime/ds3runtime.h"
#include "boss.h"
#include "ds3runtime/equip_game_data.h"
#include "ds3runtime/player_game_data.h"
#include <ds3runtime/sprj_lua_event_condition_item.h>
#include "ds3runtime/sprj_gaitem_imp.h"

namespace ds3runtime::blight {

struct ComboSource
{
	ComboSource(const std::string& taskId)
	{
		this->taskId = taskId;
	}

	std::string taskId;
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

}