#pragma once
#include "pch.h"
#include <ds3runtime/hooks/time_since_last_frame.h>
#include "ds3runtime/world_chr_man.h"

namespace ds3runtime {

TimeSinceLastFrameHook::TimeSinceLastFrameHook()
	: Hook(0x1405941d0, (uintptr_t)onUpdateTime, {})
{
	instance = this;
}

void TimeSinceLastFrameHook::onUpdateTime(float time)
{
	void(*originalFunction)(float);
	*(uintptr_t*)&originalFunction = *instance->original;
	originalFunction(time);
	const float frameRate = *accessMultilevelPointer<float>(0x144786128, 8);
	instance->time = time * (60.0f / frameRate);
}

TimeSinceLastFrameHook* TimeSinceLastFrameHook::instance = nullptr;

}