#pragma once
#include "pch.h"
#include "ds3runtime.h"

namespace ds3runtime {

DS3RuntimeScripting::DS3RuntimeScripting()
{
}

void DS3RuntimeScripting::attach() {
	for (auto& hook : hooks) {
		hook->install();
	}
}

void DS3RuntimeScripting::detach() {
	for (auto& hook : hooks) {
		hook->uninstall();
	}

	std::lock_guard<std::mutex> lock(mut);
	for (auto script : scripts) if (script->isAsync()) ((AsyncModule*)script.get())->destroy();
	scripts.clear();
	cond.notify_one();
}

void DS3RuntimeScripting::addHook(std::shared_ptr<Hook> hook) {
	hooks.push_back(hook);
}

Log* DS3RuntimeScripting::getLog()
{
	if (!log.has_value()) return nullptr;
	return &log.value();
}

void DS3RuntimeScripting::createLog(HANDLE consoleHandle)
{
	log = Log(consoleHandle);
}

void DS3RuntimeScripting::runScript(std::shared_ptr<ScriptModule> script) {
	std::lock_guard<std::mutex> lock(mut);
	scripts.push_back(script);
	if (script->isAsync()) ((AsyncModule*)script.get())->createThread(script);
	cond.notify_one();
}

void DS3RuntimeScripting::removeScript(uint64_t uniqueId) {
	std::lock_guard<std::mutex> lock(mut);

	for (int i = 0; i < scripts.size(); i++) if (uniqueId == scripts[i]->getUniqueId()) {
		scripts.erase(scripts.begin() + i);
		break;
	}

	cond.notify_one();
}

void DS3RuntimeScripting::executeScripts()
{
	std::lock_guard<std::mutex> lock(mut);

	for (auto script : scripts) {
		if (!script->isAsync()) script->execute();
	}

	cond.notify_one();
}

std::shared_ptr<Hook> DS3RuntimeScripting::accessHook(std::string name)
{
	std::shared_ptr<Hook> matchingHook;
	std::lock_guard<std::mutex> lock(mut);

	for (auto hook : hooks) if (hook->getName().compare(name) == 0)
	{
		matchingHook = hook;
		break;
	}

	cond.notify_one();
	return matchingHook;
}

}