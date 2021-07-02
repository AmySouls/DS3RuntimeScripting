/*
	* DS3RuntimeScripting
	* Contributers: Amir
*/

#pragma once
#include "pch.h"

namespace ds3runtime {

class SprjChrTimeActModule
{
public:
	SprjChrTimeActModule(uintptr_t address);

	/*
	* Get's the current amount of health points this Chr holds
	* 
	* This controls several events on the player and other entities, almost universally reaching zero starts a
	* death sequence. Some at one, or several hundreds above it.
	* 
	* @return Current amount of health points this Chr holds.
	*/
	uint32_t getHealth();


	/*
	* Set's the current amount of health points this Chr holds. 
	* 
	* This controls several events on the player and other entities, almost universally reaching zero starts a 
	* death sequence. Some at one, or several hundreds above it. 
	* 
	* Online limitations: 
	* { 
	* Self = RELAY, 
	* Players = DESYNC, 
	* NPC = DESYNC SOMETIMES
	* } 
	* 
	* A work around can be achieved with network utilities.
	* 
	* @param health Current amount of health points this Chr should hold.
	*/
	void setHealth(uint32_t health);

	uint32_t getStamina();

	void setStamina(uint32_t stamina);

	uint32_t getFP();

	void setFP(uint32_t fp);

	/*
	* Get's the maximum amount of health points this Chr can hold.
	* 
	* @return Maximum amount of health points this Chr can hold.
	*/
	uint32_t getMaxHealth();

	/*
	* Set's the maximum amount of health points this Chr can hold.
	* 
	* This value is constantly being calculated and written to based on base max health and other 
	* variables such as effect modifiers. Either use effects, adjust base max health, or use 
	* patch utilities to adjust this value to what you intend it to be. 
	* 
	* Online limitations: 
	* { 
	* Self = RELAY, 
	* Players = DESYNC, 
	* NPC = DESYNC 
	* } 
	* 
	* A work around can be acheived with effects
	* 
	* @param maxHealth Maximum amount of health points this Chr should hold.
	*/
	void setMaxHealth(uint32_t maxHealth);

	uint32_t getMaxStamina();

	void setMaxStamina(uint32_t maxStamina);

	uint32_t getMaxFP();

	void setMaxFP(uint32_t maxFp);

	/*
	* Get's the base maximum amount of health points this Chr can hold. 
	* 
	* This is the value evaluated from stats or an Chr's base health parameters and the value before 
	* being multiplied by effect modifiers. For your character, It is only evaluated on load or per update 
	* to stats(Eg. leveling up or equipping or unequipping stat level increasing rings). 
	* 
	* @return Base maximum amount of health points this Chr can hold.
	*/
	uint32_t getBaseMaxHealth();

	/*
	* Set's the base maximum amount of health points this Chr can hold. 
	* 
	* Setting this will adjust your max health, but only after taking this value and multiplying it with any and 
	* all effects that adjust your max health. 
	* 
	* Online limitations: 
	* { 
	* Self = RELAY, 
	* Players = DESYNC, 
	* NPC = DESYNC 
	* } 
	* 
	* @param baseMaxHealth Base maximum amount of health points this Chr should hold.
	*/
	void setBaseMaxHealth(uint32_t baseMaxHealth);

	uint32_t getBaseMaxStamina();

	void setBaseMaxStamina(uint32_t baseMaxStamina);

	uint32_t getBaseMaxFP();

	void setBaseMaxFP(uint32_t baseMaxFp);
private:
	uintptr_t address;
};

}