#include "pch.h"
#include "ds3runtime/ds3runtime.h"

using namespace ds3runtime;

Log* ds3runtimeLog;
static AsyncModule* script;

static bool attach()
{
    ds3runtimeLog = new Log(GetStdHandle(STD_OUTPUT_HANDLE));
    ds3runtimeLog->info("Sucessfully loaded!");
    return true;
}

static bool detach()
{
    if (script) { //TODO: Reorganize this into a class tasks with managing all scripts
        if (script->isSafeToDelete()) {
            delete script;
            script = nullptr;
        }
        else {
            if (!script->isDestroyed()) script->destroy();
            DWORD waitResult = WaitForSingleObject(script->getHandle(), 10000);
            if (waitResult == WAIT_OBJECT_0) ds3runtimeLog->info("Succesfully ended async test script thread.");
            else if (waitResult == WAIT_TIMEOUT) {
                ds3runtimeLog->error("Async test script destruction timed out.");
                return true;
            }
            else if (waitResult == WAIT_FAILED) {
                ds3runtimeLog->error("Async test script destruction failed.");
                return true;
            }

            delete script;
            script = nullptr;
        }
    }

    ds3runtimeLog->info("Sucessfully unloaded!");
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

