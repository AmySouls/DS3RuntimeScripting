#pragma once
#include "pch.h"
#include "ds3_debug_variables.h"

namespace ds3runtime {

DS3DebugVariables::DS3DebugVariables()
{
}

void DS3DebugVariables::setDebugAnimSpeedPlayer(bool value)
{
	*accessMultilevelPointer<uint8_t>(0x144768F85) = value;
}

void DS3DebugVariables::setDebugMovementMultiplier(bool value)
{
	*accessMultilevelPointer<uint8_t>(0x144768F87) = value;
}

void DS3DebugVariables::setGlobalMovementMultiplier(float value)
{
	*accessMultilevelPointer<float>(0x1405DFC5C) = value;
}

uint8_t DS3DebugVariables::getOverallBonfireLevel() const
{
	return *accessMultilevelPointer<uint8_t>(0x14454A228);
}

};