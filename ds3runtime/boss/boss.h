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
	Boss(ChrIns chrIns);

	void execute();
private:
	ChrIns chr;
protected:
	virtual void logic() = 0;

	virtual void checks() = 0;
};

}