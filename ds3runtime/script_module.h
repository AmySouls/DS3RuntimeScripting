/*
	* DS3RuntimeScripting
	* Contributers: Amir
*/

#pragma once
#include "pch.h"
#include "log.h"

namespace ds3runtime {

class ScriptModule
{
public:
	ScriptModule();

	virtual void execute() = 0;

	virtual std::string getName() = 0;

	uint64_t getUniqueId();

	void setLog(Log* log);
private:
	uint64_t uniqueId;
protected:
	Log* log;
};

}