/*
	* DS3RuntimeScripting
	* Contributers: Amir
*/

#pragma once
#include "pch.h"
#include "ds3runtime/script_module.h"
#include "ds3runtime/ds3runtime.h"
#include "spdlog/stopwatch.h"

namespace ds3runtime::eldenring {

class BackstabAngleCheck : public ScriptModule
{
public:
	bool onAttach();

	bool onDetach();

	void execute();

	std::string getName()
	{
		return "backstab_angle_check";
	}
};

}