/*
	* DS3RuntimeScripting
	* Contributers: Amir
*/

#pragma once
#include "pch.h"
#include "ds3runtime/hook.h"
#include "ds3runtime/ds3runtime.h"

namespace ds3runtime{

class HasSpEffectVisual : public Hook
{
public:
	HasSpEffectVisual();
	
	std::string getName()
	{
		return "has_speffect_visual";
	}

	static int32_t onGetPhantomColorId(uintptr_t);
private:
	static HasSpEffectVisual* instance;
};

}