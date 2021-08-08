#pragma once
#include "pch.h"
#include "chr_ins.h"
#include "ds3runtime.h"

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

uintptr_t ChrIns::getSprjChrDataModule()
{
	return *accessMultilevelPointer<uintptr_t>(address + 0x1F90, 0x18);
}

uintptr_t ChrIns::getSprjChrDamageModule()
{
	return *accessMultilevelPointer<uintptr_t>(address + 0x1F90, 0x98);
}

int32_t ChrIns::isDead()
{
	return *accessMultilevelPointer<int32_t>(address + 0x50, 0x48, 0x11C);
}

void ChrIns::setIsDead(int32_t isDead)
{
	*accessMultilevelPointer<int32_t>(address + 0x50, 0x48, 0x11C) = isDead;
}

bool ChrIns::isNoGravity()
{
	return (*accessMultilevelPointer<uint8_t>(address + 0x1A08) & (uint8_t)pow(2, 6)) == pow(2, 6);
}

void ChrIns::setNoGravity(bool value)
{
	uint8_t* newByte = accessMultilevelPointer<uint8_t>(address + 0x1A08);
	if (value) *newByte = *newByte | (uint8_t)pow(2, 6);
	else *newByte = (*newByte & ~(uint8_t)pow(2, 6));
}

bool ChrIns::isDodging()
{
	return (*accessMultilevelPointer<uint8_t>(address + 0x1F90, 0, 0x58) & (uint8_t)pow(2, 1)) == pow(2, 1);
}

uintptr_t ChrIns::getHkbCharacter()
{
	return *accessMultilevelPointer<uintptr_t>(address + 0x1F90, 0x28, 0x10, 0x28, 0xB8);
}

bool ChrIns::hasHkbCharacter()
{
	return accessMultilevelPointer<uintptr_t>(address + 0x1F90, 0x28, 0x10, 0x28, 0xB8);
}

void ChrIns::playAnimation(int32_t animationStringId)
{
	auto hook = ds3runtime_global->accessHook("play_anim_hook");
	int32_t input[6] = { animationStringId, 0, 0 };
	uintptr_t animationHandle = getHkbCharacter();
	spdlog::debug("Animation force played: {}", animationStringId);
	void(*playAnimationInternal)(uintptr_t, int32_t*);
	*(uintptr_t*)&playAnimationInternal = hook != nullptr ? *hook->getOriginal() : 0x140D84870;
	playAnimationInternal(animationHandle, input);
}

void ChrIns::playAnimation(std::wstring animationString)
{
	uintptr_t* animationHandle = accessMultilevelPointer<uintptr_t>(address + 0x1F90, 0x28, 0x10, 0x28); //AnibndResCap
	void(*playAnimationStringInternal)(...);
	*(uintptr_t*)&playAnimationStringInternal = 0x140D84450;
	playAnimationStringInternal(animationHandle, animationString.c_str());
}

int32_t ChrIns::getHkbIdFromString(std::wstring animationString) //Should be with W_
{
	char arr[32] = {};
	int32_t(*function)(...);
	*(uintptr_t*)&function = 0x141049BD0;
	return function(*accessMultilevelPointer<uintptr_t>(address + 0x1F90, 0x28, 0x10, 0x28, 0xA0), ds3runtime_global->utf8_encode(animationString).c_str());
}

int32_t ChrIns::getWeightIndex()
{
	return *accessMultilevelPointer<int32_t>(address + 0x50, 0x2B4);
}

void ChrIns::setWeightIndex(int32_t weightIndex)
{
	*accessMultilevelPointer<int32_t>(address + 0x50, 0x2B4) = weightIndex;
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

uintptr_t ChrIns::getAddress()
{
	return address;
}

bool ChrIns::isChrIns(uintptr_t address)
{
	const uintptr_t* bodyNodeFunction = accessMultilevelPointer<uintptr_t>(address, 0x400);
	return bodyNodeFunction && (*bodyNodeFunction == 0x14087D320 || *bodyNodeFunction == 0x1408AF650) && address > 0x7FF000000000;
}

}