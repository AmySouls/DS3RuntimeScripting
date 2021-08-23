/*
	* DS3RuntimeScripting
	* Contributers: Amir
*/

#pragma once
#include "pch.h"

namespace ds3runtime {

class GameDataMan
{
public:
	GameDataMan(uintptr_t address);

	/**
	* Gets the virtual address of the current save game's PlayerGameData object.
	* 
	* @return The virtual address of the current save game's PlayerGameData object.
	*/
	uintptr_t getPlayerGameData();

	/**
	* Gets the number of times the game has been finished on this save game, also known as "New Game Level".
	* 
	* @return The number of times the game has been finished on this save game, also known as "New Game Level".
	*/
	int32_t getClearCount() const;

	/**
	* Sets the number of times the game has been finished on this save game, also known as "New Game Level".
	* 
	* @param clearCount The new clear count to set.
	*/
	void setClearCount(const int32_t& clearCount);

	/**
	* Gets the number of successful co-op sessions the player has had a summoned phantom(Eg. Critical battle won) on this save game.
	* 
	* @return The number of successful co-op sessions the player has had a summoned phantom(Eg. Critical battle won) on this save game.
	*/
	int32_t getRescueWhite() const;

	/**
	* Gets the number of Dark Spirits that have been slain on this save game.
	* 
	* @return The number of Dark Spirits the player has slain on this save game.
	*/
	int32_t getKillBlack() const;

	/**
	* Gets the number of times the player has gotten a "You Died" on this save game.
	* 
	* @return The number of times the player has gotten a "You Died" on this save game.
	*/
	int32_t getDeathNum() const;

	/**
	* Gets the number of milliseconds spent playing on this save game.
	* 
	* @return The number of milliseconds spent playing on this save game.
	*/
	uint32_t getPlayTime() const;

	/**
	* Gets the one instance of GameDataMan if one exists.
	* 
	* @return The one GameDataMan instance.
	*/
	static uintptr_t getInstance();

	/**
	* Checks if the instance of GameDataMan exists.
	* 
	* @return True if GameDataMan exists.
	*/
	static bool hasInstance();
private:
	uintptr_t address;
};

}
