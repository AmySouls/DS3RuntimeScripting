/*
	* DS3RuntimeScripting
	* Contributers: Amir
*/

#pragma once
#include "pch.h"
#include "ds3runtime/hook.h"
#include "ds3runtime/ds3runtime.h"

namespace ds3runtime{

class TimeSinceLastFrameHook : public Hook
{
public:
	TimeSinceLastFrameHook();
	
	std::string getName()
	{
		return "time_since_last_frame";
	}

	float getTime() {
		return this->time;
	}

	static void onUpdateTime(float time);
private:
	float time = 0;

	static TimeSinceLastFrameHook* instance;
};

}