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
	void execute();

	virtual std::optional<uintptr_t> getChrAddress() = 0;

	virtual std::optional<int32_t> getAnimationId() = 0;
protected:
	virtual void logic() = 0;

	virtual void checks() = 0;
};

}
