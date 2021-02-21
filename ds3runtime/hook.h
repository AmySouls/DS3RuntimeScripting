#pragma once
#include "pch.h"

namespace ds3runtime {

class Hook {
public:
	Hook(uintptr_t original, uintptr_t replacement);

	~Hook();

	bool install();

	bool uninstall();

	virtual std::string getName() = 0;
protected:
	uintptr_t* original;
	uintptr_t* replacement;
};

}