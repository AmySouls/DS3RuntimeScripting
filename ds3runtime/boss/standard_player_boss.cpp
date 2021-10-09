#pragma once
#include "pch.h"
#include "standard_player_boss.h"
#include "ds3runtime/scripts/fmod_system_handler.h"
#include <ds3runtime/scripts/param_patcher.h>
#include "ds3runtime/game_data_man.h"
#include <ds3runtime/databaseaddress.h>
#include <ds3runtime/world_chr_man.h>
#include <ds3runtime/game_option_man.h>
#include "ds3runtime/scripts/animation_id_handler.h"
#include <ds3runtime/player_network_session.h>
#include <ds3runtime/hooks/play_animation_hook.h>

namespace ds3runtime {

bool StandardPlayerBoss::onAttach()
{
	if (!ds3runtime_global->accessScript("fmod_system_handler")
		|| !((FMODSystemHandler*)ds3runtime_global->accessScript("fmod_system_handler"))->getFMODSystem()) return false;
	if (!WorldChrMan::hasInstance()
		|| !getChrAddress().has_value() 
		|| !PlayerIns(getChrAddress().value()).isValid()) return false;
	if (!GameOptionMan::hasInstance()) return false;
	else if (!GameDataMan::hasInstance()) return false;
	GameDataMan gameDataMan(GameDataMan::getInstance());
	if (gameDataMan.getPlayerGameData() == 0) return false;
	PlayerGameData playerGameData(gameDataMan.getPlayerGameData());
	if (playerGameData.getEquipGameData() == 0) return false;
	EquipGameData equipGameData(playerGameData.getEquipGameData());
	if (equipGameData.getEquipInventoryData() == 0) return false;
	saveEquipment(); 
	unequipAllEquipment();
	saveAndDiscardItems();
	savePlayerData();
	GameOptionMan(GameOptionMan::getInstance()).setAutoSave(false);

	comboSystem.install();

	return true;
}

bool StandardPlayerBoss::onDetach()
{
	if (!GameOptionMan::hasInstance()) return false;
	else if (!GameDataMan::hasInstance()) return false;
	GameDataMan gameDataMan(GameDataMan::getInstance());
	if (gameDataMan.getPlayerGameData() == 0) return false;
	PlayerGameData playerGameData(gameDataMan.getPlayerGameData());
	if (playerGameData.getEquipGameData() == 0) return false;
	EquipGameData equipGameData(playerGameData.getEquipGameData());
	if (equipGameData.getEquipInventoryData() == 0) return false;
	restorePlayerData();
	unequipAllEquipment();
	loadAndGiveSavedItems();
	reequipSavedEquipment();
	comboSystem.uninstall();
	GameOptionMan(GameOptionMan::getInstance()).setAutoSave(true);
	return true;
}

std::optional<uintptr_t> StandardPlayerBoss::getChrAddress()
{
	if (PlayerIns::isMainChrLoaded() && PlayerIns(PlayerIns::getMainChrAddress()).hasHkbCharacter()) return PlayerIns::getMainChrAddress();
	return {};
}

void StandardPlayerBoss::savePlayerData()
{
	PlayerIns chr(getChrAddress().value());
	PlayerGameData playerGameData(chr.getPlayerGameData());
	savedBodyProportions = playerGameData.getBodyProportions();
	savedAttributes = playerGameData.getAttributes();
	savedGender = playerGameData.getGender();
	savedClass = playerGameData.getClass();
	savedAge = playerGameData.getAge();
	savedVoice = playerGameData.getVoice();
}

void StandardPlayerBoss::restorePlayerData()
{
	PlayerIns chr(getChrAddress().value());
	PlayerGameData playerGameData(chr.getPlayerGameData());
	playerGameData.setBodyPreportions(savedBodyProportions);
	playerGameData.setAttributes(savedAttributes);
	playerGameData.setGender(savedGender);
	playerGameData.setClass(savedClass);
	playerGameData.setAge(savedAge);
	playerGameData.setVoice(savedVoice);
}

void StandardPlayerBoss::giveGoodsAndSwap(GoodsSlot goodsSlot,
	int32_t paramItemId, int32_t quantity)
{
	EquipGameData equipGameData(PlayerGameData(GameDataMan(GameDataMan::getInstance()).getPlayerGameData()).getEquipGameData());
	EquipInventoryData equipInventoryData(equipGameData.getEquipInventoryData());
	std::optional<int32_t> indexOfItem = findInventoryIdByGiveId((uint32_t)ItemParamIdPrefix::Goods + paramItemId);
	
	if (!indexOfItem.has_value()) {
		equipInventoryData.addItem(ItemParamIdPrefix::Goods, paramItemId, quantity, 0);
		indexOfItem = findInventoryIdByGiveId((uint32_t)ItemParamIdPrefix::Goods + paramItemId);
	}

	if (indexOfItem.has_value()) {
		equipGameData.equipGoodsInventoryItem(goodsSlot, indexOfItem.value());
	}
	else {
		spdlog::error("StandardPlayerBoss::giveGoodsAndSwap: Could not give/swap item: goodsSlot = {} paramItemId = {} ",
			(uint32_t)goodsSlot,
			paramItemId);
		((FMODSystemHandler*)ds3runtime_global->accessScript("fmod_system_handler"))->tryErrorSound();
	}
}

void StandardPlayerBoss::giveItemAndSwap(InventorySlot inventorySlot,
	ItemParamIdPrefix paramIdPrefix,
	int32_t paramItemId,
	int32_t durability)
{
	EquipGameData equipGameData(PlayerGameData(GameDataMan(GameDataMan::getInstance()).getPlayerGameData()).getEquipGameData());
	EquipInventoryData equipInventoryData(equipGameData.getEquipInventoryData());
	std::optional<int32_t> indexOfItem = findInventoryIdByGiveId((uint32_t)paramIdPrefix + paramItemId);

	if (!indexOfItem.has_value()) {
		const int32_t durability = getItemMaxDurability(paramIdPrefix, paramItemId - paramItemId % 100);
		equipInventoryData.addItem(paramIdPrefix, paramItemId, 1, durability);
		indexOfItem = findInventoryIdByGiveId((uint32_t)paramIdPrefix + paramItemId);
	}

	if (indexOfItem.has_value()) {
		equipGameData.equipInventoryItem(inventorySlot, indexOfItem.value());
		PlayerNetworkSession::queueEquipmentPacket();
	}
	else {
		spdlog::error("StandardPlayerBoss::giveItemAndSwap: Could not give/swap item: inventorySlot = {} itemId = {} paramIdPrefix = {}  durability = {}",
			inventorySlot,
			paramItemId,
			(void*)paramIdPrefix,
			durability);
		((FMODSystemHandler*)ds3runtime_global->accessScript("fmod_system_handler"))->tryErrorSound();
	}
}

std::optional<int32_t> StandardPlayerBoss::findInventoryIdByGiveId(int32_t giveId)
{
	EquipGameData equipGameData(PlayerGameData(GameDataMan(GameDataMan::getInstance()).getPlayerGameData()).getEquipGameData());
	EquipInventoryData equipInventoryData(equipGameData.getEquipInventoryData());
	std::optional<int32_t> indexOfItem;

	for (int32_t i = 0; i < equipInventoryData.getInventoryItemCount(); i++) {
		auto* item = equipInventoryData.getInventoryItemById(i);

		if (item != nullptr && item->giveId == giveId) {
			indexOfItem = i;
			break;
		}
	}

	return indexOfItem;
}

void StandardPlayerBoss::saveEquipment()
{
	GameDataMan gameDataMan(GameDataMan::getInstance());
	PlayerGameData playerGameData(gameDataMan.getPlayerGameData());
	EquipGameData equipGameData(playerGameData.getEquipGameData());
	EquipInventoryData equipInventoryData(equipGameData.getEquipInventoryData());

	for (int32_t i = 0; i <= 21; i++) {
		if (i == 10 || i == 11 || i == 16) continue;
		const int32_t inventoryItemId = equipGameData.getInventoryItemIdBySlot((InventorySlot)i);
		std::optional<InventoryItem> item;

		if (inventoryItemId != -1) {
			auto* itemPtr = equipInventoryData.getInventoryItemById(inventoryItemId);;
			if (itemPtr != nullptr) item = *itemPtr;
		}

		savedEquipment[(InventorySlot)i] = item;
	}

	for (int32_t i = 0; i <= 0xE; ++i) {
		int32_t inventoryItemId = -1;
		if (i < 10) inventoryItemId = equipGameData.getInventoryItemIdByQuickSlot((GoodsSlot)i);
		else inventoryItemId = equipGameData.getInventoryItemIdByToolbeltSlot((GoodsSlot)i);

		std::optional<InventoryItem> item;

		if (inventoryItemId != -1) {
			auto* itemPtr = equipInventoryData.getInventoryItemById(inventoryItemId);;
			if (itemPtr != nullptr) item = *itemPtr;
		}

		savedGoods[(GoodsSlot)i] = item;
	}

	for (int32_t i = 1; i <= 14; ++i) {
		savedSpells[i] = playerGameData.getSpell(i);
	}
}

void StandardPlayerBoss::unequipAllEquipment()
{
	GameDataMan gameDataMan(GameDataMan::getInstance());
	PlayerGameData playerGameData(gameDataMan.getPlayerGameData());
	EquipGameData equipGameData(playerGameData.getEquipGameData());
	EquipInventoryData equipInventoryData(equipGameData.getEquipInventoryData());

	for (int32_t i = 0; i <= 21; ++i) {
		switch (i) {
		case 0:
		case 1:
		case 2:
		case 3:
		case 4:
		case 5:
			giveItemAndSwap((InventorySlot)i, ItemParamIdPrefix::Weapon, 110000, -1);
			break;
		case 12:
			giveItemAndSwap((InventorySlot)i, ItemParamIdPrefix::Protector, 900000, -1);
			break;
		case 13:
			giveItemAndSwap((InventorySlot)i, ItemParamIdPrefix::Protector, 901000, -1);
			break;
		case 14:
			giveItemAndSwap((InventorySlot)i, ItemParamIdPrefix::Protector, 902000, -1);
			break;
		case 15:
			giveItemAndSwap((InventorySlot)i, ItemParamIdPrefix::Protector, 903000, -1);
			break;
		case 6:
		case 7:
		case 8:
		case 9:
		case 17:
		case 18:
		case 19:
		case 20:
		case 21:
		{
			equipGameData.equipInventoryItem((InventorySlot)i, -1);
			break;
		}
		default:
			break;
		}
	}

	for (int32_t i = 0; i <= 0xE; ++i) {
		equipGameData.equipGoodsInventoryItem((GoodsSlot)i, -1);
	}

	for (int32_t i = 1; i <= 14; ++i) {
		playerGameData.setSpell(i, -1);
	}
}

void StandardPlayerBoss::reequipSavedEquipment()
{
	for (auto entry : savedEquipment) {
		if (!entry.second.has_value()) continue;
		const uint32_t prefix = (uint32_t)getItemParamIdPrefixFromGiveId(entry.second->giveId);
		uint32_t paramItemId = entry.second->giveId;
		if (prefix != 0) paramItemId = paramItemId % prefix;

		giveItemAndSwap(entry.first, 
			(ItemParamIdPrefix)prefix,
			paramItemId,
			0);
	}

	for (auto entry : savedGoods) {
		if (!entry.second.has_value()) continue;
		const uint32_t prefix = (uint32_t)getItemParamIdPrefixFromGiveId(entry.second->giveId);
		uint32_t paramItemId = entry.second->giveId;
		if (prefix != 0) paramItemId = paramItemId % prefix;

		giveGoodsAndSwap(entry.first,
			paramItemId, entry.second->quantity);
	}

	for (auto&& entry : savedSpells) {
		GameDataMan gameDataMan(GameDataMan::getInstance());
		PlayerGameData playerGameData(gameDataMan.getPlayerGameData());
		playerGameData.setSpell(entry.first, entry.second);
	}
}

void StandardPlayerBoss::saveAndDiscardItems()
{
	EquipGameData equipGameData(PlayerGameData(GameDataMan(GameDataMan::getInstance()).getPlayerGameData()).getEquipGameData());
	EquipInventoryData equipInventoryData(equipGameData.getEquipInventoryData());

	for (int32_t i = 0; i < equipInventoryData.getInventoryItemCount(); i++) {
		auto* item = equipInventoryData.getInventoryItemById(i);
		if (item == nullptr || isHiddenItem(item->giveId)) continue;
		savedItems.push_back(*item);
		if (getItemParamIdPrefixFromGiveId(item->giveId) != ItemParamIdPrefix::Goods) equipInventoryData.discardInventoryItems(i, item->quantity);
		else equipGameData.modifyInventoryItemQuantity(i, -(int32_t)item->quantity);
	}
}

void StandardPlayerBoss::loadAndGiveSavedItems()
{
	EquipGameData equipGameData(PlayerGameData(GameDataMan(GameDataMan::getInstance()).getPlayerGameData()).getEquipGameData());
	EquipInventoryData equipInventoryData(equipGameData.getEquipInventoryData());

	for (int32_t i = 0; i < equipInventoryData.getInventoryItemCount(); i++) {
		auto* item = equipInventoryData.getInventoryItemById(i);
		if (item == nullptr || isHiddenItem(item->giveId)) continue;
		if (getItemParamIdPrefixFromGiveId(item->giveId) != ItemParamIdPrefix::Goods) equipInventoryData.discardInventoryItems(i, item->quantity);
		else equipGameData.modifyInventoryItemQuantity(i, -(int32_t)item->quantity);
	}

	for (auto item : savedItems) {
		const uint32_t prefix = (uint32_t)getItemParamIdPrefixFromGiveId(item.giveId);
		uint32_t paramItemId = item.giveId;
		if (prefix != 0) paramItemId = paramItemId % prefix;
		const int32_t durability = getItemMaxDurability((ItemParamIdPrefix)prefix, paramItemId - paramItemId % 100);
		if ((ItemParamIdPrefix)prefix != ItemParamIdPrefix::Goods) equipInventoryData.addItem((ItemParamIdPrefix)prefix, paramItemId, item.quantity, durability);
		else {
			std::optional<int32_t> indexOfItem = findInventoryIdByGiveId((uint32_t)ItemParamIdPrefix::Goods + paramItemId);

			if (!indexOfItem.has_value()) {
				equipInventoryData.addItem(ItemParamIdPrefix::Goods, paramItemId, item.quantity, 0);
				indexOfItem = findInventoryIdByGiveId((uint32_t)ItemParamIdPrefix::Goods + paramItemId);
			}
			else {
				equipGameData.modifyInventoryItemQuantity(indexOfItem.value(), (int32_t)item.quantity);
			}
		}
	}
}

ItemParamIdPrefix StandardPlayerBoss::getItemParamIdPrefixFromGiveId(int32_t giveId)
{
	if ((giveId & (uint32_t)ItemParamIdPrefix::Protector) == (uint32_t)ItemParamIdPrefix::Protector) {
		return ItemParamIdPrefix::Protector;
	}
	else if ((giveId & (uint32_t)ItemParamIdPrefix::Accessory) == (uint32_t)ItemParamIdPrefix::Accessory) {
		return ItemParamIdPrefix::Accessory;
	}
	else if ((giveId & (uint32_t)ItemParamIdPrefix::Goods) == (uint32_t)ItemParamIdPrefix::Goods) {
		return ItemParamIdPrefix::Goods;
	}
	else {
		return ItemParamIdPrefix::Weapon;
	}
}


bool StandardPlayerBoss::isHiddenItem(const uint32_t itemId)
{
	static std::vector<uint32_t> hiddenItems = {
		0xFFFFFF,
		(uint32_t)ItemParamIdPrefix::Weapon + 110000u,
		(uint32_t)ItemParamIdPrefix::Protector + 900000u,
		(uint32_t)ItemParamIdPrefix::Protector + 901000u,
		(uint32_t)ItemParamIdPrefix::Protector + 902000u,
		(uint32_t)ItemParamIdPrefix::Protector + 903000u,
	};

	for (uint32_t hiddenItemId : hiddenItems) {
		if (hiddenItemId == itemId) return true;
	}

	return false;
}

int32_t StandardPlayerBoss::getItemMaxDurability(ItemParamIdPrefix paramIdPrefix, int32_t paramItemId)
{
	auto* paramPatcher = (ParamPatcher*)ds3runtime_global->accessScript("param_patcher");

	if (paramIdPrefix == ItemParamIdPrefix::Weapon 
		&& paramPatcher->doesIdExistInParam(L"EquipParamWeapon", paramItemId)) {
		ParamHandler weapon("", L"EquipParamWeapon", paramItemId);
		return weapon.read<uint16_t>(0xBE);
	}
	else if (paramIdPrefix == ItemParamIdPrefix::Protector
		&& paramPatcher->doesIdExistInParam(L"EquipParamProtector", paramItemId)) {
		ParamHandler protector("", L"EquipParamProtector", paramItemId);
		return protector.read<uint16_t>(0xAC);
	}
	else {
		return 0;
	}
}

void StandardPlayerBoss::replacePlayerAnibndFile(std::filesystem::path path)
{
	try {
		std::filesystem::copy(path, std::filesystem::current_path(), std::filesystem::copy_options::overwrite_existing | std::filesystem::copy_options::recursive);
	}
	catch (const std::filesystem::filesystem_error& error) {
		spdlog::error("Failed to replace player chr anibnd file {}", std::string(error.what()));
	}

	tryReloadPlayerChr();
}

void StandardPlayerBoss::restoreVannilaPlayerAnibndFile()
{
	try {
		std::filesystem::path vanillaPath = std::filesystem::current_path().append("DS3RuntimeScripting\\mods\\player_chr_vanilla\\");
		std::filesystem::copy(vanillaPath, std::filesystem::current_path(), std::filesystem::copy_options::overwrite_existing | std::filesystem::copy_options::recursive);
	}
	catch (const std::filesystem::filesystem_error& error) {
		spdlog::error("Failed to restore vannila player chr anibnd file {}", std::string(error.what()));
	}

	tryReloadPlayerChr();
}

void StandardPlayerBoss::tryReloadPlayerChr()
{
	std::optional<WorldChrMan> worldChrMan;
	if (WorldChrMan::hasInstance()) worldChrMan = WorldChrMan::getInstance();
	if (!worldChrMan.has_value() || !PlayerIns::isMainChrLoaded() || !accessMultilevelPointer<uintptr_t>(PlayerIns::getMainChrAddress() + 0x1F90, 0x58, 0x8, 0x1F90, 0x28, 0x10, 0x28, 0xB8)) return;
	worldChrMan->reloadCharacterFiles(L"c0000");
}

std::optional<int32_t> StandardPlayerBoss::getAnimationId()
{
	auto* handler = (AnimationIdHandler*)ds3runtime_global->accessScript("animation_id_handler");
	return handler->getAnimationId(getChrAddress().value());
}

bool StandardPlayerBoss::isAnimationPresent(int32_t animationId)
{
	auto* handler = (AnimationIdHandler*)ds3runtime_global->accessScript("animation_id_handler");
	std::optional<std::unordered_map<int32_t, int32_t>> animIdBuffer = handler->getAnimationIdBuffer(getChrAddress().value());
	
	if (animIdBuffer.has_value()) {
		for (int i = 0; i < 10; i++) {
			if (animIdBuffer.value()[i] == animationId) return true;
		}
	}
	
	return false;
}

void StandardPlayerBoss::setSheathState(int32_t slot)
{
	if (!GameDataMan::hasInstance()
		|| GameDataMan(GameDataMan::getInstance()).getPlayerGameData() == 0) return;
	PlayerGameData playerGameData = GameDataMan(GameDataMan::getInstance()).getPlayerGameData();
	playerGameData.setWeaponSheathState(slot);
	uint16_t sheathData[2] = {};
	sheathData[0] = playerGameData.getWeaponSheathData();
	sheathData[1] = PlayerIns(getChrAddress().value()).getForwardId();
	PlayerNetworkSession session(PlayerNetworkSession::getInstance());
	session.sessionPacketSend(13, (char*)sheathData, 4);
}

void StandardPlayerBoss::clearInventory() {
	EquipGameData equipGameData(PlayerGameData(GameDataMan(GameDataMan::getInstance()).getPlayerGameData()).getEquipGameData());
	EquipInventoryData equipInventoryData(equipGameData.getEquipInventoryData());

	for (int32_t i = 0; i < equipInventoryData.getInventoryItemCount(); i++) {
		auto* item = equipInventoryData.getInventoryItemById(i);
		if (item == nullptr || isHiddenItem(item->giveId)) continue;
		if (getItemParamIdPrefixFromGiveId(item->giveId) != ItemParamIdPrefix::Goods) equipInventoryData.discardInventoryItems(i, item->quantity);
		else equipGameData.modifyInventoryItemQuantity(i, -(int32_t)item->quantity);
	}
}

stdcombo::ComboSystem* StandardPlayerBoss::getComboSystem()
{
	return &comboSystem;
}



}