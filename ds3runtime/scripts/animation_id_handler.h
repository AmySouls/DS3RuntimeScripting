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

	uint32_t getAnimationId(ChrIns chr);
private:
	std::unordered_map<std::wstring, std::unordered_map<uint32_t, uint32_t>> animationIdMap;
	std::unordered_map<std::wstring, uint32_t> latestAnimationIdMap;
};

}