#pragma once
#include "pch.h"
#include "roll_release_test.h"
#include "ds3runtime/ds3runtime.h"
#include <ds3runtime/world_chr_man.h>
#include <ds3runtime/hooks/play_animation_hook.h>
#include <ds3runtime/scripts/hotkey_manager.h>
#include <ds3runtime/misc_functions.h>

namespace ds3runtime {

bool RollReleaseTest::onAttach()
{
	auto hotkeyManager = dynamic_cast<HotkeyManager*>(ds3runtime_global->accessScript("hotkey_manager"));

	hotkeyManager->registerKeyDownEvent("roll_release_test", [this]() {
		bool newSpaceDownState = GetKeyState(VK_SPACE) < 0;

		if (newSpaceDownState == true && spaceDownState != newSpaceDownState) {
			auto newStopwatch = sw.load();
			newStopwatch.reset();
			sw.store(newStopwatch);
			currentFrame = 0;
			spdlog::info("Key down at frame {} and timestamp {}", currentFrame, 0);
			spaceDownState = newSpaceDownState;
		}
		});

	hotkeyManager->registerKeyUpEvent("roll_release_test", [this]() {
		bool newSpaceDownState = GetKeyState(VK_SPACE) < 0;

		if (newSpaceDownState == false && spaceDownState != newSpaceDownState) {
			releaseFrame = currentFrame.load();
			releaseTimeStamp = sw.load().elapsed().count();
			spdlog::info("Key up at frame {} and timestamp {}", releaseFrame, releaseTimeStamp);
			spaceDownState = newSpaceDownState;
		}
		});

	auto playAnimHook = dynamic_cast<PlayAnimationHook*>(ds3runtime_global->accessHook("play_anim_hook"));

	playAnimHook->installFilter("roll_release_test",
		[this](uintptr_t hkbCharacter, int32_t animationPlayId) -> int32_t {
			if (!WorldChrMan::hasInstance()
				|| !PlayerIns::isMainChrLoaded()
				|| !PlayerIns(PlayerIns::getMainChrAddress()).hasHkbCharacter()) return animationPlayId;

			PlayerIns mainChr(PlayerIns::getMainChrAddress());
			std::wstring animationString = mainChr.getHkbStringFromId(animationPlayId);
			
			if (animationString.compare(L"W_RollingMedium") == 0
					|| animationString.compare(L"W_RollingMediumSelftrans") == 0
					|| animationString.compare(L"W_RollingLight") == 0
				    || animationString.compare(L"W_RollingLightSelftrans") == 0) {
				uint64_t rollFrame = currentFrame.load();
				double rollTimeStamp = sw.load().elapsed().count();
				spdlog::info("Rolled at frame {} and timestamp {} | Release to roll delay was {} frames and {} seconds.", 
					currentFrame, sw.load(), rollFrame - releaseFrame.load(), rollTimeStamp - releaseTimeStamp.load());
				totalFrames += rollFrame;
				totalTime += rollTimeStamp;
				rollCount++;
				std::ofstream file("Average_Roll_Release_Delay.txt", std::ios::out);
				if (!file) return animationPlayId;
				file << "Average frames between key down and roll: " << totalFrames / static_cast<double>(rollCount) << " | Average time between key down and roll: " << totalTime / static_cast<double>(rollCount);
			}

			return animationPlayId;
		});

	return true;
}

void RollReleaseTest::execute()
{
	currentFrame++;

	auto newStopwatch = sw.load();

	if (newStopwatch.elapsed().count() > .45) {
		newStopwatch.reset();
		sw.store(newStopwatch);
		currentFrame = 0;
	}
}

bool RollReleaseTest::onDetach()
{
	auto hotkeyManager = dynamic_cast<HotkeyManager*>(ds3runtime_global->accessScript("hotkey_manager"));
	hotkeyManager->unregisterKeyDownEvent("roll_release_test");
	hotkeyManager->unregisterKeyUpEvent("roll_release_test");
	dynamic_cast<PlayAnimationHook*>(ds3runtime_global->accessHook("play_anim_hook"))->uninstallFilter("roll_release_test");
	return true;
}

}