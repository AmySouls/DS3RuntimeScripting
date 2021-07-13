/*
	* DS3RuntimeScripting
	* Contributers: Amir
*/

#pragma once
#include "pch.h"
#include "ds3runtime/ds3runtime.h"
#include "player_boss.h"
#include "ds3runtime/equip_game_data.h"
#include "ds3runtime/player_game_data.h"
#include <ds3runtime/sprj_lua_event_condition_item.h>
#include "ds3runtime/sprj_gaitem_imp.h"

namespace ds3runtime {

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
	NeutralCombo(std::wstring animationPlayString, std::string taskId)
	{
		this->animationPlayString = animationPlayString;
		this->taskId = taskId;
	}

	std::wstring animationPlayString;
	std::string taskId;
};

class StandardPlayerBoss : public PlayerBoss
{
public:
	StandardPlayerBoss(uint16_t forwardId);

	bool onAttach();

	bool onDetach();

	void replacePlayerAnibndFile(std::filesystem::path path);

	void restoreVannilaPlayerAnibndFile();

	void giveItemAndSwap(InventorySlot inventorySlot,
		ItemParamIdPrefix paramIdPrefix,
		int32_t paramItemId,
		int32_t durability);

	void giveGoodsAndSwap(GoodsSlot inventorySlot,
		int32_t paramItemId);

	void tryReloadPlayerChr();

	std::optional<int32_t> getAnimationId();

	bool isAnimationPresent(int32_t animationId);

	std::optional<uintptr_t> getChrAddress();

	void setSheathState(int32_t slot);

	void registerNeutralCombo(NeutralCombo combo);

	void registerBossTask(BossTask bossTask);

	std::optional<BossTask> getBossTaskByTaskId(std::string taskId);

	void setCurrentMoveTask(std::optional<BossTask> bossTask);

	BossTask* getCurrentMoveTask();

	std::vector<BossTask> getBossTasks();
private:
	FMOD::System* fmodSystem = nullptr;
	std::vector<InventoryItem> savedItems;
	std::unordered_map<InventorySlot, std::optional<InventoryItem>> savedEquipment;
	std::unordered_map<GoodsSlot, std::optional<InventoryItem>> savedGoods;
	std::wstring savedName;
	Attributes savedAttributes;
	PlayerGameData::Gender savedGender;
	PlayerGameData::Class savedClass;
	PlayerGameData::Age savedAge;
	PlayerGameData::Voice savedVoice;
	std::vector<BossTask> bossTasks;
	std::optional<BossTask> currentMoveTask;
	std::vector<NeutralCombo> neutralCombos;

	std::optional<int32_t> findInventoryIdByGiveId(int32_t giveId);

	bool savePlayerData();

	bool restorePlayerData();

	bool saveEquipment();

	bool unequipAllEquipment();

	bool reequipSavedEquipment();

	bool saveAndDiscardItems();

	bool loadAndGiveSavedItems();

	ItemParamIdPrefix getItemParamIdPrefixFromGiveId(int32_t giveId);

	bool isHiddenItem(const uint32_t itemId);

	int32_t getItemMaxDurability(ItemParamIdPrefix paramIdPrefix, int32_t paramItemId);
};

}