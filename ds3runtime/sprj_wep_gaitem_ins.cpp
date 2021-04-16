#pragma once
#include "pch.h"
#include "sprj_wep_gaitem_ins.h"

namespace ds3runtime {

int32_t SprjWepGaitemIns::getDurablity()
{
	return *accessMultilevelPointer<int32_t>(getAddress() + 0x10);
}

}