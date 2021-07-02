/*
	* DS3RuntimeScripting
	* Contributers: Amir
*/

#pragma once
#include "pch.h"
#include "ds3runtime/script_module.h"
#include "ds3runtime/ds3runtime.h"

namespace ds3runtime {

class InterruptAttacks : public ScriptModule
{
public:
	bool onAttach();

	void onDetach();

	void execute();

	std::string getName()
	{
		return "interrupt_attacks";
	}
};

}