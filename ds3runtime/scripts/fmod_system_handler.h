/*
	* DS3RuntimeScripting
	* Contributers: Amir
*/

#pragma once
#include "pch.h"
#include "ds3runtime/script_module.h"

namespace ds3runtime {

class FMODSystemHandler : public ScriptModule
{
public:
	bool onAttach();

	void onDetach();

	void execute();

	std::string getName()
	{
		return "fmod_system_handler";
	}

	FMOD::System* getFMODSystem();
private:
	FMOD::System* fmodSystem;
};

}