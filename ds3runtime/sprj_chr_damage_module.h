/*
	* DS3RuntimeScripting
	* Contributers: Amir
*/

#pragma once
#include "pch.h"
#include "memory_util.h"

namespace ds3runtime {

struct S_ChrDamage {
    float AtkPhys;
    float AtkMagic;
    float AtkFire;
    float AtkLightning;
    float AtkDark;
	int16_t AtkPhysCorrection;
	int16_t AtkMagicCorrection;
	int16_t AtkFireCorrection;
	int16_t AtkLightningCorrection;
	int16_t AtkDarkCorrection;
	uint8_t Unknown1[6];
    int32_t DamageLevel;
    int32_t SpAttribute;
    float AtkStam;
	uint8_t Unknown2[20];
	int32_t AtkId;
	uint8_t Unknown3[52];
	int32_t DisableHit;
    int32_t SpEffect1;
    int32_t SpEffect2;
    int32_t SpEffect3;
    int32_t SpEffect4;
    int32_t SpEffect5;
	uint8_t Unknown4[32];
	float Unknown5;
	float Unknown6;
	float Unknown7;
	float Unknown8;
	int32_t Hit0_DmyPoly1; //??? Could be something else
	int32_t Hit1_DmyPoly1;
	uint8_t Unknown9[8];
    float AtkSuperArmor;
	uint8_t Unknown10[9];
    bool BypassIFrames;
	uint8_t Unknown11[62];
    float PositionX;
    float PositionY;
    float PositionZ;
	uint8_t Unknown12[36];
    int32_t HeadshotFlag;
	uint8_t Unknown13[3];
    int8_t GuardFlag;
	uint8_t Unknown14[16];
	float AtkPhysRate;
	float AtkMagicRate;
	float AtkFireRate;
	float AtkLightningRate;
	float AtkDarkRate;
	uint8_t Unknown15[60];
    int8_t GuardBreakFlag;
	uint8_t Unknown16[4];
	int8_t WasDodging;
	uint8_t Unknown17[1000];
};

class SprjChrDamageModule
{
public:
	SprjChrDamageModule(uintptr_t address);

	void applyAttackFromChr(uintptr_t attackerChr, S_ChrDamage* chrDamageData);
private:
	uintptr_t address;
};

}