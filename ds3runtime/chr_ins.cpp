#pragma once
#include "pch.h"
#include "chr_ins.h"

namespace ds3runtime {

ChrIns::ChrIns(uintptr_t address)
{
	this->address = address;
}

bool ChrIns::isValid()
{
	const uintptr_t* bodyNodeFunction = accessMultilevelPointer<uintptr_t>(address, 0x400);
	return bodyNodeFunction && (*bodyNodeFunction == 0x14087D320 || *bodyNodeFunction == 0x1408AF650) && address > 0x7FF000000000;
}

ChrIns::Handle ChrIns::getHandle()
{
	return (Handle)*accessMultilevelPointer<uint32_t>(address + 8);
}

ChrIns::ChrType ChrIns::getChrType()
{
	return (ChrType)*accessMultilevelPointer<uint32_t>(address + 0x70);
}

void ChrIns::setChrType(ChrType chrType) {
	*accessMultilevelPointer<uint32_t>(address + 0x70) = (uint32_t)chrType;
}

ChrIns::Team ChrIns::getTeam()
{
	return (Team)*accessMultilevelPointer<uint32_t>(address + 0x74);
}

void ChrIns::setTeam(Team team) {
	*accessMultilevelPointer<uint32_t>(address + 0x74) = (uint32_t)team;
}

unsigned short ChrIns::getForwardId()
{
	return *accessMultilevelPointer<unsigned short>(address + 0x78);
}

std::wstring ChrIns::getCharacterString()
{
	return std::wstring(accessMultilevelPointer<wchar_t>(address + 0x1F90, 0x18, 0x130));
}

std::wstring ChrIns::getAnimationString() {
	return std::wstring(accessMultilevelPointer<wchar_t>(address + 0x1F90, 0x28, 0x898));
}

float ChrIns::getAnimationTime() {
	return *accessMultilevelPointer<float>(address + 0x1F90, 0x10, 0x24);
}

float ChrIns::getMaxAnimationTime() {
	return *accessMultilevelPointer<float>(address + 0x1F90, 0x10, 0x2C);
}

float ChrIns::getTurnRate() {
	return *accessMultilevelPointer<float>(address + 0x1F90, 0x0, 0x1A0);
}

std::vector<float> ChrIns::getPosition()
{
	float* positionArray = accessMultilevelPointer<float>(address + 0x18, 0x28, 0x80);
	std::vector<float> position(positionArray, positionArray + 12);
	return position;
}

void ChrIns::setPosition(std::vector<float> position)
{
	memcpy(accessMultilevelPointer<float>(address + 0x18, 0x28, 0x80), &position[0], 12);
}

float ChrIns::getAngle()
{
	return *accessMultilevelPointer<float>(address + 0x18, 0x28, 0x74);
}

void ChrIns::setAngle(float angle)
{
	*accessMultilevelPointer<float>(address + 0x18, 0x28, 0x74) = angle;
}

uint32_t ChrIns::getHealth()
{
	return *accessMultilevelPointer<uint32_t>(address + 0x1F90, 0x18, 0xD8);
}

void ChrIns::setHealth(uint32_t health)
{
	*accessMultilevelPointer<uint32_t>(address + 0x1F90, 0x18, 0xD8) = health;
}

uint32_t ChrIns::getMaxHealth()
{
	return *accessMultilevelPointer<uint32_t>(address + 0x1F90, 0x18, 0xDC);
}

void ChrIns::setMaxHealth(uint32_t maxHealth)
{
	*accessMultilevelPointer<uint32_t>(address + 0x1F90, 0x18, 0xD8) = maxHealth;
}

uint32_t ChrIns::getBaseMaxHealth()
{
	return *accessMultilevelPointer<uint32_t>(address + 0x1F90, 0x18, 0xE0);
}

void ChrIns::setBaseMaxHealth(uint32_t baseMaxHealth)
{
	*accessMultilevelPointer<uint32_t>(address + 0x1F90, 0x18, 0xE0) = baseMaxHealth;
}

void ChrIns::playAnimation(int32_t animationStringId)
{
	int32_t input[3] = { animationStringId, 0, 0 };
	void(*playAnimationInternal)(...);
	*(uintptr_t*)&playAnimationInternal = 0x140d84870;
	playAnimationInternal(address, input);
}

void ChrIns::playAnimation(std::wstring animationString)
{
	void(*playAnimationStringInternal)(...);
	*(uintptr_t*)&playAnimationStringInternal = 0x140D84450;
	playAnimationStringInternal(address, animationString.c_str());
}

void ChrIns::playDebugIdle(int32_t animationId)
{
	*accessMultilevelPointer<int32_t>(address + 0x1F90, 0x58, 0x20) = animationId;
}

void ChrIns::setDebugAnimSpeed(float speedModifier)
{
	*accessMultilevelPointer<float>(address + 0x1F90, 0x28, 0xA58) = speedModifier;
}

std::vector<std::vector<float>> ChrIns::getDummyPolyPositions(int32_t dummyPolyId, uint32_t polyCount) {
	std::vector<float> resultBuffer(16 * polyCount);
	std::vector<int32_t> inputBuffer(4);
	inputBuffer[0] = dummyPolyId;
	void(*getBodyNodePositionInternal)(...);
	*(uintptr_t*)&getBodyNodePositionInternal = *accessMultilevelPointer<uintptr_t>(address, 0x400);
	getBodyNodePositionInternal(address, &resultBuffer[0], &inputBuffer[0], polyCount);
	std::vector<std::vector<float>> returnVector(polyCount);

	for (uint32_t i = 0; i < polyCount; i++) {
		std::vector<float> position(4);
		position[0] = resultBuffer[3 + i * 12];
		position[1] = resultBuffer[7 + i * 12];
		position[2] = resultBuffer[11 + i * 12];
		returnVector[i] = position;
	}

	return returnVector;
}

bool ChrIns::isChrIns(uintptr_t address)
{
	const uintptr_t* bodyNodeFunction = accessMultilevelPointer<uintptr_t>(address, 0x400);
	return bodyNodeFunction && (*bodyNodeFunction == 0x14087D320 || *bodyNodeFunction == 0x1408AF650) && address > 0x7FF000000000;
}

}