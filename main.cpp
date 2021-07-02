#pragma once
#include "pch.h"

#include "ds3runtime/ds3runtime.h"
#include "ds3runtime/hooks/game_frame_hook.h"
#include "ds3runtime/hooks/lua_capture.h"
#include "ds3runtime/hooks/session_receive_hook.h"
#include "ds3runtime/scripts/animation_id_handler.h"
#include "ds3runtime/scripts/latency_simulator.h"
#include "ds3runtime/scripts/sync_call_script.h"
#include "ds3runtime/scripts/king_crimson_protections.h"
#include <ds3runtime/hooks/play_animation_hook.h>
#include <ds3runtime/hooks/throw_hook.h>
#include <ds3runtime/scripts/hotkey_manager.h>
#include <ds3runtime/scripts/param_patcher.h>
#include <ds3runtime/scripts/dynamic_pvp_patch.h>
#include <ds3runtime/scripts/face_data_capture.h>
#include <ds3runtime/scripts/bonk_sound_cosmetic.h>
#include <ds3runtime/hooks/sprj_chr_damage_module_hook.h>
#include <ds3runtime/scripts/imposter_sound_cosmetic.h>
#include <ds3runtime/scripts/fmod_system_handler.h>
#include <ds3runtime/scripts/npc_mod_test.h>
#include <ds3runtime/scripts/player_name_talk.h>
#include <ds3runtime/scripts/damage_update_test.h>
#include <ds3runtime/scripts/interrupt_attacks.h>

using namespace ds3runtime;

std::shared_ptr<DS3RuntimeScripting> ds3runtime::ds3runtime_global;;

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
    ds3runtime_global->addHook(std::make_shared<GameFrameHook>());
    ds3runtime_global->addHook(std::make_shared<LuaCapture>());
    ds3runtime_global->runScript(std::make_shared<HotkeyManager>());
    ds3runtime_global->addHook(std::make_shared<SessionReceiveHook>());
    ds3runtime_global->addHook(std::make_shared<SessionSendHook>());
    ds3runtime_global->addHook(std::make_shared<PlayAnimationHook>());
    ds3runtime_global->addHook(std::make_shared<ThrowHook>());
    ds3runtime_global->addHook(std::make_shared<SprjChrDamageModuleHook>());
    ds3runtime_global->runScript(std::make_shared<ParamPatcher>());
    ds3runtime_global->runScript((std::make_shared<FMODSystemHandler>()));
    ds3runtime_global->runScript(std::make_shared<AnimationIdHandler>());
    ds3runtime_global->runScript(std::make_shared<SyncCallScript>());
    ds3runtime_global->runScript(std::make_shared<KingCrimsonProtections>());
    //ds3runtime_global->runScript(std::make_shared<LatencySimulator>());
    ds3runtime_global->runScript(std::make_shared<FaceDataCapture>());
    //ds3runtime_global->runScript(std::make_shared<ImposterSoundCosmetic>());
    ds3runtime_global->runScript(std::make_shared<BonkSoundCosmetic>());
    //ds3runtime_global->runScript(std::make_shared<InterruptAttacks>());
    //ds3runtime_global->runScript(std::make_shared<TalkWithPlayerName>());
    //ds3runtime_global->runScript(std::make_shared<DynamicPvpPatch>());
    //ds3runtime_global->setAsyncMode(true);
    ds3runtime_global->runScript(std::make_shared<NPCModTest>());
    ds3runtime_global->attach();
    return true;
}

static bool detach()
{
    ds3runtime::ds3runtime_global->detach();
    spdlog::info("Sucessfully unloaded!");
    return true;
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

