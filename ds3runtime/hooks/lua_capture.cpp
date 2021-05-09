/*
	* DS3RuntimeScripting
	* Contributers: Amir
*/

#pragma once
#include "pch.h"
#include "lua_capture.h"
#include "ds3runtime/ds3runtime.h"

namespace ds3runtime {

LuaCapture::LuaCapture()
	: Hook(0x14112fcd0, (uintptr_t)onLuaGetTop), mut(), cond()
{
	instance = this;
	this->luaStates = std::vector<lua_State*>();
}

LuaCapture::~LuaCapture()
{
}

int LuaCapture::onLuaGetTop(lua_State* luaState)
{
	int(*originalFunction)(lua_State*);
	*(uintptr_t*)&originalFunction = *instance->original;
	int luaStateTop = originalFunction(luaState);

	bool alreadyCaptured = false;
	std::lock_guard<std::mutex> lock(instance->mut);
	for (lua_State* iLuaState : instance->luaStates) if (luaState == iLuaState) alreadyCaptured = true;
	
	if (!alreadyCaptured) {
		static int stateNumber = 1;
		static uint64_t startTime = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
		instance->luaStates.push_back(luaState);
		stateNumber++;
		/*
		lua_getglobal(luaState, "hkbSelf");
		const void* hkbSelf = lua_topointer(luaState, -1);
		lua_remove(luaState, -1);
		spdlog::debug("hkbSelf??: {}", hkbSelf);
		*/
	}

	instance->cond.notify_one();

	return luaStateTop;
}

LuaCapture* LuaCapture::instance = nullptr;

};