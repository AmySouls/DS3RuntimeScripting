/*
	* DS3RuntimeScripting
	* Contributers: Amir
*/

#pragma once
#include "pch.h"
#include "ds3runtime/hook.h"
#include "ds3runtime/chr_ins.h"

namespace ds3runtime {

typedef std::function<int32_t(uintptr_t, int32_t)> AnimationFilter;

class PlayAnimationHook : public Hook
{
public:
	PlayAnimationHook();

	void installFilter(std::string key, AnimationFilter function);

	void uninstallFilter(std::string key);

	std::string getName()
	{
		return "play_anim_hook";
	}

	static void onPlayAnimation(uintptr_t hkbCharacter, int32_t* animationId);
private:
	std::unordered_map<std::string, AnimationFilter> filters;

	static PlayAnimationHook* instance;
};

}