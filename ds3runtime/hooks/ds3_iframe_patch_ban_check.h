/*
	* DS3RuntimeScripting
	* Contributers: Amir
*/

#pragma once
#include "pch.h"
#include "ds3runtime/hook.h"

namespace ds3runtime::ds3IFramePatch {

class BanCheckHook : public Hook
{
public:
	BanCheckHook();

	std::string getName()
	{
		return "ds3_iframe_patch_ban_check_hook";
	}

	int32_t getMessageCount();

	void resetMessageCount();

	static uintptr_t onAnnouncementDataDelete(uintptr_t announcementMessageData, uint32_t unk0);
private:
	int32_t messageCount = 0;

	static BanCheckHook* instance;
};

}