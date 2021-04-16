#pragma once
#include "pch.h"
#include "boss.h"

namespace ds3runtime {

Boss::Boss(ChrIns chrIns) : chr(chrIns)
{
}

void Boss::execute()
{
	checks();
	logic();
}

}