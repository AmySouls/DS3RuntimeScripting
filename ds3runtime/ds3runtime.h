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

	/**
	 * Sets if DS3RuntimeScripting should run in async mode, by default it does not.
	 *
	 * @param async True if DS3RuntimeScripting shoud be run in async mode.
	 */
	void setAsyncMode(const bool& async);

	/**
	 * This function preforms all nessessary installation of the added hooks and scripts. It should only be called once on the attachment of the source DLL
	   or once after every successful calling of detach(), calling it outside those guidelines could lead to undefined behavior.
	 */
	void attach();

	/**
	 * This function preforms all nessessary uninstallation and cleanup for installed hooks and running scripts. Should be called repeadly until it returns
	   true to indicate all hooks and scripts hae successfully detached.
	 *
	 * @return True if finished detaching.
	 */
	bool detach();

	/**
	 * Adds a game code hook to DS3RuntimeScripting. Hooks are installed into the game upon calling attach().
	 *
	 * @param hook Unique Hook to install.
	 */
	void addHook(std::unique_ptr<Hook>& hook);

	/**
	 * Adds a script to DS3RuntimeScripting. Scripts are practially a task/timer system. To attach and activate them call attach().
	 *
	 * @param script Script to install.
	 */
	void runScript(std::unique_ptr<ScriptModule>& script);

	/**
	 * Removes a script based off this particular script instance's unique id. Returns false if the script wasn't able to immediatly detach.
	 *
	 * @param uniqueId Unique id of the script to remove.
	 * @return False if the script wasn't able to immeditaly detach.
	 */
	bool removeScript(const uint64_t& uniqueId);

	/**
	 * Removes a script based of the script's name. Returns false if the script wasn't able to immediatly detach.
	 *
	 * @param name Name of the script to remove.
	 * @return False if the script wasn't able to immeditaly detach.
	 */
	bool removeScript(const std::string& name);

	/**
	 * Execute all running script's loop logic. This should be called every game-frame.
	 */
	void executeScripts();

	ScriptModule* accessScript(const uint64_t& scriptUniqueId);

	ScriptModule* accessScript(const std::string& name);

	Hook* accessHook(const std::string& name);

	void setGameThreadId(const DWORD& gameThreadId);

	/**
	 * Gets the saved id of the game's main thread.
	 *
	 * @return Thread id of the game's main thread.
	 */
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
