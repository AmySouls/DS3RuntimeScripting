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

	static void entryPoint(AsyncModule* asyncModule);

	void destroy();

	bool isDestroyed();

	void setSafeToDelete();

	bool isSafeToDelete();

	HANDLE getHandle();
private:
	HANDLE threadHandle;
	bool destroyed = false;
	bool safeToDelete = false;
};

}