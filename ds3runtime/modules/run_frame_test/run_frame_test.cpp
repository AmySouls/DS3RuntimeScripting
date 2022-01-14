#pragma once
#include "pch.h"
#include "run_frame_test.h"
#include "ds3runtime/ds3runtime.h"
#include <ds3runtime/hooks/time_since_last_frame.h>

namespace ds3runtime {

void RunFrameTest::execute()
{
	if (!PlayerIns::isMainChrLoaded() || !PlayerIns(PlayerIns::getMainChrAddress()).hasHkbCharacter()) {
		timeSinceLastMove = 0;
		isMoving = false;
		return;
	}

	PlayerIns playerChr(PlayerIns::getMainChrAddress());

	if (!isMoving && playerChr.getAnimationString() == L"Move") {
		isMoving = true;
		timeSinceLastMove = 0;
		moveStartPosition = playerChr.getPosition();
	} else if (isMoving && playerChr.getAnimationString() != L"Move") {
		isMoving = false;
		std::vector<float> currentPosition = playerChr.getPosition();

		float distance = sqrt(
			pow(moveStartPosition[0] - currentPosition[0], 2) 
			+ pow(moveStartPosition[1] - currentPosition[1], 2) 
			+ pow(moveStartPosition[2] - currentPosition[2], 2));

		spdlog::debug("You moved {} meters over {} seconds.", distance, timeSinceLastMove);
		timeSinceLastMove = 0;
	}

	auto* timeHook = (TimeSinceLastFrameHook*)ds3runtime_global->accessHook("time_since_last_frame");
	timeSinceLastMove += timeHook->getTime();
}

}