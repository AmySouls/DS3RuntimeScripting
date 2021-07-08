#pragma once
#include "pch.h"
#include "animation_id_handler.h"
#include "ds3runtime/ds3runtime.h"

namespace ds3runtime {

void AnimationIdHandler::execute()
{
	if (!PlayerIns::isMainChrLoaded()) return;
	uintptr_t entityAnimationHandle = (uintptr_t)accessMultilevelPointer<uintptr_t>(PlayerIns::getMainChrAddress() + 0x1F90, 0x10);
	if (!entityAnimationHandle) return;
	ChrIns entity(*accessMultilevelPointer<uintptr_t>(entityAnimationHandle, 8));
	std::wstring characterString = entity.getCharacterString();

	for (int i = 0; i < 10; i++) {
		const int animationId = *accessMultilevelPointer<uint32_t>(entityAnimationHandle, 0x20 + (uintptr_t)i * 0x10);

		if (animationIdMap[characterString][i] != animationId) {
			latestAnimationIdMap[characterString] = animationId;
		}

		animationIdMap[characterString][i] = animationId;
	}	
}

std::optional<int32_t> AnimationIdHandler::getAnimationId(ChrIns chr)
{
	if (!latestAnimationIdMap.contains(chr.getCharacterString())) return {};
	return latestAnimationIdMap[chr.getCharacterString()];
}

std::optional<std::unordered_map<int32_t, int32_t>> AnimationIdHandler::getAnimationIdBuffer(ChrIns chr)
{
	if (!animationIdMap.contains(chr.getCharacterString())) return {};
	return animationIdMap[chr.getCharacterString()];
}

}