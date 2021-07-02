#pragma once
#include "pch.h"
#include "ds3runtime.h"

namespace ds3runtime {

DS3RuntimeScripting::DS3RuntimeScripting()
{
}

void DS3RuntimeScripting::setAsyncMode(bool async)
{
	this->async = async;
}

void DS3RuntimeScripting::attach()
{
	attached = true;

	if (!async) {
		for (auto& hook : hooks) {
			hook->install();
		}
	}
	else {
		asyncModeThreadHandle = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)asyncModeThreadProc, NULL, 0, NULL);
	}
}

void DS3RuntimeScripting::detach()
{
	attached = false;

	if (!async) {
		for (auto& hook : hooks) {
			hook->uninstall();
		}
	}
	else {
		asyncModeThreadHandle = NULL;
	}

	scripts.erase(std::remove_if(scripts.begin(), scripts.end(), [](auto script){
		script->onDetach();
		script->remove();
		return true;
	}), scripts.end());
}

void DS3RuntimeScripting::addHook(std::shared_ptr<Hook> hook)
{
	hooks.push_back(hook);
}

void DS3RuntimeScripting::runScript(std::shared_ptr<ScriptModule> script)
{
	scripts.push_back(script);
	if (script->isAsync()) ((AsyncModule*)script.get())->createThread(script);
}

void DS3RuntimeScripting::removeScript(uint64_t uniqueId)
{
	for (int i = 0; i < scripts.size(); i++) if (uniqueId == scripts[i]->getUniqueId()) {
		scripts[i]->onDetach();
		scripts[i]->remove();
		break;
	}
}

void DS3RuntimeScripting::removeScript(std::string name)
{
	for (int i = 0; i < scripts.size(); i++) if (name == scripts[i]->getName()) {
		scripts[i]->onDetach();
		scripts[i]->remove();
		break;
	}
}

void DS3RuntimeScripting::executeScripts()
{
	scripts.erase(std::remove_if(scripts.begin(), scripts.end(), [](auto script) {
		return script->isRemoved();
	}), scripts.end());

	for (auto script : scripts) if (!script->isAsync()) {
		if (!script->isAttached()) script->tryAttach(script->onAttach());
		else script->execute();
	}
}

std::shared_ptr<ScriptModule> DS3RuntimeScripting::accessScript(uint64_t scriptUniqueId)
{
	std::shared_ptr<ScriptModule> matchingScript;

	for (auto script : scripts) if (script->getUniqueId() == scriptUniqueId)
	{
		matchingScript = script;
		break;
	}

	return matchingScript;
}

std::shared_ptr<ScriptModule> DS3RuntimeScripting::accessScript(std::string name)
{
	std::shared_ptr<ScriptModule> matchingScript;

	for (auto script : scripts) if (script->getName().compare(name) == 0)
	{
		matchingScript = script;
		break;
	}

	return matchingScript;
}

std::shared_ptr<Hook> DS3RuntimeScripting::accessHook(std::string name)
{
	std::shared_ptr<Hook> matchingHook;

	for (auto hook : hooks) if (hook->getName().compare(name) == 0)
	{
		matchingHook = hook;
		break;
	}

	return matchingHook;
}

void DS3RuntimeScripting::setGameThreadId(DWORD threadId)
{
	gameThreadId = threadId;
}

DWORD DS3RuntimeScripting::getGameThreadId()
{
	return gameThreadId;
}

std::string DS3RuntimeScripting::utf8_encode(const std::wstring &wstr)
{
	if (wstr.empty()) return std::string();
	int size_needed = WideCharToMultiByte(CP_UTF8, 0, &wstr[0], (int)wstr.size(), NULL, 0, NULL, NULL);
	std::string strTo(size_needed, 0);
	WideCharToMultiByte(CP_UTF8, 0, &wstr[0], (int)wstr.size(), &strTo[0], size_needed, NULL, NULL);
	return strTo;
}

std::wstring DS3RuntimeScripting::utf8_decode(const std::string &str)
{
	if (str.empty()) return std::wstring();
	int size_needed = MultiByteToWideChar(CP_UTF8, 0, &str[0], (int)str.size(), NULL, 0);
	std::wstring wstrTo(size_needed, 0);
	MultiByteToWideChar(CP_UTF8, 0, &str[0], (int)str.size(), &wstrTo[0], size_needed);
	return wstrTo;
}

void DS3RuntimeScripting::asyncModeThreadProc()
{
	while (ds3runtime_global->asyncModeThreadHandle != NULL) {
		ds3runtime_global->executeScripts();
	}
}

}