/*
	* DS3RuntimeScripting
	* Contributers: Amir
	* Further notes on how to use this class:
	* 
	* -Setters- Online limitations.
	* Almost any changes done by writing data to the pointers of this handle will result in desync of this bullet
	* over the network, it is therefor only recommended to use getters.
*/

#pragma once
#include "pch.h"
#include "memory_util.h"

namespace ds3runtime {

class SprjBulletState
{
public:
	SprjBulletState(uintptr_t address);

	/*
	* Check if this bullet and it's base address is valid.
	* 
	* Peformes several multi-level pointer checks.
	* 
	* @return True if bullet is valid.
	*/
	bool isValid();

	/*
	* Gets the coordinates of this bullet. 
	* 
	* @return A 3-value vector that contains the coordinates of this bullet. 
	*/
	std::vector<float> getCoordinates();

	/*
	* Gets the initial launch vector of this bullet. 
	* 
	* This is not the active direction the bullet, this is the absolute direction vector that the bullet initially launched 
	* off of.
	*
	* @return A 3-value vector that contains the vector this bullet initially launched off of. 
	*/
	std::vector<float> getLaunchVector();

	/*
	* Gets the time elapsed since this bullet was launched.
	* 
	* @return The time elapsed since this bullet was launched as a floating point value. 
	*/
	float getDuration();

	/*
	* Gets the "Bullet" param id of this bullet.
	* 
	* @return The "Bullet" param id of this bullet.
	*/
	int32_t getId();

	/*
	* Gets the handle id which is used to identify the owning ChrIns of this bullet.
	* 
	* @return The handle id which is used to identify the owning ChrIns of this bullet.
	*/
	int32_t getOwnerHandle();

	static bool isBullet(uintptr_t address);
private:
protected:
	uintptr_t address;
};

}