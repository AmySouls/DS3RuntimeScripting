#pragma once
#include "pch.h"
#include "boss.h"

namespace ds3runtime {

void Boss::execute()
{
	checks();
	logic();
}

}