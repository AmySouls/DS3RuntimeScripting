/*
	* DS3RuntimeScripting
	* Contributers: Amir
*/

#pragma once
#include "pch.h"

namespace ds3runtime {

class SprjChrDataModule
{
public:
	SprjChrDataModule(uintptr_t address);

	/**
	* Gets the current amount of health points this Chr holds.
	* 
	* Health controls several events on the player and other entities, almost universally reaching zero starts a 
	  death sequence. Some trigger the death event at one, some at several hundreds above it.
	* 
	* @return Current amount of health points this Chr holds.
	*/
	uint32_t getHealth() const;

	/**
	* Sets the current amount of health points this Chr holds. 
	* 
	* Health controls several events on the player and other entities, almost universally reaching zero starts a 
	  death sequence. Some trigger the death event at one, some at several hundreds above it. 
	* 
	* Online limitations: 
	* { 
	* Self = RELAY, 
	* Players = DESYNC, 
	* NPC = DESYNC SOMETIMES
	* } 
	* 
	* Sometimes: NPC's healh are primarly controlled by the host of an online session, so editing the memory alone 
	  won't relay over the network if your game isn't the online host.
	  A work around can be achieved with network utilities.
	* 
	* @param health Current amount of health points this Chr should hold.
	*/
	void setHealth(const uint32_t& health);

	/**
	* Gets the current amount of stamina points this Chr holds.
	* 
	* Stamina points are required for player characters to preform certain actions, and lots of actions(Eg. running, attacking)
	  will prevent regeneration of stamina or consume stamina.
	* 
	* @return Current amount of stamina points this Chr holds.
	*/
	uint32_t getStamina() const;

	/**
	* Sets the current amount of stamina points this Chr holds. 
	* 
	* Stamina points are required for player characters to preform certain actions, and lots of actions(Eg. running, attacking)
	  will prevent regeneration of stamina or consume stamina.
	* 
	* Online limitations: 
	* { 
	* Self = RELAY, 
	* Players = DESYNC, 
	* NPC = DESYNC SOMETIMES
	* } 
	* 
	* Sometimes: NPC's stamina are primarly controlled by the host of an online session, so editing the memory alone 
	  won't relay over the network if your game isn't the online host.
	  A work around can be achieved with network utilities.
	* 
	* @param stamina Current amount of stamina points this Chr should hold.
	*/
	void setStamina(const uint32_t& stamina);

	/**
	* Gets the current amount of "FP" or focus points this Chr holds.
	* 
	* Focus points are required for player characters to use certain weapon arts effectively and to cast magic.
	* 
	* @return Current amount of FP this Chr holds.
	*/
	uint32_t getFP() const;

	/**
	* Sets the current amount of "FP" or focus points this Chr holds.
	* 
	* Focus points are required for player characters to use certain weapon arts effectively and to cast magic.
	*
	* Online limitations: 
	* { 
	* Self = RELAY, 
	* Players = DESYNC, 
	* NPC = DESYNC SOMETIMES
	* } 
	* 
	* Sometimes: NPC's FP is primarly controlled by the host of an online session, so editing the memory alone 
	  won't relay over the network if your game isn't the online host.
	  A work around can be achieved with network utilities.
	* 
	* @param fp Current amount of FP this Chr should hold.
	*/
	void setFP(const uint32_t& fp);

	/**
	* Gets the maximum amount of health points this Chr can hold.
	* 
	* Health controls several events on the player and other entities, almost universally reaching zero starts a 
	  death sequence. Some trigger the death event at one, some at several hundreds above it.
	*
	* @return Maximum amount of health points this Chr can hold.
	*/
	uint32_t getMaxHealth() const;

	/**
	* Sets the maximum amount of health points this Chr can hold.
	* 
	* Health controls several events on the player and other entities, almost universally reaching zero starts a 
	  death sequence. Some trigger the death event at one, some at several hundreds above it.
	*
	* This value is constantly being calculated and written to based on base max health and other 
	  variables such as effect modifiers. Either use effects, adjust base max health, or use 
	  patch utilities to adjust this value to what you intend it to be.
	* 
	* Online limitations: 
	* { 
	* Self = RELAY, 
	* Players = DESYNC, 
	* NPC = DESYNC SOMETIMES
	* } 
	* 
	* Sometimes: NPC's healh are primarly controlled by the host of an online session, so editing the memory alone 
	  won't relay over the network if your game isn't the online host.
	  A work around can be achieved with network utilities.
	* 
	* @param maxHealth Maximum amount of health points this Chr should hold.
	*/
	void setMaxHealth(const uint32_t& maxHealth);

	/**
	* Gets the max amount of stamina points this Chr can hold.
	* 
	* Stamina points are required for player characters to preform certain actions, and lots of actions(Eg. running, attacking)
	  will prevent regeneration of stamina or consume stamina.
	* 
	* @return Max amount of stamina points this Chr can hold.
	*/
	uint32_t getMaxStamina() const;

	/**
	* Sets the max amount of stamina points this Chr can hold.
	* 
	* Stamina points are required for player characters to preform certain actions, and lots of actions(Eg. running, attacking)
	  will prevent regeneration of stamina or consume stamina.
	*
	* This value is constantly being calculated and written to based on base max stamina and other 
	  variables such as effect modifiers. Either use effects, adjust base max stamina, or use 
	  patch utilities to adjust this value to what you intend it to be. 
	*
	* Online limitations: 
	* { 
	* Self = RELAY, 
	* Players = DESYNC, 
	* NPC = DESYNC SOMETIMES
	* } 
	* 
	* Sometimes: NPC's stamina is primarly controlled by the host of an online session, so editing the memory alone 
	  won't relay over the network if your game isn't the online host.
	  A work around can be achieved with network utilities.
	* 
	* @param maxStamina Max amount of stamina this Chr should hold.
	*/
	void setMaxStamina(const uint32_t& maxStamina);

