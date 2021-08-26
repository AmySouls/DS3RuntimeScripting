/*
	* DS3RuntimeScripting
	* Contributers: Amir
*/

#pragma once
#include "pch.h"
#include "async_module.h"
#include "ds3runtime.h"

namespace ds3runtime {

AsyncModule::AsyncModule()
{	
}

void AsyncModule::createThread(ScriptModule* asyncModule)
{	
	const uint64_t uniqueId = asyncModule->getUniqueId();
	CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)AsyncModule::entryPoint, (LPVOID)uniqueId, 0, NULL);
}

DWORD WINAPI AsyncModule::entryPoint(uint64_t scriptUniqueId)
{
	ScriptModule* scriptModule = ds3runtime_global->accessScript(scriptUniqueId);
	if (scriptModule == nullptr) return 0;
	AsyncModule* asyncModule = (AsyncModule*)scriptModule;
	while (asyncModule != nullptr && !asyncModule->isRemoved()) asyncModule->execute();
	return 0;
}

void AsyncModule::sleep(uint32_t milliseconds)
{
	std::this_thread::sleep_for(std::chrono::milliseconds(milliseconds));
}

}