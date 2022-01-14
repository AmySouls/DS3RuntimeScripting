/*
	* DS3RuntimeScripting
	* Contributers: Amir
*/

#pragma once
#include "pch.h"
#include "ds3runtime/hook.h"

namespace ds3runtime::eldenring {

typedef std::function<void(uintptr_t, uintptr_t*)> MotionFilter;

class PositionCalculateHook : public Hook
{
public:
	PositionCalculateHook();

	std::string getName()
	{
		return "elden_ring_position_calculate_hook";
	}

	static void onUpdatePosition(uintptr_t CsHkCharacterProxy, uintptr_t* SprjChrPhysicsModulePtr, uintptr_t unk0, uintptr_t unk1, uintptr_t unk2);

	void installFilter(std::string key, MotionFilter function);

	void uninstallFilter(std::string key);
private:
	std::unordered_map<std::string, MotionFilter> filters;

	static PositionCalculateHook* instance;
};

}