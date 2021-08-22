/*
	* DS3RuntimeScripting
	* Contributers: Amir
*/

#pragma once
#include "pch.h"
#include "script_module.h"
#include <chrono>

namespace ds3runtime {

ScriptModule::ScriptModule() : uniqueId(uniqueIdCounter)
{
	uniqueIdCounter++;
}

bool ScriptModule::onAttach()
{
	return true;
}

bool ScriptModule::onDetach()
{
	return true;
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

bool ScriptModule::isDetaching()
{
	return detaching;
}

void ScriptModule::setAttached()
{
	attached = true;
}

void ScriptModule::setDetaching()
{
	detaching = true;
}

uint64_t ScriptModule::getUniqueId()
{
	return uniqueId;
}

uint64_t ScriptModule::uniqueIdCounter = 0;

}
