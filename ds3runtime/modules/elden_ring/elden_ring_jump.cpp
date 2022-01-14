#pragma once
#include "pch.h"
#include "elden_ring_jump.h"
#include "ds3runtime/ds3runtime.h"
#include <ds3runtime/world_chr_man.h>
#include "position_calculate_hook.h"
#include "ds3runtime/scripts/animation_id_handler.h"
#include <spdlog/stopwatch.h>
#include <ds3runtime/hooks/play_animation_hook.h>
#include <ds3runtime/hooks/time_since_last_frame.h>
#include <ds3runtime/databaseaddress.h>

namespace ds3runtime::eldenring {

bool EldenRingJump::onAttach()
{
	((PositionCalculateHook*)ds3runtime_global
		->accessHook("elden_ring_position_calculate_hook"))
		->installFilter("elden_ring_jump", [this](uintptr_t CsHkCharacterProxy, uintptr_t* SprjChrPhysicsModulePtr) {
		if (accessMultilevelPointer<uintptr_t>(SprjChrPhysicsModulePtr) == nullptr || accessMultilevelPointer<uintptr_t>(*SprjChrPhysicsModulePtr + 8) == nullptr) return;
		ChrIns chr(*accessMultilevelPointer<uintptr_t>(*SprjChrPhysicsModulePtr + 8));
		if (!WorldChrMan::hasInstance() || !PlayerIns::isMainChrLoaded() || !chr.isValid() || !chr.hasHkbCharacter()) return;
		if (chr.getCharacterId() != L"c0000") return;
		auto* handler = (AnimationIdHandler*)ds3runtime_global->accessScript("animation_id_handler");
		uintptr_t playerCsHkChrProxy = 
			*accessMultilevelPointer<uintptr_t>(chr.getAddress() + 0x50, 8, 0x1F90, 0x68, 0xA8, 0x40);

		if (!waitOverrideFrame.load()
				&& env<int64_t>(200)) {
			chrOverrideMotionMap[chr.getHkbCharacter()] = {
				*accessMultilevelPointer<float>(playerCsHkChrProxy + 0xA0),
				*accessMultilevelPointer<float>(playerCsHkChrProxy + 0xA4),
				*accessMultilevelPointer<float>(playerCsHkChrProxy + 0xA8)
			};

			waitOverrideFrame = false;
			return;
		}

		waitOverrideFrame = false;

		if (chrOverrideMotionMap.contains(chr.getHkbCharacter())) {
			auto* timeHook = (TimeSinceLastFrameHook*)ds3runtime_global->accessHook("time_since_last_frame");
			auto motion = chrOverrideMotionMap[chr.getHkbCharacter()];
			const double time = timeHook->getTime();
			const double acceleration = 27; //DS3 9.9 //Ideal 27 //TEST 10
			const double maxFallSpeed = 55;

			motion[1] = static_cast<float>(std::max(-maxFallSpeed, motion[1] - acceleration * time));

			*accessMultilevelPointer<float>(playerCsHkChrProxy + 0xA0) = motion[0];
			*accessMultilevelPointer<float>(playerCsHkChrProxy + 0xA4) = motion[1];
			*accessMultilevelPointer<float>(playerCsHkChrProxy + 0xA8) = motion[2];

			chrOverrideMotionMap[chr.getHkbCharacter()] = motion;
		}

		});

	auto playAnimHook = (PlayAnimationHook*)ds3runtime_global->accessHook("play_anim_hook");

	playAnimHook->installFilter("elden_ring_jump",
		[this](uintptr_t hkbCharacter, int32_t animationPlayId) -> int32_t {
			std::optional<PlayerIns> chr;

			for (int32_t i = 0; i <= 5; ++i) {
				PlayerIns playerChr(PlayerIns::getAddressByOffsetNumber(static_cast<PlayerIns::OffsetNumber>(i)));
				if (!playerChr.isValid()) continue;

				if (playerChr.hasHkbCharacter() && playerChr.getHkbCharacter() == hkbCharacter) {
					chr = playerChr;
					break;
				}
			}

			if (!chr.has_value()) return animationPlayId;

			std::wstring animationString = chr->getHkbStringFromId(animationPlayId);

			if (animationString.compare(L"W_GroundJumpStart") == 0) {
				uintptr_t playerCsHkChrProxy = 
					*accessMultilevelPointer<uintptr_t>(chr->getAddress() + 0x50, 8, 0x1F90, 0x68, 0xA8, 0x40);
				std::vector<float> jumpMotion = {
					*accessMultilevelPointer<float>(playerCsHkChrProxy + 0xA0),
					*accessMultilevelPointer<float>(playerCsHkChrProxy + 0xA4),
					*accessMultilevelPointer<float>(playerCsHkChrProxy + 0xA8)
				};
				
				jumpMotion[1] += 11;
				 
				chrOverrideMotionMap[hkbCharacter] = jumpMotion;
				waitOverrideFrame = true;
			}

			return animationPlayId;
		});

	return true;
}

void EldenRingJump::execute()
{
}

bool EldenRingJump::onDetach()
{
	((PositionCalculateHook*)ds3runtime_global
		->accessHook("elden_ring_position_calculate_hook"))
		->uninstallFilter("elden_ring_jump");
	auto playAnimHook = (PlayAnimationHook*)ds3runtime_global->accessHook("play_anim_hook");
	playAnimHook->uninstallFilter("elden_ring_jump");
	return true;
}

void EldenRingJump::setJumpMotion(const uintptr_t& hkbChr, const std::vector<float>& motion)
{
	chrOverrideMotionMap[hkbChr] = motion;
}

std::vector<float> EldenRingJump::getJumpMotion(const uintptr_t& hkbChr)
{
	return chrOverrideMotionMap[hkbChr];
}

uint64_t EldenRingJump::envRaw(const int& envId) 
{
	uintptr_t** chrBehLuaStatePtr =
		accessMultilevelPointer<uintptr_t*>(DataBaseAddress::WorldChrMan, 0x80, 0x1F90, 0x28, 0x10, 0x28);
	if (chrBehLuaStatePtr == nullptr) return false;
	uintptr_t playerInsPtr = PlayerIns::getMainChrAddress();
	uint64_t(*function)(...);
	*(uintptr_t*)&function = 0x14098F3D0;
	const uint64_t result = function(&playerInsPtr, envId, *chrBehLuaStatePtr);
	return result;
}

}