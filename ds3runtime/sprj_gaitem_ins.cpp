#pragma once
#include "pch.h"
#include "sprj_gaitem_ins.h"

namespace ds3runtime {

SprjGaitemIns::SprjGaitemIns(uintptr_t address)
{
	this->address = address;
}

int32_t SprjGaitemIns::getId()
{
	return *accessMultilevelPointer<int32_t>(address + 0xC);
}

bool SprjGaitemIns::isValid()
{
	auto vtablePtr = accessMultilevelPointer<uintptr_t>(address);
	return vtablePtr && (*vtablePtr == 0x14289A8C8 || *vtablePtr == 0x14289A7D8);
}

uintptr_t SprjGaitemIns::getVtablePtr()
{
	return *accessMultilevelPointer<uintptr_t>(address);
}

uintptr_t SprjGaitemIns::getAddress()
{
	return address;
}

}