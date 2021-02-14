/*
	* DS3RuntimeScripting
	* Contributers: Amir
*/

#pragma once
#include "pch.h"
#include "async_module.h"

namespace ds3runtime {

AsyncModule::AsyncModule() : ScriptModule::ScriptModule()
{	this->threadHandle = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)AsyncModule::entryPoint, this, 0, NULL);
}

void AsyncModule::entryPoint(AsyncModule* asyncModule)
{
	while (!asyncModule->isDestroyed()) asyncModule->execute();
	asyncModule->setSafeToDelete();
}

void AsyncModule::destroy()
{
	destroyed = true;
}

bool AsyncModule::isDestroyed()
{
	return destroyed;
}

void AsyncModule::setSafeToDelete()
{
	safeToDelete = true;
}

bool AsyncModule::isSafeToDelete()
{
	return safeToDelete;
}

HANDLE AsyncModule::getHandle()
{
	return threadHandle;
}

}