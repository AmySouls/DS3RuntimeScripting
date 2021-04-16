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

	virtual void onAttach();

	virtual void onDetach();

	virtual bool isAsync()
	{
		return false;
	}

	void remove();

	bool isRemoved();

	uint64_t getUniqueId();
private:
	uint64_t uniqueId;
	static uint64_t uniqueIdCounter;
	bool removed = false;
};

}