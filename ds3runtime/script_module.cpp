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
	uniqueId = uniqueIdCounter;
	uniqueIdCounter++;
}

bool ScriptModule::onAttach()
{
	return true;
}

void ScriptModule::onDetach()
{
}

void ScriptModule::remove()
{
	removed = true;
}

bool ScriptModule::isRemoved()
{
	return removed;
}

bool ScriptModule::isAttached()
{
	return attached;
}

void ScriptModule::tryAttach(const bool attachState)
{
	attached = attachState;
}

uint64_t ScriptModule::getUniqueId()
{
	return uniqueId;
}

uint64_t ScriptModule::uniqueIdCounter = 0;

}