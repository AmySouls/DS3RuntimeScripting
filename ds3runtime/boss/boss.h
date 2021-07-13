/*
	* DS3RuntimeScripting
	* Contributers: Amir
*/

#pragma once
#include "pch.h"
#include "ds3runtime/script_module.h"
#include "ds3runtime/chr_ins.h"

namespace ds3runtime {

class Boss : public ScriptModule
{
public:
	Boss(uint16_t forwardId);

	void execute();

	uint16_t getForwardId();

	void setForwardId(uint16_t forwardId);

	std::optional<uintptr_t> getChrAddress();
private:
	uint16_t forwardId;
protected:
	virtual void logic() = 0;

	virtual void checks() = 0;
};

}