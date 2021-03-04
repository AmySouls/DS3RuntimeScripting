#pragma once
#include "pch.h"
#include "sprj_bullet_state.h"

namespace ds3runtime {

SprjBulletState::SprjBulletState(uintptr_t address)
{
}

bool SprjBulletState::isValid()
{
	return true; //Fix!
}

std::vector<float> SprjBulletState::getCoordinates()
{
	std::vector<float> vector(3);
	memcpy(&vector[0], (void*)(address + 0x10), 12);
	return vector;
}

std::vector<float> SprjBulletState::getLaunchVector()
{
	std::vector<float> vector(3);
	for (uintptr_t i = 0; i < 3; i++) memcpy(&vector[i], (void*)(address + 0xAC8 + i * 0x10), 4);
	return vector;
}

float SprjBulletState::getDuration()
{
	return *accessMultilevelPointer<float>(address + 0xC9C);
}

int32_t SprjBulletState::getId()
{
	return *accessMultilevelPointer<int32_t>(address + 0xC88);
}

int32_t SprjBulletState::getOwnerHandle()
{
	int handle = *accessMultilevelPointer<uint32_t>(address + 0x4EC);
	if (handle != -1) return handle;
	handle = *accessMultilevelPointer<uint32_t>(address + 0xB98);
	if (handle != -1) return handle;
	handle = *accessMultilevelPointer<uint32_t>(address + 0x1898);
	return handle;
}

bool SprjBulletState::isBullet(uintptr_t address)
{
	return true; //Fix!
}

}