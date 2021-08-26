#pragma once
#include "pch.h"
#include "ds3runtime.h"

namespace ds3runtime {

void DS3RuntimeScripting::setAsyncMode(const bool& async)
{
	this->async = async;
}

void DS3RuntimeScripting::attach()
{
	attached = true;

	if (!async) {
		for (auto&& hook : hooks) {
			hook->install();
		}
	}
	else {
		asyncModeThreadHandle = CreateThread(NULL, 0, reinterpret_cast<LPTHREAD_START_ROUTINE>(asyncModeThreadProc), NULL, 0, NULL);
	}
}

bool DS3RuntimeScripting::detach()
{
	attached = false;

	if (!async) {
		for (auto&& hook : hooks) {
			hook->uninstall();
		}
	}
	else {
		asyncModeThreadHandle = NULL;
	}

	while (true) {
		auto itr = std::find_if(scripts.begin(), scripts.end(), [](auto&& script) {
			bool detachResult = true;
			if (script->isAttached()) detachResult = script->onDetach();
			if (detachResult) script->remove();
			else {
				spdlog::warn("Script {} was unable to detach...", script->getName());
				script->setDetaching();
			}

			return detachResult;
			});
		
		if (itr == scripts.end()) break;
		auto moveDestroy = std::move(*itr);
		scripts.erase(itr);
	}

	return true;
}

void DS3RuntimeScripting::addHook(std::unique_ptr<Hook> hook)
{
	hooks.push_back(std::move(hook));
}

void DS3RuntimeScripting::runScript(std::unique_ptr<ScriptModule> script)
{
	if (script->isAsync()) reinterpret_cast<AsyncModule*>(script.get())->createThread(script.get());
	scripts.insert(scripts.begin(), std::move(script));
}

bool DS3RuntimeScripting::removeScript(const uint64_t& uniqueId)
{
	for (int i = 0; i < scripts.size(); i++) if (uniqueId == scripts[i]->getUniqueId()) {
		bool detachResult = true;
		if (scripts[i]->isAttached()) detachResult = scripts[i]->onDetach();
		if (detachResult) scripts[i]->remove();
		else scripts[i]->setDetaching();
		return detachResult;
	}
	
	return false;
}

bool DS3RuntimeScripting::removeScript(const std::string& name)
{
	for (int i = 0; i < scripts.size(); i++) if (name == scripts[i]->getName()) {
		bool detachResult = true;
		if (scripts[i]->isAttached()) detachResult = scripts[i]->onDetach();
		if (detachResult) scripts[i]->remove();
		else scripts[i]->setDetaching();
		return detachResult;
	}
	
	return false;
}

void DS3RuntimeScripting::executeScripts()
{
	while (true) {
		auto itr = std::find_if(scripts.begin(), scripts.end(), [](auto&& script) {
			return script->isRemoved();
			});

		if (itr == scripts.end()) break;
		auto moveDestroy = std::move(*itr);
		scripts.erase(itr);
	}

	for (auto&& script : scripts) if (!script->isAsync()) {
		if (!script->isAttached() && script->onAttach()) script->setAttached();
		else if (script->isDetaching() && script->onDetach()) script->remove();
		else script->execute();
	}
}

ScriptModule* DS3RuntimeScripting::accessScript(const uint64_t& scriptUniqueId)
{
	ScriptModule* matchingScript = nullptr;

	for (auto&& script : scripts) if (script->getUniqueId() == scriptUniqueId)
	{
		matchingScript = script.get();
		break;
	}
	
	return matchingScript;
}

ScriptModule* DS3RuntimeScripting::accessScript(const std::string& name)
{
	ScriptModule* matchingScript = nullptr;

	for (auto&& script : scripts) if (script->getName().compare(name) == 0)
	{
		matchingScript = script.get();
		break;
	}
	
	return matchingScript;
}

Hook* DS3RuntimeScripting::accessHook(const std::string& name)
{
	Hook* matchingHook = nullptr;

	for (auto&& hook : hooks) if (hook->getName().compare(name) == 0)
	{
		matchingHook = hook.get();
		break;
	}
	
	return matchingHook;
}

void DS3RuntimeScripting::setGameThreadId(const DWORD& threadId)
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
	int size_needed = WideCharToMultiByte(CP_UTF8, 0, &wstr[0], static_cast<int>(wstr.size()), NULL, 0, NULL, NULL);
	std::string strTo(size_needed, 0);
	WideCharToMultiByte(CP_UTF8, 0, &wstr[0], static_cast<int>(wstr.size()), &strTo[0], size_needed, NULL, NULL);
	return strTo;
}

std::wstring DS3RuntimeScripting::utf8_decode(const std::string &str)
{
	if (str.empty()) return std::wstring();
	int size_needed = MultiByteToWideChar(CP_UTF8, 0, &str[0], static_cast<int>(str.size()), NULL, 0);
	std::wstring wstrTo(size_needed, 0);
	MultiByteToWideChar(CP_UTF8, 0, &str[0], static_cast<int>(str.size()), &wstrTo[0], size_needed);
	return wstrTo;
}

void DS3RuntimeScripting::asyncModeThreadProc()
{
	while (ds3runtime_global->asyncModeThreadHandle != NULL) {
		ds3runtime_global->executeScripts();
	}
}

}
