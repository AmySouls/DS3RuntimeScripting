/*
	* DS3RuntimeScripting
	* Contributers: Amir
*/

#pragma once
#include "pch.h"

namespace ds3runtime {

enum class DataBaseAddress {
	GAME,
	WorldChrMan,
	BaseC,
	BaseD,
	FRPGNet,
	SprjDebugEvent,
	ServerIF,
	BaseH,
	WorldChrManDbg,
	SoloParamRepository,
	SprjEventFlagMan,
	LOCK_TGT_MAN,
	SprjSessionManager
};

uintptr_t getDataBaseAddress(DataBaseAddress dataBase);

}