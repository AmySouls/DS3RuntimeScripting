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
#include <ds3runtime/hooks/ds1_backstabs.h>
#include <ds3runtime/hooks/ds3_iframe_patch_hit_reg_hook.h>
#include <ds3runtime/hooks/ds3_iframe_patch_hit_reg_hook_shockwave.h>
#include <ds3runtime/hooks/ds3_iframe_patch_hit_uuid_hook.h>
#include <ds3runtime/hooks/ds3_iframe_patch_hit_uuid_identify.h>
#include <ds3runtime/hooks/ds3_iframe_patch_hit_sibling_hook.h>
#include <ds3runtime/hooks/ds3_iframe_patch_hit_delete.h>
#include <ds3runtime/hooks/ds3_iframe_patch_no_player_iframes.h>
#include <ds3runtime/hooks/ds3_iframe_patch_attacker_effect_hook.h>
#include <ds3runtime/hooks/ds3_iframe_patch_durablity_cost_hook.h>
#include <ds3runtime/scripts/ds3_iframe_patch.h>

using namespace ds3runtime;

std::shared_ptr<DS3RuntimeScripting> ds3runtime::ds3runtime_global;;

static std::shared_ptr<spdlog::logger> createLogger()
{
    auto logger = std::make_shared<spdlog::logger>("DS3RuntimeScripting");

    logger->sinks().push_back(std::make_shared<spdlog::sinks::basic_file_sink_mt>("DS3RuntimeScripting.log"));
    logger->set_level(spdlog::level::info);
    logger->flush_on(spdlog::level::info);

//#ifdef _DEBUG
    AllocConsole();
    FILE* stream;
    freopen_s(&stream, "CONOUT$", "w", stdout);
    freopen_s(&stream, "CONIN$", "r", stdin);

    logger->sinks().push_back(std::make_shared<spdlog::sinks::stdout_color_sink_mt>());
    logger->set_level(spdlog::level::debug);
//#endif

    return logger;
}

static bool attach()
{
    ds3runtime_global.reset(new DS3RuntimeScripting);
    spdlog::set_default_logger(createLogger());
    ds3runtime_global->addHook(std::make_unique<GameFrameHook>());
    //ds3runtime_global->addHook(std::make_unique<LuaCapture>());
    //ds3runtime_global->runScript(std::make_unique<HotkeyManager>());
    ds3runtime_global->addHook(std::make_unique<SessionReceiveHook>());
    ds3runtime_global->addHook(std::make_unique<SessionSendHook>());
    //ds3runtime_global->addHook(std::make_unique<ThrowHook>());
    ds3runtime_global->addHook(std::make_unique<SprjChrDamageModuleHook>());
    //ds3runtime_global->addHook(std::make_unique<DS1Backstabs>());
    ds3runtime_global->addHook(std::make_unique<PlayAnimationHook>());
    ds3runtime_global->runScript(std::make_unique<ParamPatcher>());
    //ds3runtime_global->runScript((std::make_unique<FMODSystemHandler>()));
    //ds3runtime_global->runScript(std::make_unique<AnimationIdHandler>());
    //ds3runtime_global->runScript(std::make_unique<SyncCallScript>());
    //ds3runtime_global->runScript(std::make_unique<KingCrimsonProtections>());
    //ds3runtime_global->runScript(std::make_unique<LatencySimulator>());
    //ds3runtime_global->runScript(std::make_unique<FaceDataCapture>());
    //ds3runtime_global->runScript(std::make_unique<ImposterSoundCosmetic>());
    //ds3runtime_global->runScript(std::make_unique<BonkSoundCosmetic>());
    //ds3runtime_global->runScript(std::make_unique<InterruptAttacks>());
    //ds3runtime_global->runScript(std::make_unique<TalkWithPlayerName>());
    //ds3runtime_global->runScript(std::make_unique<DynamicPvpPatch>());
    //ds3runtime_global->setAsyncMode(true);
    //ds3runtime_global->runScript(std::make_unique<NPCModTest>());
    //ds3runtime_global->runScript(std::make_unique<YhormScreamOnWarcry>());

    ds3runtime_global->addHook(std::make_unique<ds3IFramePatch::FixDurablityCost>());
    ds3runtime_global->addHook(std::make_unique<ds3IFramePatch::AttackerEffectHook>());
    ds3runtime_global->addHook(std::make_unique<ds3IFramePatch::HitBoxDeleteHook>());
    ds3runtime_global->addHook(std::make_unique<ds3IFramePatch::HitBoxHierarchyHook>());
    ds3runtime_global->addHook(std::make_unique<ds3IFramePatch::NoPlayerIFrames>());
    ds3runtime_global->addHook(std::make_unique<ds3IFramePatch::HitUUIDIdentifyHook>());
    ds3runtime_global->addHook(std::make_unique<ds3IFramePatch::HitRegHook>());
    ds3runtime_global->addHook(std::make_unique<ds3IFramePatch::HitRegHookShockwave>());
    ds3runtime_global->addHook(std::make_unique<ds3IFramePatch::HitBoxUUIDHook>());
    ds3runtime_global->runScript(std::make_unique<ds3IFramePatch::DS3IFramePatch>());
    

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

