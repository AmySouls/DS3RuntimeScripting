/*
	* DS3RuntimeScripting
	* Contributers: Amir
*/

#pragma once
#include "pch.h"

namespace ds3runtime {

class ScriptModule
{
public:
	ScriptModule();

	virtual void execute() = 0;

	virtual std::string getName() = 0;

	uint64_t getUniqueId();

	virtual bool isAsync()
	{
		return false;
	}
private:
	uint64_t uniqueId;
};

}