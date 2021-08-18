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
private:
    std::function<std::optional<uintptr_t>()> findChr;
protected:
	virtual void logic() = 0;

	virtual void checks() = 0;
};

}