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

	static void ds3_lua_getglobal(lua_State* luaState, int idx); //14112af90

	static void ds3_lua_gettable(lua_State* luaState, int idx); //14112af90

	static void ds3_lua_pushstring(lua_State* luaState, const char* s);

	static void ds3_lua_pushnumber(lua_State* luaState, lua_Number n);

	static void ds3_lua_call(lua_State* luaState, int nargs, int nresults);

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