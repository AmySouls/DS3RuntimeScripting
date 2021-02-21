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
	for (lua_State* iLuaState : instance->luaStates) if (luaState == iLuaState) alreadyCaptured = true;
	
	if (!alreadyCaptured) {
		static int stateNumber = 1;
		static uint64_t startTime = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
		std::lock_guard<std::mutex> lock(instance->mut);
		instance->luaStates.push_back(luaState);
		instance->cond.notify_one();
		std::string str("Captured lua state: #");
		str += std::to_string(stateNumber);
		ds3runtime_global->getLog()->info(str.c_str());
		stateNumber++;
	}

	if (luaStateTop == 1) {

	}

	return luaStateTop;
}

LuaCapture* LuaCapture::instance = nullptr;

};