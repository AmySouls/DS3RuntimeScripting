/*
	* DS3RuntimeScripting
	* Contributers: Amir
	* Further notes on how to use this class:
	* 
	* -Setters- Online limitations.
	* Setting several of these values will not automatically relay this information over the network.
	* This will result in desync of these value across online player's games, or your changes being immediatly
	* reverted by the data coming in from these other online players.
	* 
	* If the ChrIns you are modifying is a player, most data fields will be immediatly reverted by incoming data
	* from their online games. If the ChrIns is not a player, the data may or may not be relayed automatically
	* or may or may not be overwritten by incoming data depending on the field, if you are the host of the online 
	* session.
	* 
	* Use this api's network utilites in your scripts to ensure intended behaivor and utilize possible work arounds.
	* 
	* Read the documentation of each method for more information, each setter method should document a basic description
	* of the unpatched network behaivor when modifying said field. 
*/

#pragma once
#include "pch.h"
#include "memory_util.h"

namespace ds3runtime {

class ChrIns
{
public:
	ChrIns(uintptr_t address);

	enum class Handle
	{
		None = 0,
		MainChr = 0x10068000
	};

	enum class ChrType
	{
		HostOfEmbers = 0,
		WhitePhantom = 1,
		DarkSpirit = 2,
		Ghost1 = 3,
		Ghost2 = 4,
		Hollow = 8,
		Ghost3 = 10,
		Ghost4 = 11,
		Dragon = 12,
		Arena = 13
	};

	enum class Team
	{
		None = 0,
		HostOfEmbers = 1,
		Phantom = 2,
		BlackPhantom = 3,
		Hollow = 4,
		Enemy = 6,
		Boss = 7,
		Friend = 8,
		AngryFriend = 9,
		DecoyEnemy = 10,
		BloodChild = 11,
		BattleFriend = 12,
		Dragon = 13,
		DarkSpirit = 16,
		WatchdogOfFarron = 17,
		AldirchFaithful = 18,
		Darkwraiths = 24,
		NPC = 26,
		HostileNPC = 27,
		Arena = 29,
		MadPhantom = 31,
		MadSpirit = 32,
		Crab = 33
	};

	/**
	* Check if this ChrIns and it's base address is valid. 
	* 
	* Peformes several multi-level pointer checks.
	* 
	* @return True if ChrIns is valid.
	*/
	bool isValid() const;

	/**
	* Gets the handle of this ChrIns. 
	* 
	* The handle seems to be an identifier used by things such as bullets to reference entities.
	* 
	* @return Handle of this ChrIns.
	*/
	Handle getHandle() const;

	/**
	* Get's the ChrInsType of this ChrIns. 
	* 
	* ChrInsType controls apperance and roles of entities.
	* 
	* @return ChrInsType of this ChrIns.
	*/
	ChrType getChrType() const;

	/**
	* Set's the ChrInsType of this ChrIns. 
	* 
	* ChrInsType controls apperance and roles of entities.
	* 
	* Online limitations: For your own player character, changes to this value relay over the network 
	* and sets a corrisponding value to your player's Team on other player's games(Eg. Setting "DarkSpirit" 
	* results in "DarkSpirit" for team type). Changing ChrInsType of any other ChrIns seems to desync.
	* 
	* @param ChrInsType ChrInsType to assign this ChrIns.
	*/
	void setChrType(const ChrType& chrType);

	/**
	* Get's the Team of this ChrIns. 
	* 
	* Team controls combat alliances of entities.
	* 
	* @return Team of this ChrIns.
	*/
	Team getTeam() const;

	/**
	* Set's the Team of this ChrIns. 
	* 
	* Team controls combat alliances of entities.
	* 
	* Online limitations: This change is not relayed over the network aside from setting what you can hit, 
	* but a work around can be peformed by setting your own ChrInsType to a corrisponding value that would 
	* set your Team type to the desired one.
	* 
	* @param team Team to assign to this ChrIns.
	*/
	void setTeam(const Team& team);

	/**
	* Get's the forward id of this ChrIns. 
	* 
	* The forward id seems to be used as a identifier to communicate ChrIns related events
	* across the network. For example to communicate who is riposting and who is being riposted
	* in a riposte grab.
	* 
	* @return Forward id of this ChrIns.
	*/
	unsigned short getForwardId() const;