	/**
	* Gets the max amount of "FP" or focus points this Chr can hold.
	* 
	* Focus points are required for player characters to use certain weapon arts effectively and to cast magic.
	* 
	* @return Max amount of FP this Chr can hold.
	*/
	uint32_t getMaxFP() const;

	/**
	* Sets the max amount of "FP" or focus points this Chr can hold.
	* 
	* Focus points are required for player characters to use certain weapon arts effectively and to cast magic.
	*
	* This value is constantly being calculated and written to based on base fp health and other 
	  variables. Either adjust base fp health or use 
	  patch utilities to adjust this value to what you intend it to be.
	*
	* Online limitations: 
	* { 
	* Self = RELAY, 
	* Players = DESYNC, 
	* NPC = DESYNC SOMETIMES
	* } 
	* 
	* Sometimes: NPC's FP is primarly controlled by the host of an online session, so editing the memory alone 
	  won't relay over the network if your game isn't the online host.
	  A work around can be achieved with network utilities.
	* 
	* @param fp Current amount of FP this Chr should hold.
	*/
	void setMaxFP(const uint32_t& maxFp);

	/**
	* Gets the base maximum amount of health points this Chr can hold. 
	* 
	* Health controls several events on the player and other entities, almost universally reaching zero starts a 
	  death sequence. Some trigger the death event at one, some at several hundreds above it.
	*
	* This is the value evaluated from stats or NpcParam and is the value before being multiplied by effect modifiers. 
	  For your character, It is only evaluated on load or per update to stats(Eg. leveling up or equipping or unequipping 
	  stat level increasing rings). 
	* 
	* @return Base maximum amount of health points this Chr can hold.
	*/
	uint32_t getBaseMaxHealth() const;

	/**
	* Sets the base maximum amount of health points this Chr can hold. 
	* 
	* Health controls several events on the player and other entities, almost universally reaching zero starts a 
	  death sequence. Some trigger the death event at one, some at several hundreds above it.
	*
	* Setting this will adjust the max health, but only after taking this value and multiplying it with any and 
	* all effects that adjust the max health. This is preformed every game fame.
	* 
	* Online limitations: 
	* { 
	* Self = RELAY, 
	* Players = DESYNC, 
	* NPC = DESYNC 
	* } 
	* 
	* @param baseMaxHealth Base max amount of health points to set for this Chr.
	*/
	void setBaseMaxHealth(const uint32_t& baseMaxHealth);

	/**
	* Gets the base maximum amount of stamina points this Chr can hold. 
	*  
	* Stamina points are required for player characters to preform certain actions, and lots of actions(Eg. running, attacking)
	  will prevent regeneration of stamina or consume stamina.
	*
	* This is the value evaluated from stats or NpcParam and is the value before being multiplied by effect modifiers. 
	  For your character, It is only evaluated on load or per update to stats(Eg. leveling up or equipping or unequipping 
	  stat level increasing rings).
	* 
	* @return Base max amount of stamina points of this Chr.
	*/
	uint32_t getBaseMaxStamina() const;

	/**
	* Sets the base max amount of stamina points of this Chr.
	* 
	* Stamina points are required for player characters to preform certain actions, and lots of actions(Eg. running, attacking)
	  will prevent regeneration of stamina or consume stamina.
	*
	* Setting this will adjust the max stamina, but only after taking this value and multiplying it with any and 
	* all effects that adjust the max stamina. This is preformed every game fame.
	*
	* Online limitations: 
	* { 
	* Self = RELAY, 
	* Players = DESYNC, 
	* NPC = DESYNC
	* }
	* 
	* @param baseMaxStamina Base max amount of stamina points to set for this Chr.
	*/
	void setBaseMaxStamina(const uint32_t& baseMaxStamina);

	/**
	* Gets the base max FP of this Chr. 
	*  
	* Focus points are required for player characters to use certain weapon arts effectively and to cast magic.
	*
	* This is the value evaluated from player attributes or NpcParam. For your character, It is only evaluated on load or per update to stats
	  (Including stat changing rings).
	* 
	* @return Base max FP of this Chr.
	*/
	uint32_t getBaseMaxFP() const;

	/**
	* Sets the base max FP for this Chr. 
	*  
	* Focus points are required for player characters to use certain weapon arts effectively and to cast magic.
	*
	* Setting this will adjust the max fp, but only after taking this value and multiplying it with any and 
	* all effects that adjust the max stamina. This is preformed every game fame.
	* 
	* Online limitations: 
	* { 
	* Self = RELAY, 
	* Players = DESYNC, 
	* NPC = DESYNC
	* }
	*
	* @param baseMaxFp Base max FP to set for this Chr.
	*/
	void setBaseMaxFP(const uint32_t& baseMaxFp);

	bool isNoDamage() const;

	void setNoDamage(const bool& value);

	bool isNoDead() const;

	void setNoDead(const bool& value);

	bool isNoStaminaConsumption() const;

	void setNoStaminaConsumption(const bool& value);

	bool isNoFPConsumption() const;

	void setNoFPConsumption(const bool& value);
private:
	uintptr_t address;
};

}
