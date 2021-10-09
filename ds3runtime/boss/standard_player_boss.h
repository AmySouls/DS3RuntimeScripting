/*
	* DS3RuntimeScripting
	* Contributers: Amir
*/

#pragma once
#include "pch.h"
#include "ds3runtime/ds3runtime.h"
#include "ds3runtime/equip_game_data.h"
#include "ds3runtime/player_game_data.h"
#include <ds3runtime/sprj_lua_event_condition_item.h>
#include "ds3runtime/sprj_gaitem_imp.h"
#include "standard_combo_system.h"

namespace ds3runtime {

class StandardPlayerBoss : public Boss
{
public:
	StandardPlayerBoss() : comboSystem(this)
	{
	}

	bool onAttach();

	bool onDetach();
	
	virtual std::optional<uintptr_t> getChrAddress();

	virtual std::optional<int32_t> getAnimationId();

	void replacePlayerAnibndFile(std::filesystem::path path);

	void restoreVannilaPlayerAnibndFile();

	void giveItemAndSwap(InventorySlot inventorySlot,
		ItemParamIdPrefix paramIdPrefix,
		int32_t paramItemId,
		int32_t durability);

	void giveGoodsAndSwap(GoodsSlot inventorySlot,
		int32_t paramItemId, int32_t quantity);

	void tryReloadPlayerChr();

	bool isAnimationPresent(int32_t animationId);

	void setSheathState(int32_t slot);
private:
	FMOD::System* fmodSystem = nullptr;
	std::vector<InventoryItem> savedItems;
	std::unordered_map<InventorySlot, std::optional<InventoryItem>> savedEquipment;
	std::unordered_map<GoodsSlot, std::optional<InventoryItem>> savedGoods;
	std::unordered_map<int32_t, int32_t> savedSpells;
	std::wstring savedName;
	Attributes savedAttributes;
	PlayerGameData::Gender savedGender;
	PlayerGameData::Class savedClass;
	PlayerGameData::Age savedAge;
	PlayerGameData::Voice savedVoice;
	BodyProportions savedBodyProportions;
	stdcombo::ComboSystem comboSystem;

	std::optional<int32_t> findInventoryIdByGiveId(int32_t giveId);

	void savePlayerData();

	void saveEquipment();

	void saveAndDiscardItems();

	ItemParamIdPrefix getItemParamIdPrefixFromGiveId(int32_t giveId);

	bool isHiddenItem(const uint32_t itemId);

	int32_t getItemMaxDurability(ItemParamIdPrefix paramIdPrefix, int32_t paramItemId);
protected:
	void restorePlayerData();

	void unequipAllEquipment();

	void reequipSavedEquipment();

	void loadAndGiveSavedItems();

	void clearInventory();

	stdcombo::ComboSystem* getComboSystem();
};

}