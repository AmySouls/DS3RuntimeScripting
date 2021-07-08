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

class StandardPlayerBoss : public PlayerBoss
{
public:
	StandardPlayerBoss(std::shared_ptr<PlayerIns> playerIns);

	bool onAttach();

	void onDetach();

	void giveItemAndSwap(InventorySlot inventorySlot,
		ItemParamIdPrefix paramIdPrefix,
		int32_t paramItemId,
		int32_t durability);

	void giveGoodsAndSwap(GoodsSlot inventorySlot,
		int32_t paramItemId);

	void replacePlayerAnibndFile(std::filesystem::path path);

	void restoreVannilaPlayerAnibndFile();

	void tryReloadPlayerChr();

	std::optional<int32_t> getAnimationId();

	bool isAnimationPresent(int32_t animationId);
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

	std::optional<int32_t> findInventoryIdByGiveId(int32_t giveId);

	void savePlayerData();

	void restorePlayerData();

	void saveEquipment();

	void unequipAllEquipment();

	void reequipSavedEquipment();

	void saveAndDiscardItems();

	void loadAndGiveSavedItems();

	ItemParamIdPrefix getItemParamIdPrefixFromGiveId(int32_t giveId);

	bool isHiddenItem(const uint32_t itemId);

	int32_t getItemMaxDurability(ItemParamIdPrefix paramIdPrefix, int32_t paramItemId);
};

struct BossTask
{
	BossTask(std::string taskId, int32_t baseAnimationId, std::function<std::optional<std::string>(StandardPlayerBoss*, BossTask* bossTask)> task)
	{
		this->taskId = taskId;
		this->baseAnimationId = baseAnimationId;
		this->task = task;
	}

	int32_t baseAnimationId;
	std::string taskId;
	int64_t tick;
	std::function<std::optional<std::string>(StandardPlayerBoss*, BossTask*)> task;
	std::unordered_map<std::string, float> floatVariables;
	std::unordered_map<std::string, int64_t> signedIntVariables;
	std::unordered_map<std::string, uint64_t> unsignedIntVariables;
	std::unordered_map<std::string, bool> boolVariables;
	std::unordered_map<std::string, std::string> stringVariables;
	std::unordered_map<std::string, std::wstring> wideStringVariables;
	std::unordered_map<std::string, std::vector<float>> floatVectorVariables;
	std::unordered_map<std::string, uintptr_t> addressVariables;
};

}