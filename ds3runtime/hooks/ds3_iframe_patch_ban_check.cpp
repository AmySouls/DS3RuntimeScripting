/*
	* DS3RuntimeScripting
	* Contributers: Amir
*/

#pragma once
#include "pch.h"
#include "ds3runtime/ds3runtime.h"
#include "ds3_iframe_patch_ban_check.h"
#include "ds3runtime/scripts/ds3_iframe_patch.h"
#include "ds3runtime/scripts/fmod_system_handler.h"

namespace ds3runtime::ds3IFramePatch {

BanCheckHook::BanCheckHook()
	: Hook(0x14222bba0, (uintptr_t)onAnnouncementDataDelete)
{
	instance = this;
}

int32_t BanCheckHook::getMessageCount()
{
	return messageCount;
}

void BanCheckHook::resetMessageCount()
{
	messageCount = 0;
}

uintptr_t BanCheckHook::onAnnouncementDataDelete(uintptr_t announcementMessageData, uint32_t unk0)
{
	uintptr_t (*originalFunction)(...);
	*(uintptr_t*)&originalFunction = *instance->original;
	instance->messageCount++;
	return originalFunction(announcementMessageData, unk0);
}

BanCheckHook* BanCheckHook::instance = nullptr;

}