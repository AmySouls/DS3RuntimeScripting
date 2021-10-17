/*
	* DS3RuntimeScripting
	* Contributers: Amir
*/

#pragma once
#include "pch.h"
#include "ds3runtime/hook.h"

namespace ds3runtime {

class JmpTableStatus : public Hook
{
public:
	JmpTableStatus();

	bool getJmpTableStatus(const int32_t& jmpTableId);

	std::string getName()
	{
		return "jmp_table_status";
	}

	void tickJmpTableStatus();

	static uintptr_t onJmpTableTaeEvent(uintptr_t sprjChrTaeAnimEvent, uintptr_t unkData);
private:
	void updateJmpTableStatus(const int32_t& jmpTableId);
	std::mutex mtx;
	std::unordered_map<int32_t, int32_t> jmpTableStatusTable;

	static JmpTableStatus* instance;
};

}