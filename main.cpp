#pragma once
#include "pch.h"

#include "ds3runtime/ds3runtime.h"
#include "ds3runtime/hooks/game_frame_hook.h"
#include "ds3runtime/hooks/session_receive_hook.h"
#include "ds3runtime/hooks/session_send_hook.h"
#include "ds3runtime/hooks/lua_capture.h"
#include "ds3runtime/scripts/animation_id_handler.h"
#include <ds3runtime/hooks/play_animation_hook.h>
#include <ds3runtime/hooks/throw_hook.h>
#include <ds3runtime/scripts/hotkey_manager.h>
#include <ds3runtime/scripts/fmod_system_handler.h>
#include <ds3runtime/scripts/param_patcher.h>
#include <ds3runtime/hooks/ds1_backstabs.h>
#include <ds3runtime/hooks/ds1_no_hyperarmor.h>
#include <ds3runtime/hooks/better_swordspear_hook.h>
#include <ds3runtime/scripts/ds1_passive_poise.h>
#include <ds3runtime/boss/build_copy.h>

using namespace ds3runtime;

std::shared_ptr<DS3RuntimeScripting> ds3runtime::ds3runtime_global;;

static std::shared_ptr<spdlog::logger> createLogger()
{
    auto logger = std::make_shared<spdlog::logger>("DS3RuntimeScripting");

    logger->sinks().push_back(std::make_shared<spdlog::sinks::basic_file_sink_mt>("DS3RuntimeScripting.log"));
    logger->set_level(spdlog::level::info);
    logger->flush_on(spdlog::level::info);

#ifdef _DEBUG
    AllocConsole();
    FILE* stream;
    freopen_s(&stream, "CONOUT$", "w", stdout);
    freopen_s(&stream, "CONIN$", "r", stdin);

    logger->sinks().push_back(std::make_shared<spdlog::sinks::stdout_color_sink_mt>());
    logger->set_level(spdlog::level::debug);
#endif

    return logger;
}

static bool attach()
{
    ds3runtime_global.reset(new DS3RuntimeScripting);
    spdlog::set_default_logger(createLogger());

    ds3runtime_global->addHook(std::make_unique<GameFrameHook>());
    ds3runtime_global->runScript(std::make_unique<HotkeyManager>());
    ds3runtime_global->addHook(std::make_unique<SessionReceiveHook>());
    ds3runtime_global->addHook(std::make_unique<SessionSendHook>());
    ds3runtime_global->addHook(std::make_unique<ThrowHook>());
    ds3runtime_global->addHook(std::make_unique<PlayAnimationHook>());
    ds3runtime_global->runScript(std::make_unique<ParamPatcher>());
    ds3runtime_global->runScript(std::make_unique<FMODSystemHandler>());
    ds3runtime_global->runScript(std::make_unique<AnimationIdHandler>());
    ds3runtime_global->runScript(std::make_unique<BuildCopy>());

    /*
    ds3runtime_global->runScript(std::make_unique<DS1PassivePoise>());
    ds3runtime_global->addHook(std::make_unique<GameFrameHook>());
    ds3runtime_global->addHook(std::make_unique<ThrowHook>());
    ds3runtime_global->addHook(std::make_unique<PlayAnimationHook>());
    ds3runtime_global->addHook(std::make_unique<DS1Backstabs>());
    ds3runtime_global->addHook(std::make_unique<DS1NoHyperArmor>());
    ds3runtime_global->runScript(std::make_unique<ParamPatcher>());
    ds3runtime_global->runScript(std::make_unique<FMODSystemHandler>());
    */

    ds3runtime_global->attach();
    spdlog::info("Sucessfully loaded!");
    return true;
}

static bool detach()
{
    const bool result = ds3runtime::ds3runtime_global->detach();
    if (result) spdlog::info("Sucessfully unloaded!");
    else spdlog::info("Unable to unload.");
    return result;
}

BOOL APIENTRY DllMain(HMODULE, DWORD dwReason, LPVOID)
{
    switch (dwReason) {
    case DLL_PROCESS_ATTACH:
        return attach();
        break;
    case DLL_PROCESS_DETACH:
        return detach();
        break;
    }

    return TRUE;
}

