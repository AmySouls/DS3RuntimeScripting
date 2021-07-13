/*
	* DS3RuntimeScripting
	* Contributers: Amir
*/

#pragma once
#include "script_module.h"

namespace ds3runtime {

class AsyncModule : public ScriptModule
{
public:
	AsyncModule();

	static void createThread(ScriptModule* asyncModule);

	bool isAsync()
	{
		return true;
	}
private:
	void sleep(uint32_t milliseconds);

	static DWORD WINAPI entryPoint(uint64_t scriptUniqueId);
};

}