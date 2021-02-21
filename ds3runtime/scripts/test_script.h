/*
	* DS3RuntimeScripting
	* Contributers: Amir
*/

#pragma once
#include "pch.h"
#include "ds3runtime/log.h"
#include "ds3runtime/script_module.h"

namespace ds3runtime {

class TestScript : public ScriptModule
{
public:
	void execute();

	std::string getName()
	{
		return "test_script";
	}
private:
	std::wstring lastAnimationString;
};

}