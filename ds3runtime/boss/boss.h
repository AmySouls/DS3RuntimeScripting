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
	Boss(std::shared_ptr<ChrIns> chrIns);

	void execute();

	std::shared_ptr<ChrIns> getChr();
private:
	std::shared_ptr<ChrIns> chr;
protected:
	virtual void logic() = 0;

	virtual void checks() = 0;
};

}