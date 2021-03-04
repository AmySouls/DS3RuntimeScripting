/*
	* DS3RuntimeScripting
	* Contributers: Amir
	* Further notes on how to use this class:
*/

#pragma once
#include "pch.h"
#include "memory_util.h"
#include "databaseaddress.h"

namespace ds3runtime {

class BulletSpawn
{
public:
	BulletSpawn();

	int32_t getOwner();

	void setOwner(int32_t ownerHandle);

	int32_t getBulletId();

	void setBulletId(int32_t bulletParamId);

	int32_t getTurretFlag();

	void setTurretFlag(int32_t turretFlag);

	int32_t getHomingFlag();

	void setHomingFlag(int32_t homingFlag);

	int32_t getNetworkFlag();

	void setNetworkFlag(int32_t networkFlag);

	std::vector<float> getCoordinates();

	void setCoordinates(std::vector<float> coordinates);

	void setX(float coordinateX);

	void setY(float coordinateY);

	void setZ(float coordinateZ);

	std::vector<float> getDirection();

	void setDirection(std::vector<float> direction);

	void setDirectionX(float directionX);

	void setDirectionY(float directionY);

	void setDirectionZ(float directionZ);

	/*
	  Launches this bullet with it's current parameters.

	  This calls one of the game's internal SprjBulletMan functions to spawn this bullet, any incompatible arguments 
	  or NaN/Non-Finite float arguments will result in undefined behaivor.

	  This call is also not thread safe and will likely crash the game.
 	*/
	void launch();

	static void launchBullet(int32_t ownerHandle,
		int32_t bulletParamId,
		int32_t homingFlag = -1,
		float x = 0,
		float y = 0,
		float z = 0,
		float directionX = 0,
		float directionY = 0,
		float directionZ = 0);

	static void launchBullet(int32_t ownerHandle,
		int32_t bulletParamId,
		int32_t homingFlag,
		std::vector<float> coordinates,
		std::vector<float> direction);
private:
	int32_t ownerHandle = -1;
	int32_t bulletParamId = -1;
	int32_t turretFlag = 0;
	int32_t homingFlag = -1;
	int32_t networkFlag = 9;
	std::vector<float> coordinates;
	std::vector<float> direction;

	static void SprjBulletManSpawnCall(BulletSpawn bulletSpawn);
};

}