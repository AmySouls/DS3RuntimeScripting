/*
	* DS3RuntimeScripting
	* Contributers: Amir
	* Further notes on how to use this class:
	* 
	* -EquipData Setters- Online limitations.
	* Setting equipment values is only local and in online session will be reverted as
	* the PlayerIns's game regularly sends their actual equip data.
	* 
	* Changing your own PlayerIns's equipdata will not be reverted but not change 
	* on other's games unless you send a network packet with the updated data.
	* 
	* The game normally sends the equip data of your inventory items rather than 
	* the data on these pointers. However your game aside from durablity, UI and
	* other inventory specific things, your game renders and uses the data from the
	* item identified in this pointer.
	* 
	* Use this api's network utilites in your scripts to ensure intended behaivor and utilize possible work arounds.
*/

#pragma once
#include "chr_ins.h"

namespace ds3runtime {

class PlayerIns : public ChrIns
{
public:
	enum class OffsetNumber {
		MainCharacter = 0,
		Player1 = 1,
		Player2 = 2,
		Player3 = 3,
		Player4 = 4,
		Player5 = 5
	};

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

	enum class SummonType {
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

	PlayerIns(uintptr_t address);

	uintptr_t* getNetworkPointer();

	uint32_t getNumber();

	std::wstring getName();

	void setName(std::wstring name);

	Covenant getCovenant();

	void setCovenant(Covenant covenant);

	SummonType getSummonType();

	void setSummonType(SummonType summonType);

	int32_t getLeftHandWeapon(uint32_t slotNumber);

	void setLeftHandWeapon(uint32_t slotNumber, int32_t equipParamWeaponId);

	int32_t getRightHandWeapon(uint32_t slotNumber);

	void setRightHandWeapon(uint32_t slotNumber, int32_t equipParamWeaponId);

	int32_t getHead();

	void setHead(int32_t equipParamProtectorId);

	int32_t getChest();

	void setChest(int32_t equipParamProtectorId);

	int32_t getHands();

	void setHands(int32_t equipParamProtectorId);

	int32_t getLegs();

	/*
	* Sets the item in the legs slot of this PlayerIns in the game's memory. 
	* 
	* Setting this value may not result in expected behaivor, to learn more read this header's documentation 
	* at the top of the file.
	* 
	* @param offsetNumber The PlayerIns number to get the address of
	* @return The base address of the specified PlayerIns if they exist in memory, otherwise this returns 0.
	*/
	void setLegs(int32_t equipParamProtectorId);

	int32_t getRing(uint32_t slotNumber);

	void setRing(uint32_t slotNumber, int32_t equipParamAccessoryId);

	/*
	* Gets a handle to this player's network handle on the game's peer-to-peer network.
	* 
	* @return handle to this player's network handle on the game's peer-to-peer network.
	*/
	uintptr_t* getNetworkHandle();

	bool isValid();

	/*
	* Get the base address of PlayerIns by the offset multiplier in memory. 
	* 
	* 0 is the main character you control in the game, 1-5 are the PlayerInss 
	* that may appear in your game via online session.
	* 
	* @param offsetNumber The PlayerIns number to get the address of
	* @return The base address of the specified PlayerIns if they exist in memory, otherwise this returns 0.
	*/
	static uintptr_t getAddressByOffsetNumber(OffsetNumber offsetNumber);

	/*
	* Check if an address is the base address of a PlayerIns. 
	* 
	* Compares this address with the base addresses of PlayerInss in memory, 
	* and calls isEntity to verify that the PlayerIns is also a valid entity
	* 
	* @param address Base address to check.
	* @return True if address is the base address of a PlayerIns.
	*/
	static bool isPlayer(uintptr_t address);

	static uintptr_t getMainChrAddress();

	static bool isMainChr(uintptr_t address);

	static bool isMainChrLoaded();
private:
};

}