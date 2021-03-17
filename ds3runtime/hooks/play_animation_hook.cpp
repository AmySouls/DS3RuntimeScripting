/*
	* DS3RuntimeScripting
	* Contributers: Amir
*/

#pragma once
#include "pch.h"
#include "play_animation_hook.h"
#include "ds3runtime/ds3runtime.h"
#include "ds3runtime/player_ins.h"
#include <ds3runtime/databaseaddress.h>
#include "ds3runtime/sprj_gaitem_imp.h"

namespace ds3runtime {

PlayAnimationHook::PlayAnimationHook()
	: Hook(0x140D84870, (uintptr_t)onPlayAnimation)
{
	instance = this;
}

void PlayAnimationHook::onPlayAnimation(void* chrAnimationHandle, int32_t* animationId, int32_t r8, int32_t r9, int32_t rsp20)
{
	void (*originalFunction)(...);
	*(uintptr_t*)&originalFunction = *instance->original;
	originalFunction(chrAnimationHandle, animationId, r8, r9, rsp20);
	uintptr_t* chrAddrPtr = accessMultilevelPointer<uintptr_t>(DataBaseAddress::WorldChrMan, 0x80);
	if (!chrAddrPtr) return;
	uintptr_t chrAddr = PlayerIns::getMainChrAddress();
	if (!PlayerIns::isMainChr(chrAddr)) return;
	PlayerIns chr(chrAddr);
	PlayerIns player1(PlayerIns::getAddressByOffsetNumber(PlayerIns::OffsetNumber::Player1));
	
	//auto list = SprjGaitemImp(SprjGaitemImp::getInstance()).getItemList();
	//for (auto item : list) spdlog::debug("Item id in inventory: {}", item.getId());
}

PlayAnimationHook* PlayAnimationHook::instance = nullptr;

}