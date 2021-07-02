/*
	* DS3RuntimeScripting
	* Contributers: Amir
*/

#pragma once
#include "pch.h"
#include "game_frame_hook.h"
#include "ds3runtime/ds3runtime.h"
#include "spdlog/stopwatch.h"

namespace ds3runtime {

GameFrameHook::GameFrameHook()
	: Hook(0x1423842C0, (uintptr_t)onGameFrame)
{
	instance = this;
}

void GameFrameHook::onGameFrame(void* rcx, void* rdx, void* r8, void* r9, void* rsp20)
{
	void(*originalFunction)(...);
	*(uintptr_t*)&originalFunction = *instance->original;
	originalFunction(rcx, rdx, r8, r9, rsp20);
	if (ds3runtime_global->getGameThreadId() == 0) ds3runtime_global->setGameThreadId(GetCurrentThreadId());
	static uint64_t uniqueFrameClock = 0;
	uniqueFrameClock++;
	if (uniqueFrameClock % 2 == 0) ds3runtime_global->executeScripts();
}

GameFrameHook* GameFrameHook::instance = nullptr;

}