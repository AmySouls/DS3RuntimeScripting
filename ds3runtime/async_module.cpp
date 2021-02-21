/*
	* DS3RuntimeScripting
	* Contributers: Amir
*/

#pragma once
#include "pch.h"
#include "async_module.h"

namespace ds3runtime {

AsyncModule::AsyncModule() : ScriptModule::ScriptModule()
{	
}

void AsyncModule::createThread(std::shared_ptr<ScriptModule> asyncModule)
{
	CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)AsyncModule::entryPoint, &asyncModule, 0, NULL);
}

DWORD WINAPI AsyncModule::entryPoint(std::shared_ptr<AsyncModule> asyncModule)
{
	while (!asyncModule->isDestroyed()) asyncModule->execute();
	return 0;
}

void AsyncModule::destroy()
{
	destroyed = true;
}

bool AsyncModule::isDestroyed()
{
	return destroyed;
}

}