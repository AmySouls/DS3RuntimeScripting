/*
	* DS3RuntimeScripting
	* Contributers: Amir
*/

#pragma once
#include "pch.h"
#include "databaseaddress.h"

namespace ds3runtime {

uintptr_t getDataBaseAddress(DataBaseAddress dataBase)
{
	switch (dataBase) {
		case DataBaseAddress::GAME:
			return 0x144740178;
		case DataBaseAddress::WorldChrMan:
			return 0x144768E78;
		case DataBaseAddress::BaseC:
			return 0x144743AB0;
		case DataBaseAddress::BaseD:
			return 0x144743A80;
		case DataBaseAddress::FRPGNet:
			return 0x14473FD08;
		case DataBaseAddress::SprjDebugEvent:
			return 0x14473AD78;
		case DataBaseAddress::ServerIF:
			return 0x144777FA8;
		case DataBaseAddress::BaseH:
			return 0x14474CFF8;
		case DataBaseAddress::WorldChrManDbg:
			return 0x144768F98;
		case DataBaseAddress::SoloParamRepository:
			return 0x144782838;
		case DataBaseAddress::SprjEventFlagMan:
			return 0x14473BE28;
		case DataBaseAddress::LOCK_TGT_MAN:
			return 0x144766CA0;
		case DataBaseAddress::SprjSessionManager:
			return 0x144780990;
		case DataBaseAddress::SprjBulletManager:
			return 0x144772D78;
	}

	return 0;
}

}