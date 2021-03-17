/*
	* DS3RuntimeScripting
	* Contributers: Amir
*/

#pragma once
#include "pch.h"
#include "ds3runtime/hook.h"
#include "ds3runtime/chr_ins.h"

namespace ds3runtime {

class PlayAnimationHook : public Hook
{
public:
	PlayAnimationHook();

	static void onPlayAnimation(void* chrAnimationHandle, int32_t* animationId, int32_t, int32_t, int32_t);

	std::string getName()
	{
		return "play_animation_hook";
	}
private:
	static PlayAnimationHook* instance;
};

}