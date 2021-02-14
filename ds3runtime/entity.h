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
	* If the entity you are modifying is a player, most data fields will be immediatly reverted by incoming data
	* from their online games. If the entity is not a player, the data may or may not be relayed automatically
	* or may or may not be overwritten by incoming data depending on the field, if you are the host of the online 
	* session.
	* 
	* Use this api's network utilites in your scripts to ensure intended behaivor and utilize possible work arounds.
	* 
	* Read the documentation of each method for more information, each setter method should document a basic description
	* of the unpatched network behaivor when modifying said field. 
*/

#pragma once

namespace ds3runtime {

class Entity
{
public:
	Entity(uintptr_t address);

	enum class Handle;

	enum class EntityType;

	enum class Team;

	/*
	* Check if this entity and it's base address is valid. 
	* 
	* Peformes several multi-level pointer checks.
	* 
	* @return True if entity is valid.
	*/
	bool isValid();

	/*
	* Get's the handle of this entity. 
	* 
	* The handle seems to be an identifier used by things such as bullets to reference entities.
	* 
	* @return Handle of this entity.
	*/
	Handle getHandle();

	/*
	* Get's the EntityType of this entity. 
	* 
	* EntityType controls apperance and roles of entities.
	* 
	* @return EntityType of this entity.
	*/
	EntityType getEntityType();

	/*
	* Set's the EntityType of this entity. 
	* 
	* EntityType controls apperance and roles of entities.
	* 
	* Online limitations: For your own player character, changes to this value relay over the network 
	* and sets a corrisponding value to your player's Team on other player's games(Eg. Setting "DarkSpirit" 
	* results in "DarkSpirit" for team type). Changing EntityType of any other entity seems to desync.
	* 
	* @param entityType EntityType to assign this entity.
	*/
	void setEntityType(EntityType entityType);

	/*
	* Get's the Team of this entity. 
	* 
	* Team controls combat alliances of entities.
	* 
	* @return Team of this entity.
	*/
	Team getTeam();

	/*
	* Set's the Team of this entity. 
	* 
	* Team controls combat alliances of entities.
	* 
	* Online limitations: This change is not relayed over the network aside from setting what you can hit, 
	* but a work around can be peformed by setting your own EntityType to a corrisponding value that would 
	* set your Team type to the desired one.
	* 
	* @param team Team to assign to this entity.
	*/
	void setTeam(Team team);

	/*
	* Get's the forward id of this entity. 
	* 
	* The forward id seems to be used as a identifier to communicate entity related events
	* across the network. For example to communicate who is riposting and who is being riposted
	* in a riposte grab.
	* 
	* @return Forward id of this entity.
	*/
	unsigned short getForwardId();

	/*
	* Get's the character string of this entity 
	* 
	* The character string contains the character id of an entity which is the same as the 
	* actual file that holds the model, animations and behaivors of this character(Eg. c0000 is the player character). 
	* 
	* After the underscore is the character number, which is used to identify which
	* instance of this character this is on the current map.
	* 
	* @return Character string of this entity.
	*/
	std::wstring getCharacterString();

	/*
	* Get's the current animation string of this entity. 
	* 
	* Animation strings are the strings defined in hks files which give descriptions of an animation's moveset 
	* role. For example, if a player entity does a light attack while two handing a weapon, this value should be 
	* "AttackBothLight1".
	* 
	* @return Current animation string of this entity.
	*/
	std::wstring getAnimationString();

	/*
	* Get's the current X-Y-Z position of this entity. 
	* 
	* This is the absolute root position of this entity in the game's virtual 3d space. 
	* 
	* The returned vector contains a copy of the coordinates, so changing these will not 
	* have any effect unless you write this vector back using setPosition().
	* 
	* @return A vector containing the current X-Y-Z position of this entity.
	*/
	std::vector<float> getPosition();

