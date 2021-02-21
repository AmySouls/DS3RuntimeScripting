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

	static void createThread(std::shared_ptr<ScriptModule> asyncModule);

	void destroy();

	bool isDestroyed();

	bool isAsync()
	{
		return true;
	}
private:
	static DWORD WINAPI entryPoint(std::shared_ptr<AsyncModule> asyncModule);

	bool destroyed = false;
	bool safeToDelete = false;
};

}