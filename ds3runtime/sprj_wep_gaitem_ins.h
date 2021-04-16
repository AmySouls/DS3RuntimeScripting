/*
	* DS3RuntimeScripting
	* Contributers: Amir
*/

#pragma once
#include "pch.h"
#include "memory_util.h"
#include "sprj_gaitem_ins.h"

namespace ds3runtime {

class SprjWepGaitemIns : public SprjGaitemIns
{
public:
	int32_t getDurablity();
};

}