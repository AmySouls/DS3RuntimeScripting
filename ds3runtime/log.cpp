#pragma once
#include "pch.h"
#include "log.h"

namespace ds3runtime {

Log::Log(HANDLE consoleHandle)
{
	this->consoleHandle = consoleHandle;
}

}