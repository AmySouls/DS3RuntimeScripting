#pragma once
#include "pch.h"
#include "animation_id_handler.h"
#include "ds3runtime/ds3runtime.h"

namespace ds3runtime {

void AnimationIdHandler::execute()
{
	if (!PlayerIns::isMainChrLoaded()) return;

	for (int32_t i = 0; i <= 5; ++i) {
		if (!PlayerIns::isChrWithOffsetNumber(static_cast<PlayerIns::OffsetNumber>(i))) continue;
		const uintptr_t playerAddress = PlayerIns::getAddressByOffsetNumber(static_cast<PlayerIns::OffsetNumber>(i));
		PlayerIns chr(playerAddress);
		uintptr_t entityAnimationHandle = reinterpret_cast<uintptr_t>(accessMultilevelPointer<uintptr_t>(chr.getAddress() + 0x1F90, 0x10));
		if (!entityAnimationHandle) return;
		PlayerIns::Handle characterHandle = chr.getHandle();

		for (int i = 0; i < 10; i++) {
			const int animationId = *accessMultilevelPointer<uint32_t>(entityAnimationHandle, 0x20 + (uintptr_t)i * 0x10);

			if (animationIdMap[characterHandle][i] != animationId) {
				latestAnimationIdMap[characterHandle] = animationId;
			}

			animationIdMap[characterHandle][i] = animationId;
		}	
	}
}

std::optional<int32_t> AnimationIdHandler::getAnimationId(ChrIns chr)
{
	if (!latestAnimationIdMap.contains(chr.getHandle())) return {};
	return latestAnimationIdMap[chr.getHandle()];
}

std::optional<std::unordered_map<int32_t, int32_t>> AnimationIdHandler::getAnimationIdBuffer(ChrIns chr)
{
	if (!animationIdMap.contains(chr.getHandle())) return {};
	return animationIdMap[chr.getHandle()];
}

}