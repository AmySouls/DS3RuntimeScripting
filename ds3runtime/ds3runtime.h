#pragma once
#include "log.h"
#include "memory_util.h"
#include "entity.h"
#include "player.h"
#include "main_character.h"
#include "async_module.h"
#include "hook.h"

namespace ds3runtime {

class DS3RuntimeScripting
{
public:
	DS3RuntimeScripting();

	void attach();

	void detach();

	void addHook(std::shared_ptr<Hook> hook);

	Log* getLog();

	void createLog(HANDLE consoleHandle);

	void runScript(std::shared_ptr<ScriptModule> script);

	void removeScript(uint64_t uniqueId);

	void executeScripts();

	std::shared_ptr<Hook> accessHook(std::string name);
private:
	std::optional<Log> log;
	std::vector<std::shared_ptr<Hook>> hooks;
	std::vector<std::shared_ptr<ScriptModule>> scripts;
	std::mutex mut;
	std::condition_variable cond;
};

extern std::shared_ptr<DS3RuntimeScripting> ds3runtime_global;

}