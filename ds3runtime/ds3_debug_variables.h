/*
	* DS3RuntimeScripting
	* Contributers: Amir
	* Further notes on how to use this class:
*/

#pragma once
#include "pch.h"
#include "memory_util.h"

namespace ds3runtime {

class DS3DebugVariables
{
public:
	DS3DebugVariables();

	void setDebugAnimSpeedPlayer(bool value);

	void setDebugMovementMultiplier(bool value);

	void setGlobalMovementMultiplier(float value);

	uint8_t getOverallBonfireLevel() const;
};

}