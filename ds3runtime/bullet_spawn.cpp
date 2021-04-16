/*
	* DS3RuntimeScripting
	* Contributers: Amir
*/

#pragma once
#include "pch.h"
#include "bullet_spawn.h"

namespace ds3runtime {

BulletSpawn::BulletSpawn()
{
	coordinates = direction = std::vector<float>(3);
}

int32_t BulletSpawn::getOwner()
{
	return ownerHandle;
}

void BulletSpawn::setOwner(int32_t ownerHandle)
{
	this->ownerHandle = ownerHandle;
}

int32_t BulletSpawn::getBulletId()
{
	return bulletParamId;
}

void BulletSpawn::setBulletId(int32_t bulletParamId)
{
	this->bulletParamId = bulletParamId;
}

int32_t BulletSpawn::getTurretFlag()
{
	return turretFlag;
}

void BulletSpawn::setTurretFlag(int32_t turretFlag)
{
	this->turretFlag = turretFlag;
}

int32_t BulletSpawn::getHomingFlag()
{
	return homingFlag;
}

void BulletSpawn::setHomingFlag(int32_t homingFlag)
{
	this->homingFlag = homingFlag;
}

int32_t BulletSpawn::getNetworkFlag()
{
	return networkFlag;
}

void BulletSpawn::setNetworkFlag(int32_t networkFlag)
{
	this->networkFlag = networkFlag;
}

std::vector<float> BulletSpawn::getCoordinates()
{
	return coordinates;
}

void BulletSpawn::setCoordinates(std::vector<float> coordinates)
{
	this->coordinates = coordinates;
}

void BulletSpawn::setX(float coordinateX)
{
	coordinates[0] = coordinateX;
}

void BulletSpawn::setY(float coordinateY)
{
	coordinates[1] = coordinateY;
}

void BulletSpawn::setZ(float coordinateZ)
{
	coordinates[2] = coordinateZ;
}

std::vector<float> BulletSpawn::getDirection()
{
	return direction;
}

void BulletSpawn::setDirection(std::vector<float> direction)
{
	this->direction = direction;
}

void BulletSpawn::setDirectionX(float directionX)
{
	direction[0] = directionX;
}

void BulletSpawn::setDirectionY(float directionY)
{
	direction[1] = directionY;
}

void BulletSpawn::setDirectionZ(float directionZ)
{
	direction[2] = directionZ;
}

void BulletSpawn::launch()
{
	SprjBulletManSpawnCall(*this);
}

void BulletSpawn::launchBullet(int32_t ownerHandle,
	int32_t bulletParamId,
	int32_t homingFlag,
	float x,
	float y,
	float z,
	float directionX,
	float directionY,
	float directionZ)
{
	BulletSpawn bulletSpawn = BulletSpawn();
	bulletSpawn.setOwner(ownerHandle);
	bulletSpawn.setBulletId(bulletParamId);
	bulletSpawn.setHomingFlag(homingFlag);
	bulletSpawn.setCoordinates(std::vector<float>({ x, y, z }));
	bulletSpawn.setDirection(std::vector<float>({ directionX, directionY, directionZ }));
	bulletSpawn.launch();
}

void BulletSpawn::launchBullet(int32_t ownerHandle,
	int32_t bulletParamId,
	int32_t homingFlag,
	std::vector<float> coordinates,
	std::vector<float> direction)
{
	BulletSpawn bulletSpawn = BulletSpawn();
	bulletSpawn.setOwner(ownerHandle);
	bulletSpawn.setBulletId(bulletParamId);
	bulletSpawn.setHomingFlag(homingFlag);
	bulletSpawn.setCoordinates(coordinates);
	bulletSpawn.setDirection(direction);
	bulletSpawn.launch();
}
	
void BulletSpawn::SprjBulletManSpawnCall(BulletSpawn bulletSpawn)
{
	unsigned char data[0x700] = {};
	*(int32_t*)(data + 0x30) = bulletSpawn.getOwner();
	*(int32_t*)(data + 0x40) = bulletSpawn.getBulletId();
	*(int32_t*)(data + 0x48) = bulletSpawn.getTurretFlag();
	*(int32_t*)(data + 0x4C) = bulletSpawn.getHomingFlag();
	*(int32_t*)(data + 0x6C) = bulletSpawn.getNetworkFlag();
	const auto coordinates = bulletSpawn.getCoordinates();
	const auto direction = bulletSpawn.getDirection();
	
	for (uintptr_t i = 0; i < 3; i++) {
		*(float*)(data + 0x7C + i * 0x10) = coordinates[i];
		*(float*)(data + 0x78 + i * 0x10) = direction[i];
	}

	void(*SprjBulletManSpawn)(...);
	*(uintptr_t*)&SprjBulletManSpawn = 0x140978500;
	SprjBulletManSpawn(*(uintptr_t*)DataBaseAddress::SprjBulletManager, data, data + 0x30);
}

}