/*
	* DS3RuntimeScripting
	* Contributers: Amir
*/

#pragma once
#include "pch.h"
#include "ds3runtime/script_module.h"
#include "ds3runtime/player_ins.h"

namespace ds3runtime {

class AnimationIdHandler : public ScriptModule
{
public:
	void execute();

	std::string getName()
	{
		return "animation_id_handler";
	}

	std::optional<int32_t> getAnimationId(ChrIns chr);

	std::optional<std::unordered_map<int32_t, int32_t>> getAnimationIdBuffer(ChrIns chr);
private:
	std::unordered_map<PlayerIns::Handle, std::unordered_map<int32_t, int32_t>> animationIdMap;
	std::unordered_map<PlayerIns::Handle, int32_t> latestAnimationIdMap;
};

}