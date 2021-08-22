/*
	* DS3RuntimeScripting
	* Contributers: Amir
*/

#pragma once
#include "pch.h"
#include "equip_game_data.h"

namespace ds3runtime {

struct Attributes
{
	int32_t vigor;
	int32_t attunement;
	int32_t endurance;
	int32_t strength;
	int32_t dexterity;
	int32_t intelligence;
	int32_t faith;
	int32_t luck;
	int32_t unknown1;
	int32_t humanity;
	int32_t vitality;
	int32_t soulLevel;
};

class PlayerGameData
{
public:
	enum class Covenant {
		None = 0,
		BladeOfTheDarkmoon = 1,
		WarriorsOfSunlight = 2,
		MoundMakers = 3,
		SpearsOfTheChurch = 4,
		FingersOfRosaria = 5,
		WatchdogsOfFarron = 6,
		AldrichFaithfuls = 7,
		WayOfBlue = 8,
		BlueSentinels = 9
	};

	enum class InvadeType {
		Host = 0,
		SummonWhite = 1,
		SummonRed = 2,
		InvadeRed = 3,
		SummonPurpleWhite = 4,
		SpearsOfTheChurch = 6,
		CovenantBladeOfTheDarkmoon = 7,
		UnusedGuardianOfRosaria = 8,
		CovenantWatchdogsOfFarron = 9,
		CovenantAldrichFaithfuls = 10,
		UnusedAvatar = 11,
		ArenaBrawl = 12,
		UnusedUmbasaWhite = 13,
		SummonSunlightWhite = 14,
		SummonSunlightDark = 15,
		SummonPurpleDark = 16,
		InvadeSunlightDark = 17,
		InvadePurpleDark = 18,
		UnusedForceJoinSession = 19,
		UnusedRedHunter = 20,
		BlueSentinels = 21
	};

	enum class Voice : uint8_t {
		Young = 0,
		Mature = 1,
		Aged = 2,
		None = 174
	};

	enum class Class : uint8_t {
		Knight = 0,
		Mercenary = 1,
		Warrior = 2,
		Herald = 3,
		Thief = 4,
		Assasin = 5,
		Sorcerer = 6,
		Pyromancer = 7,
		Cleric = 8,
		Deprived = 9
	};

	enum class Gender : uint8_t {
		Female = 0,
		Male = 1
	};

	enum class Age : uint8_t {
		Young = 0,
		Mature = 1,
		Aged = 2,
		Gael = 3
	};

	PlayerGameData(uintptr_t address);

	/**
	* Gets this players numeber, which is used to identify unique players in online sessions.
	* 
	* @return The player number.
	*/
	int32_t getPlayerNo() const;

	/**
	* Gets the voice type of his player. 
	* 
	* @return The player's voice type.
	*/
	Voice getVoice() const;

	/**
	* Sets the voice type of his player. 
	* 
	* @param voice The voice type to give to the player.
	*/
	void setVoice(const Voice& voice);

	/**
	* Gets the starting class of this player. 
	* 
	* @return The starting class of this player.
	*/
	Class getClass() const;

	/**
	* Sets the starting class of this player. 
	* 
	* @param chrClass The starting class to set for this player.
	*/
	void setClass(const Class& chrClass);

	/**
	* Gets the gender of this player. 
	* 
	* @return This player's gender.
	*/
	Gender getGender() const;

	/**
	* Sets the gender of this player. 
	* 
	* @param gender The gender to set for this player.
	*/
	void setGender(const Gender& gender);

	/**
	* Gets the age type of this player. 
	* 
	* @return The age type of this player.
	*/
	Age getAge() const;

	/**
	* Sets the age type of this player. 
	* 
	* @param age The age type to set for this player.
	*/
	void setAge(const Age& age);

	/**
	* Gets the attributes(also known as stats) of this player. 
	* 
	* @return The attributes of this player.
	*/
	Attributes getAttributes() const;

	/**
	* Sets the attributes(also known as stats) of this player. 
	* 
	* @param attributes The attributes to set for this player.
	*/
	void setAttributes(const Attributes& attributes);

	/**
	* Gets the name of this player. 
	* 
	* @return This player's name.
	*/
	std::wstring getName() const;

	/**
	* Sets the name of this player. 
	* 
	* @param name The name to set for this player.
	*/
	void setName(const std::wstring& name);

	Covenant getCovenant() const;

	void setCovenant(const Covenant& covenant);

	InvadeType getInvadeType() const;

	void setInvadeType(const InvadeType& summonType);

	/**
	* Gets the virtual address of this player's EquipGameData.
	* 
	* @return The virtual address of this player's EquipGameData.
	*/
	uintptr_t getEquipGameData();

	uint16_t getWeaponSheathData() const;

	int32_t getRightHandSlot() const;

	void setRightHandSlot(const int32_t& slot);

	int32_t getLeftHandSlot() const;

	void setLeftHandSlot(const int32_t& slot);

	int32_t getWeaponSheathState() const;

	void setWeaponSheathState(const int32_t& slot);
private:
	uintptr_t address;
};

}
