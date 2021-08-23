/*
	* DS3RuntimeScripting
	* Contributers: Amir
*/

#pragma once
#include "pch.h"

namespace ds3runtime {

class ScriptModule
{
public:
	ScriptModule();

	/**
	 * Execute this script's loop logic which is meant to be called per-game frame.
	 */
	virtual void execute() = 0;

	/**
	 * Get's the name of this script. If you are intent on running more than one of the same script, prefer retrievel by unique id instead.
	 *
	 * @return The name of this script.
	 */
	virtual std::string getName() = 0;

	/**
	 * Execute this script's attach logic. It should preform all nessessary checks to see if it can preform all the attach events on the current frame.
	   If it returns false, the script should not be attached and instead wait for the next game frame to conditions to change. For example, if
	   a script that modifies the player character once on attaching is ran while in a loading screen, it will need to wait for the player
	   character to load to make these changes and properly attach.
	 *
	 * @return true on successfully preforming the attach events.
	 */
	virtual bool onAttach();

	/**
	 * Execute this script's removal logic. It should preform all nessessary checks to see if it can preform all the events meant to be called when 
	   this script detaches.
	   If it returns false, it should similarly to onAttach() wait until conditions allow it to call it's events and detach.
	 *
	 * @return true on successfully detaching.
	 */
	virtual bool onDetach();

	/**
	 * Checks Indicates wheather or not this script is meant to be run only in async-mode.
	 * @return true if this script is async.
	 */
	virtual bool isAsync()
	{
	    return false;
	}

	/**
	 * Marks this script to be removed. It's up to the clients of this script to actually stop executing and destroy the script.
	 */
	void remove();

	/**
	 * Checks wheather or not this script has been marked to be removed.
	 *
	 * @return true if this script is marked for removal.
	 */
	bool isRemoved();

	/**
	 * Checks wheather or not this script has been successfully attached.
	 *
	 * @return true if this script has successfully attached.
	 */
	bool isAttached();

	/**
	 * Checks if this script is trying to detach. See onAttach() and onDetach() for more details about how the attach and detach process work.
	 *
	 * @return true if this script is trying to detach.
	 */
	bool isDetaching();

	/**
	 * Marks this script as attached. See onAttach() and onDetach() for more details about how the attach and detach process work.
	 */
	void setAttached();

	/**
	 * Marks this script as detaching. See onAttach() and onDetach() for more details about how the attach and detach process work.
	 */
	void setDetaching();

	/**
	 * Get's the unique id associated with this script instance.
	 *
	 * @return The unique id associated with this script instance.
	 */
	uint64_t getUniqueId();
private:
	const uint64_t uniqueId;
	static uint64_t uniqueIdCounter;
	bool removed = false;
	bool attached = false;
	bool detaching = false;
};

}
