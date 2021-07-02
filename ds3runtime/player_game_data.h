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
		Age = 2,
		Gael = 3
	};

	PlayerGameData(uintptr_t address);

	int32_t getPlayerNo();

	Voice getVoice();

	void setVoice(Voice voice);

	Class getClass();

	void setClass(Class chrClass);

	Gender getGender();

	void setGender(Gender gender);

	Age getAge();

	void setAge(Age age);

	Attributes getAttributes();

	void setAttributes(Attributes attributes);

	std::wstring getName();

	void setName(std::wstring name);

	Covenant getCovenant();

	void setCovenant(Covenant covenant);

	InvadeType getInvadeType();

	void setInvadeType(InvadeType summonType);

	uintptr_t getEquipGameData();
private:
	uintptr_t address;
};

}