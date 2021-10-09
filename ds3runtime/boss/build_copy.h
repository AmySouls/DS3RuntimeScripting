/*
	* DS3RuntimeScripting
	* Contributers: Amir
*/

#pragma once
#include "pch.h"
#include "standard_player_boss.h"
#include "ds3runtime/game_packet.h"

namespace ds3runtime {

enum class TargetType {
	Host,
	Invader,
	SteamId,
	Arena
};

class BuildCopy : public StandardPlayerBoss
{
public:
	bool onAttach();

	bool onDetach();

	std::string getName()
	{
		return "build_copy";
	}
protected:
	void logic();

	void checks();
private:
	std::optional<PlayerIns> target;
	std::unordered_map<int32_t, int32_t> targetGoods;
	std::unordered_map<int32_t, int32_t> targetSpells;
	std::optional<packet::PlayerStruct> localPlayerStruct;
	std::optional<packet::PlayerStruct> targetPlayerStruct;
	TargetType targetType = TargetType::Host;
	std::optional<std::wstring> targetSteamId;

	inline std::optional<PlayerIns> findNewTarget();

	void updateAndCopyEquipment();

	void copyPlayerData();

	void tryAddSpell(const int32_t& magicId);

	void tryAddGood(const int32_t& goodId);

	void tryCopyGesture();

	bool isPlayersLoaded();
};

}