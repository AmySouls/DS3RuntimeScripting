/*
	* DS3RuntimeScripting
	* Contributers: Amir
*/

#pragma once
#include "pch.h"
#include "ds3runtime/hook.h"
#include "ds3runtime/chr_ins.h"

namespace ds3runtime {

typedef void (*writeAnimationIds)(void*, void*, void*, void*, int64_t, void*, void*, void*, void*);

class AnimationIdHook : public Hook
{
public:
	AnimationIdHook();

	static void onWriteAnimationIds(uintptr_t entityAnimationHandle, void*, void*, void*, int64_t, void*, void*, void*, void*);

	std::string getName()
	{
		return "animation_id_hook";
	}

	uint32_t getAnimationId(ChrIns chr);
private:
	std::unordered_map<std::wstring, std::unordered_map<uint32_t, uint32_t>> animationIdMap;
	std::unordered_map<std::wstring, uint32_t> latestAnimationIdMap;

	static AnimationIdHook* instance;
};

}