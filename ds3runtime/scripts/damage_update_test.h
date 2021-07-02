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

class DamageUpdateTest : public ScriptModule
{
public:
	bool onAttach();

	void onDetach();

	void execute();

	std::string getName()
	{
		return "damage_update_test";
	}
private:
	uint64_t currentFrame = 0;
	uint64_t lastFrameWithHitReceive = 0;
	uint64_t totalFramesBetweenHitReceives = 0;
	spdlog::stopwatch lastHitSendTime;
	spdlog::stopwatch lastHitReceiveTime;
	std::atomic<bool> didReceiveHitOnThisFrame = false;
};

}