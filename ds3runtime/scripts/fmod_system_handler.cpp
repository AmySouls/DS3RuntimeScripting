/*
	* DS3RuntimeScripting
	* Contributers: Amir
*/

#pragma once
#include "pch.h"
#include "fmod_system_handler.h"
#include "ds3runtime/ds3runtime.h"

namespace ds3runtime {

bool FMODSystemHandler::onAttach()
{
	FMOD_RESULT result = FMOD::System_Create(&fmodSystem);

	if (result != FMOD_OK) {
		printf("FMOD error at FMODSystemHandler::onAttach, call of FMOD::System_Create: (%d) %s\n", 
			result, FMOD_ErrorString(result));
		return true;
	}

	result = fmodSystem->init(512, FMOD_INIT_NORMAL, nullptr);

	if (result != FMOD_OK) {
		printf("FMOD error at FMODSystemHandler::onAttach, call of FMOD::System::init: (%d) %s\n",
			result, FMOD_ErrorString(result));
	}

	return true;
}

void FMODSystemHandler::onDetach()
{
	if (!fmodSystem) return;
	fmodSystem->release();
}

void FMODSystemHandler::execute()
{
	if (!fmodSystem) return;
	fmodSystem->update();
}

FMOD::System* FMODSystemHandler::getFMODSystem()
{
	return fmodSystem;
}

}