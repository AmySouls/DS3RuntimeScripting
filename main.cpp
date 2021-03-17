#pragma once
#include "pch.h"
#include "ds3runtime/ds3runtime.h"
#include "ds3runtime/hooks/game_frame_hook.h"
#include "ds3runtime/hooks/lua_capture.h"
#include "ds3runtime/hooks/animation_id_hook.h"
#include "ds3runtime/hooks/session_receive_hook.h"
#include "ds3runtime/scripts/latency_simulator.h"
#include "ds3runtime/scripts/sync_call_script.h"
#include "ds3runtime/scripts/king_crimson_protections.h"
#include <ds3runtime/hooks/play_animation_hook.h>

using namespace ds3runtime;

static AsyncModule* script;
std::shared_ptr<DS3RuntimeScripting> ds3runtime::ds3runtime_global;

static std::shared_ptr<spdlog::logger> createLogger()
{
    auto logger = std::make_shared<spdlog::logger>("DS3RuntimeScripting");

    logger->sinks().push_back(std::make_shared<spdlog::sinks::basic_file_sink_mt>("DS3RuntimeScripting.log"));
    logger->set_level(spdlog::level::info);
    logger->flush_on(spdlog::level::info);

    if (_DEBUG) {
        AllocConsole();
        FILE* stream;
        freopen_s(&stream, "CONOUT$", "w", stdout);
        freopen_s(&stream, "CONIN$", "r", stdin);

        logger->sinks().push_back(std::make_shared<spdlog::sinks::stdout_color_sink_mt>());
        logger->set_level(spdlog::level::debug);
    }

    return logger;
}

static bool attach()
{
    ds3runtime_global.reset(new DS3RuntimeScripting);
    spdlog::set_default_logger(createLogger());
    spdlog::info("Sucessfully loaded!");
    /*
    ds3runtime_global->addHook(std::make_shared<GameFrameHook>());
    ds3runtime_global->addHook(std::make_shared<LuaCapture>());
    ds3runtime_global->addHook(std::make_shared<AnimationIdHook>());
    ds3runtime_global->addHook(std::make_shared<SessionReceiveHook>());
    ds3runtime_global->addHook(std::make_shared<SessionSendHook>());
    ds3runtime_global->addHook(std::make_shared<PlayAnimationHook>());
    ds3runtime_global->runScript(std::make_shared<SyncCallScript>());
    ds3runtime_global->runScript(std::make_shared<KingCrimsonProtections>());
    ds3runtime_global->runScript(std::make_shared<LatencySimulator>());
    ds3runtime_global->attach();
    */
    return true;
}

static bool detach()
{
    /*
    ds3runtime::ds3runtime_global->detach();
    */
    spdlog::info("Sucessfully unloaded!");
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

