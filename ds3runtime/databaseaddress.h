/*
	* DS3RuntimeScripting
	* Contributers =  Amir
*/

#pragma once
#include "pch.h"

namespace ds3runtime {

enum class DataBaseAddress : uintptr_t {
	GAME = 0x144740178,
	WorldChrMan = 0x144768E78,
	BaseC = 0x144743AB0,
	BaseD = 0x144743A80,
	FRPGNet = 0x14473FD08,
	SprjDebugEvent = 0x14473AD78,
	ServerIF = 0x144777FA8,
	PlayerNetworkSession = 0x14474CFF8,
	WorldChrManDbg = 0x144768F98,
	SoloParamRepository = 0x144782838,
	SprjEventFlagMan = 0x14473BE28,
	LOCK_TGT_MAN = 0x144766CA0,
	SprjSessionManager = 0x144780990,
	SprjBulletManager = 0x144772D78,
	SprjGaitem = 0x144775D58,
	SprjLuaEventConditionItem = 0x144752300,
};

}