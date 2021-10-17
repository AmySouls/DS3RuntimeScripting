/*
	* DS3RuntimeScripting
	* Contributers: Amir
*/

#pragma once
#include "pch.h"
#include "ds3runtime/ds3runtime.h"
#include "standard_combo_system.h"

namespace ds3runtime::blight {

class BlightComboSystem : public stdcombo::ComboSystem
{
public:
	BlightComboSystem(Boss* boss, std::string recoveryTaskId, int maxComboCharges, float comboChargesRegenRate);

	void install();

	bool isExecutingComboChain();

	float getComboCharges();

	void setComboCharges(const float& comboCharges);

	int getMaxComboCharges();

	void setMaxComboCharges(const int& maxComboCharges);

	float getComboChargeRegenRate();

	void setComboChargeRegenRate(const float& comboChargeRegenRate);
private:
	std::string lastTaskIdInChain;
	std::string recoveryTaskId;
	int maxComboCharges;
	float comboChargeRegenRate;
	float comboCharges;
	bool isInComboChain = false;
};

}