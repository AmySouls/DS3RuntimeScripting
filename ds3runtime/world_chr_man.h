/*
	* DS3RuntimeScripting
	* Contributers: Amir
	* Further notes on how to use this class:
*/

#pragma once
#include "pch.h"
#include "memory_util.h"
#include "chr_ins.h"

namespace ds3runtime {

class WorldChrMan
{
public:
	WorldChrMan(uintptr_t address);

	void reloadCharacterFiles(std::wstring characterId);

	void findEntityTest(int32_t id);

	uintptr_t getInsByHandle(int32_t handle);

	/**
	* Gets the camera quaternion angle.
	* 
	* @return The camera quaternion angle.
	*/
	std::array<float, 3> getCamVector() const;

	bool doesChrHaveSpEffect(ChrIns chr, const int32_t& spEffectId) const;

	static uintptr_t getInstance();

	static bool hasInstance();
private:
	uintptr_t address;
};

}