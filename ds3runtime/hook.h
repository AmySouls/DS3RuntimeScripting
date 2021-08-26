#pragma once
#include "pch.h"

namespace ds3runtime {

class Hook {
public:
	Hook(uintptr_t originalFunc, uintptr_t replacementFunc);
	
	/**
	 * Preforms the attaching of this hook via a Detours transaction.
	 *
	 * @return The result of the Detours transaction, true if no error.
	 */
	bool install();

	/**
	 * Preforms the detaching of this hook via a Detours transaction.
	 *
	 * @return The result of the Detours transaction, true if no error.
	 */
	bool uninstall();

	/**
	 * Gets the name of this hook.
	 *
	 * @return The name of this hook.
	 */
	virtual std::string getName() = 0;

	/**
	 * Gets a pointer to the original function that this hook replaces.
	 *
	 * @return Pointer to the original function.
	 */
	uintptr_t* getOriginal();
protected:
	std::unique_ptr<uintptr_t> original;
	std::unique_ptr<uintptr_t> replacement;
};


}