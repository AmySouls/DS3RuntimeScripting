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
	enum class OffsetNumber
	{
		MainCharacter = 0,
		Player1 = 1,
		Player2 = 2,
		Player3 = 3,
		Player4 = 4,
		Player5 = 5
	};

	PlayerIns(uintptr_t address);

	/**
	* Gets the virtual address of this Player's PlayerGameData object.
	* 
	* @return Virtual address of this Player's PlayerGameData object.
	*/
	uintptr_t getPlayerGameData();

	/**
	* Gets the virtual address of this Player's NetworkPointer object.
	* 
	* @return Virtual address of this Player's NetworkPointer object.
	*/
	uintptr_t* getNetworkPointer();

	/**
	* Gets the item in the specified left weapon slot of this PlayerIns in the game's memory. 
	* 
	* @param slotNumber The weapon slot to get the weapon from, must be a integer ranging 1-3.
	* @return equipParamId of the weapon in this slot.
	*/
	int32_t getLeftHandWeapon(const uint32_t& slotNumber) const;

	/**
	* Sets the item in the specified left hand weapon slot of this PlayerIns in the game's memory.
	*
	* Setting this value may not result in expected behaivor, to learn more read this header's documentation 
 	  at the top of the file.
	* 
	* @param slotNumber The weapon slot to put the weapon in, must be an integer ranging 1-3.
	* @param equipParamWeaponId equipParamId of the weapon to put in this slot.
	*/
	void setLeftHandWeapon(const uint32_t& slotNumber, const int32_t& equipParamWeaponId);

	/**
	* Gets the item in the specified right weapon slot of this PlayerIns in the game's memory. 
	* 
	* @param slotNumber The weapon slot to get the weapon from, must be a integer ranging 1-3.
	* @return equipParamId of the weapon in this slot.
	*/
	int32_t getRightHandWeapon(const uint32_t& slotNumber) const;

	/**
	* Sets the item in the specified right hand weapon slot of this PlayerIns in the game's memory.
	*
	* Setting this value may not result in expected behaivor, to learn more read this header's documentation 
 	  at the top of the file.
	* 
	* @param slotNumber The weapon slot to put the weapon in, must be an integer ranging 1-3.
	* @param equipParamWeaponId equipParamId of the weapon to put in this slot.
	*/
	void setRightHandWeapon(const uint32_t& slotNumber, const int32_t& equipParamWeaponId);

	/**
	* Gets the item in the head slot of this PlayerIns in the game's memory. 
	* 
	* @return equipParamId of the protector(Armor) in the head slot.
	*/
	int32_t getHead() const;

	/**
	* Sets the item in the head slot of this PlayerIns in the game's memory. 
	* 
	* Setting this value may not result in expected behaivor, to learn more read this header's documentation 
 	  at the top of the file.
	* 
	* @param equipParamProtectorId equipParamId of the protector(Armor) to put into the head slot.
	*/
	void setHead(const int32_t& equipParamProtectorId);

	/**
	* Gets the item in the chest slot of this PlayerIns in the game's memory. 
	* 
	* @return equipParamId of the protector(Armor) in the chest slot.
	*/
	int32_t getChest() const;

	/**
	* Sets the item in the chest slot of this PlayerIns in the game's memory. 
	* 
	* Setting this value may not result in expected behaivor, to learn more read this header's documentation 
 	  at the top of the file.
	* 
	* @param equipParamProtectorId equipParamId of the protector(Armor) to put into the chest slot.
	*/
	void setChest(const int32_t& equipParamProtectorId);

	/**
	* Gets the item in the hands slot of this PlayerIns in the game's memory. 
	* 
	* @return equipParamId of the protector(Armor) in the hands slot.
	*/
	int32_t getHands() const;

	/**
	* Sets the item in the hands slot of this PlayerIns in the game's memory. 
	* 
	* Setting this value may not result in expected behaivor, to learn more read this header's documentation 
 	  at the top of the file.
	* 
	* @param equipParamProtectorId equipParamId of the protector(Armor) to put into the hands slot.
	*/
	void setHands(const int32_t& equipParamProtectorId);

	/**
	* Gets the item in the legs slot of this PlayerIns in the game's memory. 
	* 
	* @return equipParamId of the protector(Armor) in the legs slot.
	*/
	int32_t getLegs() const;

	/**
	* Sets the item in the legs slot of this PlayerIns in the game's memory. 
	* 
	* Setting this value may not result in expected behaivor, to learn more read this header's documentation 
 	  at the top of the file.
	* 
	* @param equipParamProtectorId equipParamId of the protector(Armor) to put into the legs slot.
	*/
	void setLegs(const int32_t& equipParamProtectorId);

	/**
	* Gets the item in the specified ring slot of this PlayerIns in the game's memory. 
	* 
	* @param slotNumber The ring slot to get the ring from, must be a integer ranging 1-4.
	* @return equipParamId of the ring in this slot.
	*/
	int32_t getRing(const uint32_t& slotNumber) const;

	/**
	* Sets the item in the specified ring slot of this PlayerIns in the game's memory.
	*
	* Setting this value may not result in expected behaivor, to learn more read this header's documentation 
 	  at the top of the file.
	* 
	* @param slotNumber The ammo slot to put the ring in, must be an integer ranging 1-4.
	* @param equipParamAccessoryId equipParamId of the ring to put in this slot.
	*/
	void setRing(const uint32_t& slotNumber, const int32_t& equipParamAccessoryId);

	/**
	* Gets the item in the specified ammo slot of this PlayerIns in the game's memory. 
	* 
	* @param slotNumber The ammo slot to get the ring from, must be a integer ranging 1-4.
	* @return equipParamId of the ammo in this slot.
	*/
	int32_t getAmmo(const uint32_t& slotNumber) const;

	/**
	* Sets the item in the specified ammo slot of this PlayerIns in the game's memory.
	*
	* Setting this value may not result in expected behaivor, to learn more read this header's documentation 
	at the top of the file.
	* 
	* @param slotNumber The ammo slot to put the ammo in, must be an integer ranging 1-4.
	* @param equipParamWeaponId equipParamId of the ammo to put in this slot.
	*/
	void setAmmo(const uint32_t& slotNumber, const int32_t& equipParamWeaponId);

	/**
	* Gets the covenant item of this PlayerIns in the game's memory. 
	* 
	* @return equipParamId of this player's covenant.
	*/
	int32_t getCovenant() const;

	/**
	* Sets the covenant item for this PlayerIns in the game's memory. 
	* 
	* Setting this value may not result in expected behaivor, to learn more read this header's documentation 
 	  at the top of the file.
	* 
	* @papram equipParamAccessoryId equipParamId of the new covenant to give this player.
	*/
	void setCovenant(const int32_t& equipParamAccessoryId);

	/**
	* Checks if the "No Goods Consume" debug flag is set for this Player. This flag prevents consumption of inventory goods which may allow them to be used 
	  infintely.
	* 
	* @return True if the "No Goods Consume" debug flag is set for this Player.
	*/
	bool isNoGoodsConsume() const;
	
	/**
	* Sets the "No Goods Consume" debug flag on or off for this Player. This flag prevents consumption of inventory goods which may allow them to be used 
	  infintely.
	* 
	* @param value The state to set for the flag.
	*/
	void setNoGoodsConsume(const bool& value);

	/**
	* Gets the virtual address of this Player's PlayerCtrl object.
	* 
	* @return Virtual address of this Player's PlayerCtrl object.
	*/
	uintptr_t getPlayerCtrl();

	/**
	* Gets a pointer to this player's network handle on the game's peer-to-peer network.
	* 
	* @return Pointer to this player's network handle on the game's peer-to-peer network.
	*/
	uintptr_t* getNetworkHandle();

	/**
	* Check if this refers to a valid PlayerIns object.
	* 
	* Peformes several multi-level pointer checks.
	* 
	* @return True if PlayerIns is valid.
	*/
	bool isValid() const;

	/**
	* Get the base address of PlayerIns by the offset multiplier in memory. 
	* 
	* 0 is the main character you control in the game, 1-5 are the PlayerInss 
	* that may appear in your game via online session.
	* 
	* @param offsetNumber The PlayerIns number to get the address of
	* @return The base address of the specified PlayerIns if they exist in memory, otherwise this returns 0.
	*/
	static uintptr_t getAddressByOffsetNumber(const OffsetNumber& offsetNumber);

	static bool isChrWithOffsetNumber(const OffsetNumber& offsetNumber);

	/**
	* Check if an address is the virtual address of a PlayerIns. 
	* 
	* @param address Virtual address to check.
	* @return True if address is the base address of a PlayerIns.
	*/
	static bool isPlayer(const uintptr_t& address);

	/**
	* Get the virtual address of the main chr(The player charater the game takes the perspective of and allows you to control).
	* 
	* @return Virtual address of the main chr.
	*/
	static uintptr_t getMainChrAddress();

	/**
	* Check if an address matches the virtual address of the main chr. 
	* 
	* @param address Virtual address to check.
	* @return True if address is the virtual address of the main chr.
	*/
	static bool isMainChr(const uintptr_t& address);

	/**
	* Check if the main chr is loaded(The player charater the game takes the perspective of and allows you to control).
	* 
	* @return True if the main chr is loaded
	*/
	static bool isMainChrLoaded();
};

}
