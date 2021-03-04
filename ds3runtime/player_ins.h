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
	enum class OffsetNumber;

	enum class Covenant;

	PlayerIns(uintptr_t address);

	uintptr_t* getNetworkPointer();

	uint32_t getNumber();

	std::wstring getName();

	void setName(std::wstring name);

	Covenant getCovenant();

	void setCovenant(Covenant covenant);

	uint32_t getLeftHandWeapon(uint32_t slotNumber);

	void setLeftHandWeapon(uint32_t slotNumber, uint32_t equipParamWeaponId);

	uint32_t getRightHandWeapon(uint32_t slotNumber);

	void setRightHandWeapon(uint32_t slotNumber, uint32_t equipParamWeaponId);

	uint32_t getHead();

	void setHead(uint32_t equipParamProtectorId);

	uint32_t getChest();

	void setChest(uint32_t equipParamProtectorId);

	uint32_t getHands();

	void setHands(uint32_t equipParamProtectorId);

	uint32_t getLegs();

	/*
	* Sets the item in the legs slot of this PlayerIns in the game's memory. 
	* 
	* Setting this value may not result in expected behaivor, to learn more read this header's documentation 
	* at the top of the file.
	* 
	* @param offsetNumber The PlayerIns number to get the address of
	* @return The base address of the specified PlayerIns if they exist in memory, otherwise this returns 0.
	*/
	void setLegs(uint32_t equipParamProtectorId);


	/*
	* Gets a handle to this player's network handle on the game's peer-to-peer network.
	* 
	* @return handle to this player's network handle on the game's peer-to-peer network.
	*/
	uintptr_t* getNetworkHandle();

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
private:
};

}