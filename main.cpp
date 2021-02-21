#include "pch.h"
#include "ds3runtime/ds3runtime.h"
#include "ds3runtime/hooks/game_frame_hook.h"
#include "ds3runtime/hooks/lua_capture.h"
#include "ds3runtime/scripts/test_script.h"

using namespace ds3runtime;

static AsyncModule* script;
std::shared_ptr<DS3RuntimeScripting> ds3runtime::ds3runtime_global;

static bool attach()
{
    ds3runtime_global.reset(new DS3RuntimeScripting);
    ds3runtime_global->createLog(GetStdHandle(STD_OUTPUT_HANDLE));
    ds3runtime_global->getLog()->info("Sucessfully loaded!");
    std::shared_ptr<Hook> gameFrameHook = std::make_shared<GameFrameHook>();
    std::shared_ptr<Hook> luaCaptureHook = std::make_shared<LuaCapture>();
    ds3runtime_global->addHook(gameFrameHook);
    ds3runtime_global->addHook(luaCaptureHook);
    std::shared_ptr<ScriptModule> testScript = std::make_shared<TestScript>();
    ds3runtime_global->runScript(testScript);
    ds3runtime_global->attach();
    return true;
}

static bool detach()
{
    ds3runtime::ds3runtime_global->detach();
    ds3runtime::ds3runtime_global->getLog()->info("Sucessfully unloaded!");
    return true;
}

BOOL APIENTRY DllMain(HMODULE, DWORD dwReason, LPVOID)
{
    switch (dwReason) {
    case DLL_PROCESS_ATTACH:
        return attach();
    case DLL_PROCESS_DETACH:
        return detach();
    }

    return TRUE;
}

