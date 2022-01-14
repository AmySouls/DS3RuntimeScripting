/*
	* DS3RuntimeScripting
	* Contributers: Amir
*/

#pragma once
#include "pch.h"
#include "ds3runtime/script_module.h"
#include "ds3runtime/player_ins.h"

namespace ds3runtime {

class RunFrameTest : public ScriptModule
{
public:
	void execute();

	std::string getName()
	{
		return "run_frame_test";
	}
private:
	float timeSinceLastMove = 0;
	bool isMoving = false;
	std::vector<float> moveStartPosition;
};

}