	/**
	* Get's the character string of this ChrIns 
	* 
	* The character string contains the character id of an ChrIns which is the same as the 
	* actual file that holds the model, animations and behaivors of this character(Eg. c0000 is the player character). 
	* 
	* After the underscore is the character number, which is used to identify which
	* instance of this character this is on the current map.
	* 
	* @return Character string of this ChrIns.
	*/
	std::wstring getCharacterString() const;

	/**
	* Get's the current animation string of this ChrIns. 
	* 
	* Animation strings are the strings defined in hks files which give descriptions of an animation's moveset 
	* role. For example, if a player ChrIns does a light attack while two handing a weapon, this value should be 
	* "AttackBothLight1".
	* 
	* @return Current animation string of this ChrIns.
	*/
	std::wstring getAnimationString() const;

	/**
	* Get's the time elapsed since the current animation of this ChrIns started.
	* 
	* @return The time elapsed since the current animation of this ChrIns started.
	*/
	float getAnimationTime() const;

	/**
	* Get's the maximum time this animation can last.
	* 
	* @return Maximum time this animation can last.
	*/
	float getMaxAnimationTime() const;

	/**
	* Get's the current turn rate of this animation. 
	* 
	* The turn rate is a float value that measures turn rate in degrees per second. 
	* -1 means turn rate doesn't apply, which means the turn rate is infinite or a flag is set so the ChrIns cannot turn. 
	* 
	* @return Current turn rate of this animation.
	*/
	float getTurnRate() const;

	/**
	* Get's the current X-Y-Z position of this ChrIns. 
	* 
	* This is the absolute root position of this ChrIns in the game's virtual 3d space. 
	* 
	* The returned vector contains a copy of the coordinates, so changing these will not 
	* have any effect unless you write this vector back using setPosition().
	* 
	* @return A vector containing the current X-Y-Z position of this ChrIns.
	*/
	std::vector<float> getPosition() const;

	/**
	* Set's the current X-Y-Z position of this ChrIns.
	*
	* This is the absolute root position of this ChrIns in the game's virtual 3d space.
	*
	* Online limitations:
	* { 
	* Self = RELAY, 
	* Players = DESYNC, 
	* NPC = DESYNC SOMETIMES
	* } 
	*
	* SOMETIMES: Relay only if the NPCS's position and actions are currently naturally being set by your own game.
	*
	* A work around can be some-what achieved with network utilities.
	*
	* @param position A 3-value vector containing the X-Y-Z position to move this ChrIns to.
	*/
	void setPosition(const std::vector<float>& position);

	/**
	* Get's the current angle of this ChrIns. 
	* 
	* This is the absolute root rotation of this ChrIns in the game's virtual 3d space.
	* 
	* @return Angle this ChrIns is currently facing.
	*/
	float getAngle() const;

	/**
	* Get's the crossbow vertical aim angle.
	* 
	* @return Vertical crossbow angle this ChrIns is currently facing.
	*/
	float getCrossbowAngle() const;

	/**
	* Get's the current angle of this ChrIns. 
	* 
	* This is the absolute root rotation of this ChrIns in the game's virtual 3d space. 
	* 
	* Online limitations: 
	* { 
	* Self = RELAY, 
	* Players = DESYNC, 
	* NPC = DESYNC SOMETIMES
	* } his Chr's
	* 
	* SOMETIMES: Relay only if the NPCS's position and actions are currently naturally being set by your own game. 
	* 
	* A work around can be some-what achieved with network utilities.
	* 
	* @param angle The angle you want this ChrIns to face.
	*/
	void setAngle(const float& angle);

	/**
	* Gets the virtual address of this Chr's SprjChrDataModule object.
	* 
	* @return Virtual address of this Chr's SprjChrDataModule object.
	*/
	uintptr_t getSprjChrDataModule();

	/**
	* Gets the virtual address of this Chr's SprjChrDamageModule object.
	* 
	* @return Virtual address of this Chr's SprjChrDamageModule object.
	*/
	uintptr_t getSprjChrDamageModule();

	/**
	* Get's this ChrIns's death state. 
	* 
	* Online limitations: 
	* { 
	* Self = RELAY, 
	* Players = DESYNC, 
	* NPC = UNDEFINED
	* } 
	*/
	int32_t isDead() const;

	/**
	* Get's this ChrIns's death state. 
	* 
	* Online limitations: 
	* { 
	* Self = RELAY, 
	* Players = DESYNC, 
	* NPC = UNDEFINED
	* } 
	* 
	* @param isDead If this value is set to less or equal to 0, this character is dead.
	*/
	void setIsDead(const int32_t& isDead);

