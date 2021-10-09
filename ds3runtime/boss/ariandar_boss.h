/*
	* DS3RuntimeScripting
	* Contributers: Amir
*/

#pragma once
#include "pch.h"
#include "standard_player_boss.h"

namespace ds3runtime {

class AriandarBoss : public StandardPlayerBoss
{
public:
	bool onAttach();

	bool onDetach();

	std::string getName()
	{
		return "ariandar_boss";
	}
protected:
	void logic();

	void checks();
private:
	std::string chainComboLastMove;

	void updateChainComboMove(const std::string& taskId);
};

}