		/*
		* Set's the current X-Y-Z position of this entity.
		*
		* This is the absolute root position of this entity in the game's virtual 3d space.
		*
		* Online limitations:
		* {
		* Self = RELAY,
		* Players = OVERWRITE,
		* NPC = RELAY IF SPECIAL
		* }
		*
		* SPECIAL: Relay only if the NPCS's position and actions are currently naturally being set by your own game.
		*
		* A work around can be some-what achieved with network utilities.
		*
		* @param position A 3-value vector containing the X-Y-Z position to move this entity to.
		*/
		void setPosition(std::vector<float> position);

	/*
	* Get's the current angle of this entity. 
	* 
	* This is the absolute root rotation of this entity in the game's virtual 3d space.
	* 
	* @return Angle this entity is currently facing.
	*/
	float getAngle();

	/*
	* Get's the current angle of this entity. 
	* 
	* This is the absolute root rotation of this entity in the game's virtual 3d space. 
	* 
	* Online limitations: 
	* { 
	* Self = RELAY, 
	* Players = OVERWRITE, 
	* NPC = RELAY IF SPECIAL 
	* } 
	* 
	* SPECIAL: Relay only if the NPCS's position and actions are currently naturally being set by your own game. 
	* 
	* A work around can be some-what achieved with network utilities.
	* 
	* @param angle The angle you want this entity to face.
	*/
	void setAngle(float angle);

	/*
	* Get's the current amount of health points this entity holds
	* 
	* This controls several events on the player and other entities, almost universally reaching zero starts a
	* death sequence. Some at one, or several hundreds above it.
	* 
	* @return Current amount of health points this entity holds.
	*/
	uint32_t getHealth();

	/*
	* Set's the current amount of health points this entity holds. 
	* 
	* This controls several events on the player and other entities, almost universally reaching zero starts a 
	* death sequence. Some at one, or several hundreds above it. 
	* 
	* Online limitations: 
	* { 
	* Self = RELAY, 
	* Players = OVERWRITE, 
	* NPC = RELAY IF HOST  
	* } 
	* 
	* A work around can be achieved with network utilities.
	* 
	* @param health Current amount of health points this entity should hold.
	*/
	void setHealth(uint32_t health);

	/*
	* Get's the maximum amount of health points this entity can hold.
	* 
	* @return Maximum amount of health points this entity can hold.
	*/
	uint32_t getMaxHealth();

	/*
	* Set's the maximum amount of health points this entity can hold.
	* 
	* This value is constantly being calculated and written to based on base max health and other 
	* variables such as effect modifiers. Either use effects, adjust base max health, or use 
	* patch utilities to adjust this value to what you intend it to be. 
	* 
	* Online limitations: 
	* { 
	* Self = RELAY, 
	* Players = OVERWRITE, 
	* NPC = ??? 
	* } 
	* 
	* @param maxHealth Maximum amount of health points this entity should hold.
	*/
	void setMaxHealth(uint32_t maxHealth);

	/*
	* Get's the base maximum amount of health points this entity can hold. 
	* 
	* This is the value evaluated from stats or an entity's base health parameters and the value before 
	* being multiplied by effect modifiers. For your character, It is only evaluated on load or per update 
	* to stats(Eg. leveling up or equipping or unequipping stat level increasing rings). 
	* 
	* @return Base maximum amount of health points this entity can hold.
	*/
	uint32_t getBaseMaxHealth();

	/*
	* Set's the base maximum amount of health points this entity can hold. 
	* 
	* Setting this will adjust your max health, but only after taking this value and multiplying it with any and 
	* all effects that adjust your max health. 
	* 
	* Online limitations: 
	* { 
	* Self = ???, 
	* Players = ???, 
	* NPC = ??? 
	* } 
	* 
	* @param baseMaxHealth Base maximum amount of health points this entity should hold.
	*/
	void setBaseMaxHealth(uint32_t baseMaxHealth);

	/*
	* Check if an address is the base address of an entity.
	* 
	* Peformes several multi-level pointer checks.
	* 
	* @param address Base address to check.
	* @return True if address is the base address of an entity.
	*/
	static bool isEntity(uintptr_t address);
private:
protected:
	uintptr_t address;
};

}