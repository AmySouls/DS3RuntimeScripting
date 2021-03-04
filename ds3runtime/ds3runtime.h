#pragma once
#include "pch.h"
#include "memory_util.h"
#include "async_module.h"
#include "hook.h"
#include "player_ins.h"

namespace ds3runtime {

class DS3RuntimeScripting
{
public:
	DS3RuntimeScripting();

	void attach();

	void detach();

	void addHook(std::shared_ptr<Hook> hook);

	void runScript(std::shared_ptr<ScriptModule> script);

	void removeScript(uint64_t uniqueId);

	void executeScripts();

	std::shared_ptr<ScriptModule> accessScript(uint64_t scriptUniqueId);

	std::shared_ptr<ScriptModule> accessScript(std::string name);

	std::shared_ptr<Hook> accessHook(std::string name);
private:
	std::vector<std::shared_ptr<Hook>> hooks;
	std::vector<std::shared_ptr<ScriptModule>> scripts;
	std::mutex mut;
	std::condition_variable cond;
};

extern std::shared_ptr<DS3RuntimeScripting> ds3runtime_global;

}