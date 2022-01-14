/*
	* DS3RuntimeScripting
	* Contributers: Amir
*/

#pragma once
#include "pch.h"
#include "ds3runtime/script_module.h"
#include "ds3runtime/ds3runtime.h"
#include <spdlog/stopwatch.h>

namespace ds3runtime {

class RollReleaseTest : public ScriptModule
{
public:
	bool onAttach();

	bool onDetach();

	void execute();

	std::string getName()
	{
		return "roll_release_test";
	}
private:
	std::atomic<uint64_t> currentFrame = 0;
	std::atomic<uint64_t> releaseFrame = 0;
	std::atomic<double> releaseTimeStamp = 0;
	std::atomic<spdlog::stopwatch> sw;
	std::atomic<bool> spaceDownState;
	std::atomic<uint64_t> totalFrames = 0;
	std::atomic<double> totalTime = 0;
	std::atomic<uint64_t> rollCount = 0;
};

}