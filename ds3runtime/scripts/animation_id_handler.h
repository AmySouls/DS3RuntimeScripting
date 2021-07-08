/*
	* DS3RuntimeScripting
	* Contributers: Amir
*/

#pragma once
#include "pch.h"
#include "ds3runtime/script_module.h"
#include "ds3runtime/chr_ins.h"

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
	std::unordered_map<std::wstring, std::unordered_map<int32_t, int32_t>> animationIdMap;
	std::unordered_map<std::wstring, int32_t> latestAnimationIdMap;
};

}