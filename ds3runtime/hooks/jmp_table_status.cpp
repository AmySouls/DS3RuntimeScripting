#pragma once
#include "pch.h"
#include "jmp_table_status.h"
#include "ds3runtime/memory_util.h"

namespace ds3runtime {

JmpTableStatus::JmpTableStatus()
	: Hook(0x1409a8ac0, (uintptr_t)onJmpTableTaeEvent, {})
{
	instance = this;
}

bool JmpTableStatus::getJmpTableStatus(const int32_t& jmpTableId)
{
	return jmpTableStatusTable.contains(jmpTableId);
}

void JmpTableStatus::updateJmpTableStatus(const int32_t& jmpTableId)
{
	jmpTableStatusTable[jmpTableId] = 0;
}

void JmpTableStatus::tickJmpTableStatus()
{
	for (auto it = std::begin(jmpTableStatusTable); it != std::end(jmpTableStatusTable);) {
		if (it->second == 1) {
			it = jmpTableStatusTable.erase(it);
		}
		else {
			it->second += 1;
			++it;
		}
	}
}

uintptr_t JmpTableStatus::onJmpTableTaeEvent(uintptr_t sprjChrTaeAnimEvent, uintptr_t unkData)
{
	const int32_t jmpTableId = *accessMultilevelPointer<int32_t>(unkData + 8, 0);
	instance->mtx.lock();
	
	instance->updateJmpTableStatus(jmpTableId);

	instance->mtx.unlock();
	uintptr_t(*originalFunction)(...);
	*(uintptr_t*)&originalFunction = *instance->original;
	return originalFunction(sprjChrTaeAnimEvent, unkData);
}

JmpTableStatus* JmpTableStatus::instance = nullptr;

}