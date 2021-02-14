#pragma once
#include "pch.h"
#include "memory_util.h"
#include "entity.h"

namespace ds3runtime {

Entity::Entity(uintptr_t address)
{
	this->address = address;
}

enum class Entity::Handle {
	None = 0,
	MainCharacter = 0x10068000
};

enum class Entity::EntityType {
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

enum class Entity::Team {
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

bool Entity::isValid()
{
	const uintptr_t* bodyNodeFunction = accessMultilevelPointer<uintptr_t>(address, 0x400);
	return bodyNodeFunction && (*bodyNodeFunction == 0x14087D320 || *bodyNodeFunction == 0x1408AF650) && address > 0x7FF000000000;
}

Entity::Handle Entity::getHandle()
{
	return (Handle)*accessMultilevelPointer<uint32_t>(address + 8);
}

Entity::EntityType Entity::getEntityType()
{
	return (EntityType)*accessMultilevelPointer<uint32_t>(address + 0x70);
}

void Entity::setEntityType(EntityType entityType) {
	*accessMultilevelPointer<uint32_t>(address + 0x70) = (uint32_t)entityType;
}

Entity::Team Entity::getTeam()
{
	return (Team)*accessMultilevelPointer<uint32_t>(address + 0x74);
}

void Entity::setTeam(Team team) {
	*accessMultilevelPointer<uint32_t>(address + 0x74) = (uint32_t)team;
}

unsigned short Entity::getForwardId()
{
	return *accessMultilevelPointer<unsigned short>(address + 0x78);
}

std::wstring Entity::getCharacterString()
{
	return std::wstring(accessMultilevelPointer<wchar_t>(address + 0x1F90, 0x18, 0x130));
}

std::wstring Entity::getAnimationString() {
	return std::wstring(accessMultilevelPointer<wchar_t>(address + 0x1F90, 0x10, 0xB0));
}

std::vector<float> Entity::getPosition()
{
	float* positionArray = accessMultilevelPointer<float>(address + 0x18, 0x28, 0x80);
	std::vector<float> position(positionArray, positionArray + 12);
	return position;
}

void Entity::setPosition(std::vector<float> position)
{
	memcpy(accessMultilevelPointer<float>(address + 0x18, 0x28, 0x80), &position[0], 12);
}

float Entity::getAngle()
{
	return *accessMultilevelPointer<float>(address + 0x18, 0x28, 0x74);
}

void Entity::setAngle(float angle)
{
	*accessMultilevelPointer<float>(address + 0x18, 0x28, 0x74) = angle;
}

uint32_t Entity::getHealth()
{
	return *accessMultilevelPointer<uint32_t>(address + 0x1F90, 0x18, 0xD8);
}

void Entity::setHealth(uint32_t health)
{
	*accessMultilevelPointer<uint32_t>(address + 0x1F90, 0x18, 0xD8) = health;
}

uint32_t Entity::getMaxHealth()
{
	return *accessMultilevelPointer<uint32_t>(address + 0x1F90, 0x18, 0xDC);
}

void Entity::setMaxHealth(uint32_t maxHealth)
{
	*accessMultilevelPointer<uint32_t>(address + 0x1F90, 0x18, 0xD8) = maxHealth;
}

uint32_t Entity::getBaseMaxHealth()
{
	return *accessMultilevelPointer<uint32_t>(address + 0x1F90, 0x18, 0xE0);
}

void Entity::setBaseMaxHealth(uint32_t baseMaxHealth)
{
	*accessMultilevelPointer<uint32_t>(address + 0x1F90, 0x18, 0xE0) = baseMaxHealth;
}

bool Entity::isEntity(uintptr_t address)
{
	const uintptr_t* bodyNodeFunction = accessMultilevelPointer<uintptr_t>(address, 0x400);
	return bodyNodeFunction && (*bodyNodeFunction == 0x14087D320 || *bodyNodeFunction == 0x1408AF650) && address > 0x7FF000000000;
}

}