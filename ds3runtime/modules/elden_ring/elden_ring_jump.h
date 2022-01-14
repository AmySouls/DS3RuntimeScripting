/*
	* DS3RuntimeScripting
	* Contributers: Amir
*/

#pragma once
#include "pch.h"
#include "ds3runtime/script_module.h"
#include "ds3runtime/ds3runtime.h"
#include "spdlog/stopwatch.h"

namespace ds3runtime::eldenring {

class EldenRingJump : public ScriptModule
{
public:
	bool onAttach();

	bool onDetach();

	void execute();

	std::string getName()
	{
		return "elden_ring_jump";
	}

	void setJumpMotion(const uintptr_t& hkbChr, const std::vector<float>& motion);

	std::vector<float> getJumpMotion(const uintptr_t& hkbChr);

	template<class ReturnType>
	ReturnType env(const int& envId)
	{
		uint64_t rawData = envRaw(envId);
		return *reinterpret_cast<ReturnType*>(&rawData);
	}
private:
	std::unordered_map<int32_t, std::vector<float>> chrOverrideMotionMap;
	std::atomic<bool> waitOverrideFrame = false;

	uint64_t envRaw(const int& envId);
};

}