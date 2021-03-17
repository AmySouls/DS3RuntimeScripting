/*
	* DS3RuntimeScripting
	* Contributers: Amir
*/

#pragma once
#include "pch.h"
#include "animation_id_hook.h"
#include "ds3runtime/ds3runtime.h"
#include "spdlog/stopwatch.h"

namespace ds3runtime {

AnimationIdHook::AnimationIdHook()
	: Hook(0x1409AAC60, (uintptr_t)onWriteAnimationIds)
{
	instance = this;
}

void AnimationIdHook::onWriteAnimationIds(uintptr_t entityAnimationHandle, void* arg2, void* arg3, void* arg4, int64_t args5, void* arg6, void* arg7, void* arg8, void* arg9)
{
	void (*originalFunction)(...);
	*(uintptr_t*)&originalFunction = *instance->original;
	originalFunction(entityAnimationHandle, arg2, arg3, arg4, args5, arg6, arg7, arg8, arg9);
	if (GetCurrentThreadId() != ds3runtime_global->getGameThreadId()) return;
	
	ChrIns entity(*accessMultilevelPointer<uintptr_t>(entityAnimationHandle + 8));
	std::wstring characterString = entity.getCharacterString();

	for (int i = 0; i < 10; i++) {
		const int animationId = *accessMultilevelPointer<uint32_t>(entityAnimationHandle + 0x20 + (uintptr_t)i * 0x10);

		if (instance->animationIdMap[characterString][i] != animationId) {
			instance->latestAnimationIdMap[characterString] = animationId;
		}

		instance->animationIdMap[characterString][i] = animationId;
	}
}

uint32_t AnimationIdHook::getAnimationId(ChrIns chr)
{
	return latestAnimationIdMap[chr.getCharacterString()];
}

AnimationIdHook* AnimationIdHook::instance = nullptr;

}