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

	void setAsyncMode(const bool& async);

	void attach();

	bool detach();

	void addHook(std::unique_ptr<Hook>& hook);

	void runScript(std::unique_ptr<ScriptModule>& script);

	bool removeScript(const uint64_t& uniqueId);

	bool removeScript(const std::string& name);

	void executeScripts();

	ScriptModule* accessScript(const uint64_t& scriptUniqueId);

	ScriptModule* accessScript(const std::string& name);

	Hook* accessHook(const std::string& name);

	void setGameThreadId(const DWORD& gameThreadId);

	DWORD getGameThreadId();

	std::string utf8_encode(const std::wstring& wstr);

	std::wstring utf8_decode(const std::string& str);
private:
	std::vector<std::unique_ptr<Hook>> hooks;
	std::vector<std::unique_ptr<ScriptModule>> scripts;
	std::mutex scriptMutex;
	DWORD gameThreadId = 0;
	bool async = false;
	HANDLE asyncModeThreadHandle;
	std::atomic<bool> attached = false;

	static void asyncModeThreadProc();
};

extern std::shared_ptr<DS3RuntimeScripting> ds3runtime_global;

}
