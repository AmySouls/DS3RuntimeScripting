/*
	* DS3RuntimeScripting
	* Contributers: Amir
*/

#pragma once
#include "pch.h"
#include "script_module.h"
#include <chrono>

namespace ds3runtime {

ScriptModule::ScriptModule()
{
	this->uniqueId = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
}

uint64_t ScriptModule::getUniqueId()
{
	return uniqueId;
}

}