#pragma once
#include "pch.h"
#include "boss.h"

namespace ds3runtime {

Boss::Boss(uint16_t forwardId)
{
	this->forwardId = forwardId;
}

void Boss::execute()
{
	checks();
	logic();
}

uint16_t Boss::getForwardId()
{
	return forwardId;
}

void Boss::setForwardId(uint16_t forwardId)
{
	this->forwardId = forwardId;
}

std::optional<uintptr_t> Boss::getChrAddress()
{
	return {}; //To be implemented
}

}