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
	AriandarBoss(std::shared_ptr<PlayerIns> playerIns);

	bool onAttach();

	void onDetach();

	std::string getName()
	{
		return "ariandar_boss";
	}
protected:
	void logic();

	void checks();
private:
	//std::optional<BossTask> currentMoveTask;
	/*
	static BossTask dragonSlayerSpear_2h_running_r1;
	static BossTask phase1_2h_running_r1_followup_r2_charged;
	static BossTask phase1_2h_running_r1_followup_r2_uncharged;
	static BossTask dragonSlayerSpear_2h_kick_r2;
	static std::vector<BossTask*> allBossTasks;
	*/
};

}