/*
	* DS3RuntimeScripting
	* Contributers: Amir
*/

#pragma once
#include "pch.h"
#include "ds3runtime/hook.h"
#include "ds3runtime/bullet_spawn.h"

namespace ds3runtime {

typedef void (*gameFrame)(void*, void*, void*, void*, void*);

class GameFrameHook : public Hook
{
public:
	GameFrameHook();

	static void onGameFrame(void*, void*, void*, void*, void*);

	std::string getName()
	{
		return "game_frame_hook";
	}
private:
	static GameFrameHook* instance;
};

}