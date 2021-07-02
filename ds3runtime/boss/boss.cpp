#pragma once
#include "pch.h"
#include "boss.h"

namespace ds3runtime {

Boss::Boss(std::shared_ptr<ChrIns> chrIns) : chr(chrIns)
{
}

void Boss::execute()
{
	checks();
	logic();
}

std::shared_ptr<ChrIns> Boss::getChr()
{
	return chr;
}

}