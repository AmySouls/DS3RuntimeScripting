#pragma once
#include "pch.h"
#include "build_copy.h"
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
#include <ds3runtime/hooks/session_receive_hook.h>
#include <ds3runtime/sprj_session_manager.h>
#include "ds3runtime/game_option_man.h"

namespace ds3runtime {

bool BuildCopy::onAttach() {

	if (!getChrAddress().has_value() || !PlayerIns(getChrAddress().value()).hasHkbCharacter()) return false;
	PlayerIns chr(getChrAddress().value());
	if (!StandardPlayerBoss::onAttach()) return false;
	GameOptionMan(GameOptionMan::getInstance()).setAutoSave(false);

	((SessionReceiveHook*)ds3runtime_global
		->accessHook("session_receive_hook"))
		->installPacketFilter("build_copy", [&](uintptr_t networkSession, uintptr_t* networkHandle, int id, char* buffer, uint32_t maxLength, uint32_t receiveLength) -> uint32_t {
		auto session = PlayerNetworkSession(networkSession);
		auto sessionManager = SprjSessionManager(SprjSessionManager::getInstance());

		if (id == 8) {
			auto packet = packet::PlayerStruct(buffer, receiveLength);
			spdlog::debug("New player: {}", ds3runtime_global->utf8_encode(packet.getWideStringField("name").c_str()));
			auto invadeType = *reinterpret_cast<PlayerGameData::InvadeType*>(packet.getData() + 0x58);
			auto targetInvadeType = PlayerGameData::InvadeType::Host;
			bool isMatchingTarget = false;

			switch (targetType) {
			case TargetType::Host:
				isMatchingTarget = invadeType == PlayerGameData::InvadeType::InvadeRed
					|| invadeType == PlayerGameData::InvadeType::InvadePurpleDark
					|| invadeType == PlayerGameData::InvadeType::InvadeSunlightDark;
				break;
			case TargetType::Invader:
				isMatchingTarget = invadeType == PlayerGameData::InvadeType::InvadeRed
					|| invadeType == PlayerGameData::InvadeType::InvadePurpleDark
					|| invadeType == PlayerGameData::InvadeType::InvadeSunlightDark;
				break;
			case TargetType::Arena:
				isMatchingTarget = invadeType == PlayerGameData::InvadeType::ArenaBrawl;
				break;
			case TargetType::SteamId:
				isMatchingTarget = targetSteamId.has_value()
					&& packet.getWideStringField("steam_id").compare(targetSteamId.value()) != std::wstring::npos;
				break;
			default:
				break;
			}

			if (isMatchingTarget) {
				targetPlayerStruct = packet;
			}
		}

		//Log target player spells & goods
		if (id == 19 && getChrAddress().has_value() && target.has_value() && *networkHandle == *target->getNetworkHandle()) {
			auto packet = packet::Packet(id, buffer, receiveLength);
			const uint16_t networkId = *reinterpret_cast<int16_t*>(packet.getData());
			if (target->getForwardId() != networkId) return receiveLength;
			const int32_t currentSpell = *reinterpret_cast<int32_t*>(packet.getData() + 4);
			const int32_t currentGood = *reinterpret_cast<int32_t*>(packet.getData() + 8);
			bool hasSpell = false;
			bool hasGood = false;

			for (const auto& [key, value] : targetSpells) if (currentSpell == value) {
				hasSpell = true;
				break;
			}

			for (const auto& [key, value] : targetGoods) if (currentGood == value) {
				hasGood = true;
				break;
			}

			//Add spell if copy cat doesn't already have it
			if (!hasSpell && currentSpell != -1) {
				tryAddSpell(currentSpell);
			}

			//Add good if copy cat doesn't already have it
			if (!hasGood && currentGood != -1) {
				tryAddGood(currentGood);
			}
		}

		return receiveLength;
		});

	((SessionSendHook*)ds3runtime_global->accessHook("session_send_hook"))->installPacketFilter("build_copy", [&](uintptr_t playerNetworkSession, uintptr_t* networkHandle, int id, char* buffer, uint32_t maxLength) -> uint32_t {
		auto session = PlayerNetworkSession(playerNetworkSession);
		auto sessionManager = SprjSessionManager(SprjSessionManager::getInstance());
		
		if (id == 8) {
			auto packet = packet::PlayerStruct(buffer, maxLength);

			if (!localPlayerStruct.has_value()) {
				localPlayerStruct = packet;
				return 0;
			}
		}

		return maxLength;
		});

	return true;
}

bool BuildCopy::onDetach()
{

	if (!getChrAddress().has_value() || !PlayerIns(getChrAddress().value()).hasHkbCharacter()) return false;
	auto chrAddress = getChrAddress();
	const bool success = StandardPlayerBoss::onDetach();
	if (!success) return false;

	((SessionReceiveHook*)ds3runtime_global
		->accessHook("session_receive_hook"))
		->uninstallPacketFilter("build_copy");

	GameOptionMan(GameOptionMan::getInstance()).setAutoSave(true);
	return true;
}

void BuildCopy::logic()
{
	if (!getChrAddress().has_value()) return;
	PlayerIns chr(getChrAddress().value());
	
	if (!target.has_value()) {
		target = findNewTarget();
		if (!target.has_value()) return;

		if (targetPlayerStruct.has_value() && localPlayerStruct.has_value()) {
			packet::PlayerStruct playerData = localPlayerStruct.value();
			packet::PlayerStruct targetPlayerData = targetPlayerStruct.value();
			memcpy(reinterpret_cast<uint8_t*>(playerData.getData() + 0x2C),
				reinterpret_cast<uint8_t*>(targetPlayerData.getData() + 0x2C),
				sizeof(Attributes));
			memcpy(reinterpret_cast<uint8_t*>(playerData.getData() + 0xA4),
				reinterpret_cast<uint8_t*>(targetPlayerData.getData() + 0xA4),
				256);
			playerData.setWideStringField("name", targetPlayerData.getWideStringField("name"));
			auto session = PlayerNetworkSession(PlayerNetworkSession::getInstance());
			session.sessionPacketSend(&playerData);
		}

		spdlog::info("New target!: {}", ds3runtime_global->utf8_encode(PlayerGameData(target->getPlayerGameData()).getName()));
	}

	if (!target->isValid()) {
		target.reset();
		targetGoods.clear();
		targetSpells.clear();
		unequipAllEquipment();
		loadAndGiveSavedItems();
		restorePlayerData();
		clearInventory();
		return;
	}

	updateAndCopyPlayerData();

	if (chr.hasHkbCharacter()) {
		updateAndCopyEquipment();
	}
}

void BuildCopy::checks()
{
	if (localPlayerStruct.has_value() && isPlayersLoaded()) {
		spdlog::debug("players are loaded, sending normal.");
		auto session = PlayerNetworkSession(PlayerNetworkSession::getInstance());
		session.sessionPacketSend(&localPlayerStruct.value());
		localPlayerStruct.reset();
	}
}

inline std::optional<PlayerIns> BuildCopy::findNewTarget()
{
	for (int32_t i = 1; i <= 5; ++i) {
		const uintptr_t playerAddress = 
			PlayerIns::getAddressByOffsetNumber(static_cast<PlayerIns::OffsetNumber>(i));

		if (!PlayerIns::isPlayer(playerAddress)) continue;
		PlayerIns player(playerAddress);

		if (player.getChrType() == PlayerIns::ChrType::HostOfEmbers
					||player.getChrType() == PlayerIns::ChrType::Arena) {
			return playerAddress;
		}
	}

	return {};
}

void BuildCopy::updateAndCopyPlayerData()
{
	PlayerIns chr(getChrAddress().value());
	PlayerGameData gameData(chr.getPlayerGameData());
	PlayerGameData targetData(target->getPlayerGameData());
	
	if (gameData.getAttributes() != targetData.getAttributes()) {
		gameData.setAttributes(targetData.getAttributes());
	}

	if (gameData.getAge() != targetData.getAge()) {
		gameData.setAge(targetData.getAge());
	}
	
	if (gameData.getGender() != targetData.getGender()) {
		gameData.setGender(targetData.getGender());
	}

	if (gameData.getVoice() != targetData.getVoice()) {
		gameData.setVoice(targetData.getVoice());
	}
}

void BuildCopy::updateAndCopyEquipment()
{
	PlayerIns chr(getChrAddress().value());

	//Left hand weapons swap
	for (int32_t i = 1; i <= 3; ++i) {
		const int32_t copyCatWeapon = chr.getLeftHandWeapon(i);
		const int32_t targetWeapon = target->getLeftHandWeapon(i);

		if (copyCatWeapon != targetWeapon) {
			InventorySlot inventorySlot = InventorySlot::PrimaryLeftWep;

			switch (i) {
			case 1:
				inventorySlot = InventorySlot::PrimaryLeftWep;
				break;
			case 2:
				inventorySlot = InventorySlot::SecondaryLeftWep;
				break;
			case 3:
				inventorySlot = InventorySlot::TertiaryLeftWep;
				break;
			default:
				break;
			}
			
			giveItemAndSwap(inventorySlot,
				ItemParamIdPrefix::Weapon,
				targetWeapon,
				-1);
			
		}
	}

	//Right hand weapons swap
	for (int32_t i = 1; i <= 3; ++i) {
		const int32_t copyCatWeapon = chr.getRightHandWeapon(i);
		const int32_t targetWeapon = target->getRightHandWeapon(i);

		if (copyCatWeapon != targetWeapon) {
			InventorySlot inventorySlot = InventorySlot::PrimaryRightWep;

			switch (i) {
			case 1:
				inventorySlot = InventorySlot::PrimaryRightWep;
				break;
			case 2:
				inventorySlot = InventorySlot::SecondaryRightWep;
				break;
			case 3:
				inventorySlot = InventorySlot::TertiaryRightHand;
				break;
			default:
				break;
			}

			giveItemAndSwap(inventorySlot,
				ItemParamIdPrefix::Weapon,
				targetWeapon,
				-1);
		}
	}

	//Ring swaps
	for (int32_t i = 1; i <= 4; ++i) {
		const int32_t copyCatRing = chr.getRing(i);
		const int32_t targetRing = target->getRing(i);

		if (copyCatRing != targetRing) {
			InventorySlot inventorySlot = InventorySlot::Ring1;

			switch (i) {
			case 1:
				inventorySlot = InventorySlot::Ring1;
				break;
			case 2:
				inventorySlot = InventorySlot::Ring2;
				break;
			case 3:
				inventorySlot = InventorySlot::Ring3;
				break;
			case 4:
				inventorySlot = InventorySlot::Ring4;
				break;
			default:
				break;
			}

			if (targetRing == -1) {
				EquipGameData equipGameData(PlayerGameData(GameDataMan(GameDataMan::getInstance()).getPlayerGameData()).getEquipGameData());
				equipGameData.equipInventoryItem((InventorySlot)i, -1);
			}
			else {
				giveItemAndSwap(inventorySlot,
					ItemParamIdPrefix::Accessory,
					targetRing,
					-1);
			}
		}
	}

	//Arrow/Bolt swaps
	for (int32_t i = 1; i <= 4; ++i) {
		const int32_t copyCatAmmo = chr.getAmmo(i);
		const int32_t targetAmmo = target->getAmmo(i);

		if (copyCatAmmo != targetAmmo) {
			InventorySlot inventorySlot = InventorySlot::PrimaryArrow;

			switch (i) {
			case 1:
				inventorySlot = InventorySlot::PrimaryArrow;
				break;
			case 2:
				inventorySlot = InventorySlot::PrimaryBolt;
				break;
			case 3:
				inventorySlot = InventorySlot::SecondaryArrow;
				break;
			case 4:
				inventorySlot = InventorySlot::SecondaryBolt;
				break;
			default:
				break;
			}

			if (targetAmmo == -1) {
				EquipGameData equipGameData(PlayerGameData(GameDataMan(GameDataMan::getInstance()).getPlayerGameData()).getEquipGameData());
				equipGameData.equipInventoryItem(inventorySlot, -1);
			}
			else {
				giveItemAndSwap(inventorySlot,
					ItemParamIdPrefix::Weapon,
					targetAmmo,
					-1);
			}
		}
	}

	//Covenant swap
	const int32_t copyCatCovenant = chr.getCovenant();
	const int32_t targetCovenant = target->getCovenant();

	if (copyCatCovenant != targetCovenant) {
		
		if (targetCovenant == -1) {
			EquipGameData equipGameData(PlayerGameData(GameDataMan(GameDataMan::getInstance()).getPlayerGameData()).getEquipGameData());
			equipGameData.equipInventoryItem(InventorySlot::Covenant, -1);
		}
		else {
			giveItemAndSwap(InventorySlot::Covenant,
				ItemParamIdPrefix::Accessory,
				targetCovenant,
				-1);
		}
	}

	//Helmet swap
	const int32_t copyCatHelm = chr.getHead();
	const int32_t targetHelm = target->getHead();

	if (copyCatHelm != targetHelm) {
		giveItemAndSwap(InventorySlot::Head,
			ItemParamIdPrefix::Protector,
			targetHelm,
			-1);
	}

	//Chest swap
	const int32_t copyCatChest = chr.getChest();
	const int32_t targetChest = target->getChest();

	if (copyCatChest != targetChest) {
		giveItemAndSwap(InventorySlot::Chest,
			ItemParamIdPrefix::Protector,
			targetChest,
			-1);
	}

	//Gauntlets swap
	const int32_t copyCatGauntlets = chr.getHands();
	const int32_t targetGauntlets = target->getHands();

	if (copyCatGauntlets != targetGauntlets) {
		giveItemAndSwap(InventorySlot::Hands,
			ItemParamIdPrefix::Protector,
			targetGauntlets,
			-1);
	}

	//Leggings swap
	const int32_t copyCatLeggings = chr.getLegs();
	const int32_t targetLeggings = target->getLegs();

	if (copyCatLeggings != targetLeggings) {
		giveItemAndSwap(InventorySlot::Legs,
			ItemParamIdPrefix::Protector,
			targetLeggings,
			-1);
	}
}

void BuildCopy::tryAddSpell(const int32_t& magicId)
{
	const int32_t spellCount = targetSpells.size() + 1;
	EquipGameData equipGameData(PlayerGameData(GameDataMan(GameDataMan::getInstance()).getPlayerGameData()).getEquipGameData());
	EquipInventoryData equipInventoryData(equipGameData.getEquipInventoryData());
	equipInventoryData.addItem(ItemParamIdPrefix::Goods, magicId, 1, 0);

	if (spellCount <= 14 && getChrAddress().has_value()) {
		PlayerIns chr(getChrAddress().value());
		chr.setSpell(spellCount, magicId);
	}

	targetGoods[spellCount] = magicId;
}

void BuildCopy::tryAddGood(const int32_t& goodId)
{
	const int32_t goodCount = targetGoods.size() + 1;
	std::optional<GoodsSlot> goodsSlot;

	switch (goodCount) {
	case 1:
		goodsSlot = GoodsSlot::QuickItem1;
		break;
	case 2:
		goodsSlot = GoodsSlot::QuickItem2;
		break;
	case 3:
		goodsSlot = GoodsSlot::QuickItem3;
		break;
	case 4:
		goodsSlot = GoodsSlot::QuickItem4;
		break;
	case 5:
		goodsSlot = GoodsSlot::QuickItem5;
		break;
	case 6:
		goodsSlot = GoodsSlot::QuickItem6;
		break;
	case 7:
		goodsSlot = GoodsSlot::QuickItem7;
		break;
	case 8:
		goodsSlot = GoodsSlot::QuickItem8;
		break;
	case 9:
		goodsSlot = GoodsSlot::QuickItem9;
		break;
	case 10:
		goodsSlot = GoodsSlot::QuickItem10;
		break;
	case 11:
		goodsSlot = GoodsSlot::Toolbelt1;
		break;
	case 12:
		goodsSlot = GoodsSlot::Toolbelt2;
		break;
	case 13:
		goodsSlot = GoodsSlot::Toolbelt3;
		break;
	case 14:
		goodsSlot = GoodsSlot::Toolbelt4;
		break;
	case 15:
		goodsSlot = GoodsSlot::Toolbelt5;
		break;
	default:
		break;
	}

	int32_t giveQuantity = 99;

	if (goodId >= 0x40000096 && goodId <= 0x400000AB) { //Estus flask should not be more than 4
		giveQuantity = 4;
	}
	else if (goodId >= 0x400000BE && goodId <= 0x400000D3) { //Ashen Estus flask should not be more than 2
		giveQuantity = 2;
	}

	if (goodsSlot.has_value()) {
		giveGoodsAndSwap(goodsSlot.value(),
			goodId,
			giveQuantity);
	}
	else {
		EquipGameData equipGameData(PlayerGameData(GameDataMan(GameDataMan::getInstance()).getPlayerGameData()).getEquipGameData());
		EquipInventoryData equipInventoryData(equipGameData.getEquipInventoryData());
		equipInventoryData.addItem(ItemParamIdPrefix::Goods, goodId, giveQuantity, 0);
	}

	targetGoods[goodCount] = goodId;
}

bool BuildCopy::isPlayersLoaded()
{
	for (int32_t i = 1; i <= 5; ++i) { //Check if there is any other player loading in
		const uintptr_t playerAddress =
			PlayerIns::getAddressByOffsetNumber(static_cast<PlayerIns::OffsetNumber>(i));
		if (!PlayerIns::isPlayer(playerAddress)) continue;
		PlayerIns player(playerAddress);
		if (player.getPlayerGameData() == 0) return false;
	}
	
	return true;
}

}