/*
	* DS3RuntimeScripting
	* Contributers: Amir
*/

#pragma once
#include "pch.h"
#include "ds3runtime/hook.h"
#include <lua.h>

namespace ds3runtime {

typedef uintptr_t(__cdecl *gettop)(uintptr_t);

class LuaCapture : public Hook
{
public:
	LuaCapture();

	~LuaCapture();

	static int onLuaGetTop(lua_State* luaState);

	std::string getName()
	{
		return "lua_capture";
	}
private:
	std::vector<lua_State*> luaStates;
	std::mutex mut;
	std::condition_variable cond;

	static LuaCapture* instance;
};

}