	/**
	* Checks if the "No Gravity" debug flag is set for this Chr. This flag prevents falling of all kinds.
	* 
	* @return True if the "No Gravity" debug flag is set for this Chr.
	*/
	bool isNoGravity() const;
	
	/**
	* Sets the "No Gravity" debug flag is set for this Chr. This flag prevents consumption of FP on preforming weapon arts and magic.
	*
	* @return True if the "No Gravity" debug flag is set for this Chr.
	*/
	void setNoGravity(const bool& value);

	/**
	* Checks if the "Dodging" flag is set for this Chr. While this is active a chr cannot be hit by most attacks.
	* 
	* @return True if the "Dodging" flag is set for this Chr.
	*/
	bool isDodging() const;

	/**
	* Gets the virtual address of this Chr's hkbCharacter object.
	* 
	* @return The virtual address of this Chr's hkbCharacter object.
	*/
	uintptr_t getHkbCharacter();

	/**
	* Checks if this chr has a loaded hkbCharater object.
	* 
	* @return True if this chr has a loaded hkbCharacter object.
	*/
	bool hasHkbCharacter() const;

	/**
	* Play's an animation on this ChrIns via ingame function it's hkbCharacter class.
	* 
	* Online limitations: 
	* { 
	* Self = RELAY, 
	* Players = DESYNC, 
	* NPC = DESYNC SOMETIMES 
	* } 
	* 
	* @param animationStringId The integer version of the animation string id to play.
	*/
	void playAnimation(const int32_t& animationStringId);

	/*
	* Play's an animation on this ChrIns via calling the function for it's AnibndResCap class.
	* 
	* Online limitations: 
	* { 
	* Self = RELAY, 
	* Players = DESYNC, 
	* NPC = DESYNC SOMETIMES
	* } 
	* 
	* SPECIAL: Relay only if the NPCS's position and actions are currently naturally being set by your own game. 
	* 
	* @param animationString The animation string id to play.
	*/
	void playAnimation(const std::wstring& animationString);

	/**
	* Gets the integer id which matches a specified Havok Behaivor String.
	* 
	* @param animationString The specified Havok Behaivor String.
	* @return The integer repersentation of the specified Havok Behaivor.
	*/
	int32_t getHkbIdFromString(const std::wstring& animationString) const;
	
	/**
	* Plays a specified idle animation id via writing to a debug pointer. The game reads this pointer and unless a throw animation is underway, 
	* it will override the current animation. 
	* 
	* Online limitations: 
	* { 
	* Self = RELAY, 
	* Players = DESYNC, 
	* NPC = DESYNC SOMETIMES 
	* } 
	* 
	* @param animationId The idle animation to play
	*/
	void playDebugIdle(const int32_t& animationId);

	/**
	* Gets the weight index of this ChrIns, which is a variable used for determing how Player Characters should be set to move among other things.
	* 
	* @return The weight index of this ChrIns;
	*/
	int32_t getWeightIndex() const;
	
	/**
	* Sets the weight index of this ChrIns, which is a variable used for determing how Player Characters should be set to move among other things.
	* 
	* @return The weight index of this ChrIns;
	*/
	void setWeightIndex(const int32_t& weightIndex);

	/**
	* Multiplies the animation speed of this ChrIns's animations. 
	* 
	* Online limitations: 
	* { 
	* Self = DESYNC, 
	* Players = DESYNC, 
	* NPC = DESYNC 
	* } 
	* 
	* @param speedModifer The animation speed modifier to set for this ChrIns. 1.0 is the default.
	*/
	void setDebugAnimSpeed(const float& speedModifier);

	/**
	* Gets a list of dummy poly positions on this ChrIns associated with the specified dummy poly id.
	* 
	* @param dummyPolyId The dummy poly id of the target dummy polys.
	* @param polyCount Max amount of dummy poly positions to return.
	* @return A vector containing the found dummy poly positions, stored in own std::array objects.
	*/
	std::vector<std::array<float, 4>> getDummyPolyPositions(const int32_t& dummyPolyId, const uint32_t& polyCount) const;
	
	/**
	* Gets the virtual address of this ChrIns.
	* 
	* @return The virtual address of this ChrIns.
	*/
	uintptr_t getAddress() const;

	/**
	* Check if an address is the base address of an ChrIns.
	* 
	* Peformes several multi-level pointer checks.
	* 
	* @param address Base address to check.
	* @return True if address is the base address of an ChrIns.
	*/
	static bool isChrIns(const uintptr_t& address);
private:
	uintptr_t address;
};

}
