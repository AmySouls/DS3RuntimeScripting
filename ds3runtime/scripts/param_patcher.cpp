#pragma once
#include "pch.h"
#include "param_patcher.h"
#include "dist/pugixml.hpp"
#include <ds3runtime/world_chr_man.h>
#include "ds3runtime/databaseaddress.h"

namespace ds3runtime {

ParamHandler::ParamHandler(const std::string& patchId, const std::wstring& param, const int32_t& id)
{
    this->patchId = patchId;
    this->param = param;
    this->id = id;
}

bool ParamHandler::isValidParam() const
{
    return (dynamic_cast<ParamPatcher*>(ds3runtime_global->accessScript("param_patcher")))
        ->isValidParam(param, id);
}

bool ParamHandler::readBinary(const uintptr_t& offset, const uint8_t& binaryOffset) const
{
    return (dynamic_cast<ParamPatcher*>(ds3runtime_global->accessScript("param_patcher")))
        ->readBinary(param, id, offset, binaryOffset);
}

bool ParamHandler::readBinaryOriginal(const uintptr_t& offset, const uint8_t& binaryOffset) const
{
    return (dynamic_cast<ParamPatcher*>(ds3runtime_global->accessScript("param_patcher")))
        ->readBinaryOriginal(param, id, offset, binaryOffset);
}

void ParamHandler::patchBinary(const uintptr_t& offset, const int8_t& binaryOffset, const bool& value)
{
    ParamPatchInfo patch = {};
    patch.patchId = patchId;
    patch.value = value;
    patch.size = sizeof(value);
    (dynamic_cast<ParamPatcher*>(ds3runtime_global->accessScript("param_patcher")))
        ->patchBinary(param, id, offset, binaryOffset, patch);
}

void ParamHandler::patchBinary(const std::string& fieldName, const bool& value)
{
    for (ParamField field : (dynamic_cast<ParamPatcher*>(ds3runtime_global->accessScript("param_patcher")))
	 	->getParamLayout(param)) {
        if (field.fieldName != fieldName) continue;
        patchBinary(field.bitOffset - field.bitOffset % 8, field.bitOffset % 8, value);
        break;
    }
}

void ParamPatcher::execute()
{
}

bool ParamPatcher::onAttach()
{
    if (!accessMultilevelPointer<uintptr_t>(DataBaseAddress::SoloParamRepository, 0x10C0, 0x68, 0x68)) {
        return false;
    }

    const uintptr_t paramStart = *accessMultilevelPointer<uintptr_t>(0x144785FE0, 0x10);
    const uintptr_t paramEnd = *accessMultilevelPointer<uintptr_t>(0x144785FE0, 0x10 + 8);
    const uintptr_t paramCount = (paramEnd - paramStart) / 8;

    for (uintptr_t i = 0; i < paramCount; i++) {
        const uintptr_t paramOffset = *accessMultilevelPointer<uintptr_t>(paramStart + i * 8);
        std::wstring paramName = L"UnknownParam";

        if (*accessMultilevelPointer<int32_t>(paramOffset + 0x20) > 7) {
            paramName = accessMultilevelPointer<const wchar_t>(paramOffset + 0x10, 0);
        }
        else {
            paramName = accessMultilevelPointer<const wchar_t>(paramOffset + 0x10);
        }

        paramOffsetTable[paramName] = paramOffset;
    }
    
    for (auto paramEntry : paramOffsetTable) {
        paramIdTables[paramEntry.first] = createParamIdTable(paramEntry.second);
    }

    for (auto paramEntry : paramLayoutFileTable) {
        pugi::xml_document layoutFile;

        if (layoutFile.load_file(("DS3RuntimeScripting/assets/DS3/Layouts/" + ds3runtime_global->utf8_encode(paramEntry.second)).c_str())) {
            pugi::xml_node layout = layoutFile.child("layout");
            if (!layout) continue;
            uint64_t bitOffset = 0;

            for (pugi::xml_node entry : layout.children("entry")) {
                ParamField paramField = {};
                paramField.fieldName = entry.child("name").text().as_string();
                paramField.fieldType = entry.child("type").text().as_string();
                paramField.bitOffset = bitOffset;
                pugi::xml_text defaultValue = entry.child("type").text();
                bool invalidType = false;
               
                if (paramField.fieldType == "b8") {
                    std::string value = defaultValue.as_string();
                    bool boolValue = false;
                    if (value == "False") boolValue = false;
                    else if (value == "True") boolValue = true;
                    memcpy((void*)&paramField.defaultValue, &boolValue, 1);
                    bitOffset += 1;
                }
                else if (paramField.fieldType == "s8") {
                    int8_t value = defaultValue.as_int();
                    memcpy((void*)&paramField.defaultValue, &value, 1);
                    bitOffset += 8;
                }
                else if (paramField.fieldType == "u8") {
                    uint8_t value = defaultValue.as_uint();
                    memcpy((void*)&paramField.defaultValue, &value, 1);
                    bitOffset += 8;
                }
                else if (paramField.fieldType == "s16") {
                    int16_t value = defaultValue.as_int();
                    memcpy((void*)&paramField.defaultValue, &value, 2);
                    bitOffset += 16;
                }
                else if (paramField.fieldType == "u16") {
                    uint16_t value = defaultValue.as_uint();
                    memcpy((void*)&paramField.defaultValue, &value, 2);
                    bitOffset += 16;
                }
                else if (paramField.fieldType == "s32") {
                    int32_t value = defaultValue.as_int();
                    memcpy((void*)&paramField.defaultValue, &value, 4);
                    bitOffset += 32;
                }
                else if (paramField.fieldType == "u32") {
                    uint32_t value = defaultValue.as_uint();
                    memcpy((void*)&paramField.defaultValue, &value, 4);
                    bitOffset += 32;
                }
                else if (paramField.fieldType == "f32") {
                    float value = defaultValue.as_float();
                    memcpy((void*)&paramField.defaultValue, &value, 4);
                    bitOffset += 32;
                }
                else if (paramField.fieldType == "s64") {
                    int64_t value = defaultValue.as_llong();
                    memcpy((void*)&paramField.defaultValue, &value, 8);
                    bitOffset += 64;
                }
                else if (paramField.fieldType == "u64") {
                    uint64_t value = defaultValue.as_ullong();
                    memcpy((void*)&paramField.defaultValue, &value, 8);
                    bitOffset += 64;
                }
                else {
                    invalidType = true;
                }

                if (!invalidType) paramLayoutTable[paramEntry.first].push_back(paramField);
            }
        }
    }

    return true;
}

bool ParamPatcher::onDetach()
{
    return true;
}

bool ParamPatcher::doesIdExistInParam(const std::wstring& param, const int32_t& id)
{
    return paramIdTables[const_cast<std::wstring&>(param)].find(id) != paramIdTables[const_cast<std::wstring&>(param)].end();
}

bool ParamPatcher::isValidParam(const std::wstring& param, const int32_t& id)
{
    return accessMultilevelPointer<uint8_t>(paramIdTables[param][id]) != nullptr;
}

bool ParamPatcher::readBinary(const std::wstring& param, const int32_t& id, const uintptr_t& offset, const uint8_t& binaryOffset)
{
    return (*accessMultilevelPointer<uint8_t>(paramIdTables[param][id] + offset) & static_cast<uint8_t>(pow(2, binaryOffset))) == pow(2, binaryOffset);
}

bool ParamPatcher::readBinaryOriginal(const std::wstring& param, const int32_t& id, const uintptr_t& offset, const uint8_t& binaryOffset)
{
    std::vector<ParamPatchInfo>* patchList = &binaryPatchMap[param][id][offset][binaryOffset];

    if (patchList->size() == 0) {
        return (*accessMultilevelPointer<uint8_t>(paramIdTables[param][id] + offset) & static_cast<uint8_t>(pow(2, binaryOffset))) == pow(2, binaryOffset);
    }
    else {
        return (patchList->at(0).value & static_cast<uint8_t>(pow(2, binaryOffset))) == pow(2, binaryOffset);
    }
}

void ParamPatcher::patch(const std::wstring& param, const int32_t& id, const uintptr_t& offset, const ParamPatchInfo& patch)
{
    if (patch.patchId == "original") return;
    std::unordered_map<int32_t,std::unordered_map<uintptr_t, std::vector<ParamPatchInfo>>> mapHuh = patchMap[param];
    std::vector<ParamPatchInfo> patchList = patchMap[param][id][offset];

    if (patchList.size() == 0) {
        ParamPatchInfo original = patch;
        original.patchId = "original";
        memcpy(&original.value, (void*)(paramIdTables[param][id] + offset), patch.size);
        patchList.push_back(original);
    }

    memcpy((void*)(paramIdTables[param][id] + offset), &patch.value, patch.size);
    patchList.push_back(patch);
    patchMap[param][id][offset] = patchList;
}

void ParamPatcher::patchBinary(const std::wstring& param, const int32_t& id, const uintptr_t& offset, const uint8_t& binaryOffset, const ParamPatchInfo& patch)
{
    if (patch.patchId == "original") return;
    std::vector<ParamPatchInfo>* patchList = &binaryPatchMap[param][id][offset][binaryOffset];
    const uint8_t currentByte = *accessMultilevelPointer<uint8_t>(paramIdTables[param][id] + offset);

    if (patchList->size() == 0) {
        ParamPatchInfo original = patch;
        original.patchId = "original";
        original.value = (currentByte & static_cast<uint8_t>(pow(2, binaryOffset))) == pow(2, binaryOffset);
        patchList->push_back(original);
    }

    uint8_t newByte = currentByte;
    if (patch.value) newByte = newByte | static_cast<uint8_t>(pow(2, binaryOffset));
    else newByte = (newByte & ~static_cast<uint8_t>(pow(2, binaryOffset)));
    *accessMultilevelPointer<uint8_t>(paramIdTables[param][id] + offset) = newByte;
    patchList->push_back(patch);
}

void ParamPatcher::restore(const std::wstring& param, const int32_t& id, const uintptr_t& offset, const std::string& patchId)
{
    if (patchId == "original") return;
    std::vector<ParamPatchInfo>* patchList = &patchMap[param][id][offset];

    patchList->erase(std::remove_if(patchList->begin(), patchList->end(), [&](auto patch) -> bool {
        if (patchId == patch.patchId) return true;

        return false;
        }),
        patchList->end());

    if (patchList->size() == 0) return;
    const ParamPatchInfo patch = (*patchList)[patchList->size() - 1];
    memcpy((void*)(paramIdTables[param][id] + offset), &patch.value, patch.size);
}

void ParamPatcher::restoreBinary(const std::wstring& param, const int32_t& id, const uintptr_t& offset, const uint8_t& binaryOffset, const std::string& patchId)
{
    if (patchId == "original") return;
    std::vector<ParamPatchInfo>* patchList = &binaryPatchMap[param][id][offset][binaryOffset];

    patchList->erase(std::remove_if(patchList->begin(), patchList->end(), [&](auto patch) -> bool {
        if (patchId == patch.patchId) return true;
        return false;
        }),
        patchList->end());

    if (patchList->size() == 0) return;
    const ParamPatchInfo patch = (*patchList)[patchList->size() - 1];
    const uint8_t currentByte = *accessMultilevelPointer<uint8_t>(paramIdTables[param][id] + offset);
    uint8_t newByte = currentByte;
    if (patch.value) newByte = newByte | static_cast<uint8_t>(pow(2, binaryOffset));
    else newByte = (newByte & ~static_cast<uint8_t>(pow(2, binaryOffset)));
    *accessMultilevelPointer<uint8_t>(paramIdTables[param][id] + offset) = newByte;
}

void ParamPatcher::restore(const std::string& patchId)
{
    for (auto param : patchMap) {
        for (auto ids : param.second) {
            for (auto offsetPatches : ids.second) {
                restore(param.first, ids.first, offsetPatches.first, patchId);
            }
        }
    }

    for (auto param : binaryPatchMap) {
        for (auto ids : param.second) {
            for (auto offsets : ids.second) {
                for (auto binaryPatches : offsets.second) {
                    restoreBinary(param.first, ids.first, offsets.first, binaryPatches.first, patchId);
                }
            }
        }
    }
}

std::unordered_map<int32_t, uint64_t> ParamPatcher::createParamIdTable(const uintptr_t& paramPointer)
{
	uintptr_t paramAddress = *accessMultilevelPointer<uintptr_t>(paramPointer + 0x68, 0x68);
	int32_t tableSize = *accessMultilevelPointer<unsigned short>(paramAddress + 0xA);
	std::unordered_map<int32_t, uintptr_t> table;

	for (uintptr_t i = 0; i < tableSize; i++) {
		int32_t pID = *accessMultilevelPointer<int32_t>(paramAddress + 0x40 + 0x18 * i);
		int32_t iDO = *accessMultilevelPointer<int32_t>(paramAddress + 0x48 + 0x18 * i);
		table.emplace(pID, paramAddress + iDO);
	}

	return table;
}

uintptr_t ParamPatcher::getIdAddress(const std::unordered_map<int, uintptr_t>& idTable, const int32_t& id)
{
    uintptr_t result = 0;

    for (auto& element: idTable) {
        int idItr = element.first;
        uintptr_t address = element.second;
        if (id == idItr) result = address;
    }

    return result;
}

int32_t ParamPatcher::getIdFromAddress(const std::unordered_map<int, uintptr_t>& idTable, const uintptr_t& address)
{
    int32_t result = 0;

    for (auto& element: idTable) {
        int32_t id = element.first;
        uintptr_t addressItr = element.second;
        if (addressItr == address) result = id;
    }

    return result;
}

std::vector<ParamField> ParamPatcher::getParamLayout(const std::wstring& paramName)
{
    return paramLayoutTable[paramName];
}

std::vector<int32_t> ParamPatcher::getIdsInParam(const std::wstring& paramName)
{
    std::vector<int32_t> ids(paramIdTables[paramName].size());
    for (auto entry : paramIdTables[paramName]) ids.push_back(entry.first);
    return ids;
}

std::unordered_map<std::wstring, std::wstring> ParamPatcher::paramLayoutFileTable = {
    { L"ActionButtonParam", L"ACTIONBUTTON_PARAM_ST.xml" },
    { L"AiSoundParam", L"AI_SOUND_PARAM_ST.xml" },
    { L"AtkParam_Pc", L"ATK_PARAM_ST.xml" },
    { L"AtkParam_Npc", L"ATK_PARAM_ST.xml" },
    { L"AttackElementCorrectParam", L"ATTACK_ELEMENT_CORRECT_PARAM_ST.xml" },
    { L"BehaviorParam_PC", L"BEHAVIOR_PARAM_ST.xml" },
    { L"BehaviorParam", L"BEHAVIOR_PARAM_ST.xml" },
    { L"BonfireWarpParam", L"BONFIRE_WARP_PARAM_ST.xml" },
    { L"BudgetParam", L"BUDGET_PARAM_ST.xml" },
    { L"BulletCreateLimitParam", L"BULLET_CREATE_LIMIT_PARAM_ST.xml" },
    { L"Bullet", L"BULLET_PARAM_ST.xml" },
    { L"CalcCorrectGraph",  L"CACL_CORRECT_GRAPH_ST.xml" },
    { L"Ceremony", L"CEREMONY_PARAM_ST.xml" },
    { L"CharaInitParam", L"CHARACTER_INIT_PARAM.xml" },
    { L"CharacterLoadParam", L"CHARACTER_LOAD_PARAM_ST.xml" },
    { L"CharMakeMenuListItemParam", L"CHARMAKEMENU_LISTITEM_PARAM_ST.xml" },
    { L"CharMakeMenuTopParam", L"CHARMAKEMENUTOP_PARAM_ST.xml" },
    { L"ClearCountCorrectParam", L"CLEAR_COUNT_CORRECT_PARAM_ST.xml" },
    { L"CoolTimeParam", L"COOL_TIME_PARAM_ST.xml" },
    { L"CultSettingParam", L"CULT_SETTING_PARAM_ST.xml" },
    { L"DecalParam",  L"DECAL_PARAM_ST.xml" },
    { L"DirectionCameraParam", L"DIRECTION_CAMERA_PARAM_ST.xml" },
    { L"EquipMtrlSetParam", L"EQUIP_MTRL_SET_PARAM_ST.xml" },
    { L"EquipParamAccessory", L"EQUIP_PARAM_ACCESSORY_ST.xml" },
    { L"EquipParamGoods", L"EQUIP_PARAM_GOODS_ST.xml" },
    { L"EquipParamProtector", L"EQUIP_PARAM_PROTECTOR_ST" },
    { L"EquipParamWeapon", L"EQUIP_PARAM_WEAPON_ST.xml" },
    { L"HPEstusFlaskRecoveryParam", L"ESTUS_FLASK_RECOVERY_PARAM_ST.xml" },
    { L"FaceGenParam", L"FACE_GEN_PARAM_ST.xml" },
    { L"FaceParam", L"FACE_PARAM_ST.xml" },
    { L"FaceRangeParam", L"FACE_RANGE_PARAM_ST.xml" },
    { L"FootSfxParam", L"FOOT_SFX_PARAM_ST.xml" },
    { L"GameAreaParam",L"GAME_AREA_PARAM_ST.xml" },
    { L"GameProgressParam", L"GAME_PROGRESS_PARAM_ST.xml" },
    { L"GemCategoryParam", L"GEM_CATEGORY_PARAM_ST.xml" },
    { L"GemDropDopingParam", L"GEM_DROP_DOPING_PARAM_ST.xml" },
    { L"GemDropModifyParam", L"GEM_DROP_MODIFY_PARAM_ST.xml" },
    { L"GemGenParam", L"GEM_GEN_PARAM_ST.xml" },
    { L"GemeffectParam", L"GEMEFFECT_PARAM_ST.xml" },
    { L"HitEffectSeParam", L"HIT_EFFECT_SE_PARAM_ST.xml" },
    { L"HitEffectSfxConceptParam", L"HIT_EFFECT_SFX_CONCEPT_PARAM_ST.xml" },
    { L"HitEffectSfxParam", L"HIT_EFFECT_SFX_PARAM_ST.xml" },
    { L"HitMtrlParam", L"HIT_MTRL_PARAM_ST.xml" },
    { L"ItemLotParam", L"ITEMLOT_PARAM_ST.xml" },
    { L"KnockBackParam", L"KNOCKBACK_PARAM_ST.xml" },
    { L"KnowledgeLoadScreenItemParam", L"KNOWLEDGE_LOADSCREEN_ITEM_PARAM_ST.xml" },
    { L"LoadBalancerDrawDistScaleParam", L"LOAD_BALANCER_DRAW_DIST_SCALE_PARAM_ST.xml" },
    { L"LoadBalancerParam", L"LOAD_BALANCER_PARAM_ST.xml" },
    { L"LockCamParam", L"LOCK_CAM_PARAM_ST.xml" },
    { L"LodParam", L"LOD_BANK.xml" },
    { L"Magic", L"MAGIC_PARAM_ST.xml" },
    { L"MapMimicryEstablishmentParam", L"MAP_MIMICRY_ESTABLISHMENT_PARAM_ST.xml" },
    { L"MenuOffscrRendParam", L"MENU_OFFSCR_REND_PARAM_ST.xml" },
    { L"NewMenuColorTableParam", L"MENU_PARAM_COLOR_TABLE_ST.xml" },
    { L"MenuValueTableParam", L"MENU_VALUE_TABLE_SPEC.xml" },
    { L"MenuPropertyLayoutParam", L"MENUPROPERTY_LAYOUT.xml" },
    { L"MenuPropertySpecParam", L"MENUPROPERTY_SPEC.xml" },
    { L"ModelSfxParam",  L"MODEL_SFX_PARAM_ST.xml" },
    { L"MoveParam", L"MOVE_PARAM_ST.xml" },
    { L"MultiHPEstusFlaskBonusParam", L"MULTI_ESTUS_FLASK_BONUS_PARAM_S.xml" },
    { L"MultiPlayCorrectionParam", L"MULTI_PLAY_CORRECTION_PARAM_ST.xml" },
    { L"MultiSoulBonusRateParam", L"MULTI_SOUL_BONUS_RATE_PARAM_ST.xml" },
    { L"NetworkAreaParam", L"NETWORK_AREA_PARAM_S.xml" },
    { L"NetworkMsgParam", L"NETWORK_MSG_PARAM_S.xml" },
    { L"NetworkParam", L"NETWORK_PARAM_ST.xml" },
    { L"NpcAiActionParam",  L"NPC_AI_ACTION_PARAM_ST.xml" },
    { L"NpcParam", L"NPC_PARAM_ST.xml" },
    { L"NpcThinkParam", L"NPC_THINK_PARAM_ST.xml" },
    { L"ObjActParam", L"OBJ_ACT_PARAM_ST.xml" },
    { L"ObjectMaterialSfxParam",  L"OBJECT_MATERIAL_SFX_PARAM_ST.xml" },
    { L"ObjectParam", L"OBJECT_PARAM_ST.xml" },
    { L"PhantomParam", L"PHANTOM_PARAM_ST.xml" },
    { L"PlayRegionParam", L"PLAY_REGION_PARAM_ST.xml" },
    { L"ProtectorGenParam", L"PROTECTOR_GEN_PARAM_ST.xml" },
    { L"RagdollParam", L"RAGDOLL_PARAM_ST.xml" },
    { L"ReinforceParamProtector", L"REINFORCE_PARAM_PROTECTOR_ST.xml" },
    { L"ReinforceParamWeapon", L"REINFORCE_PARAM_WEAPON_ST.xml" },
    { L"RoleParam", L"ROLE_PARAM_ST.xml" },
    { L"SeMaterialConvertParam", L"SE_MATERIAL_CONVERT_PARAM_ST.xml" },
    { L"ShopLineupParam", L"SHOP_LINEUP_PARAM.xml" },
    { L"SkeletonParam", L"SKELETON_PARAM_ST.xml" },
    { L"SpEffectParam", L"SP_EFFECT_PARAM_ST.xml" },
    { L"SpEffectVfxParam", L"SP_EFFECT_VFX_PARAM_ST.xml" },
    { L"SwordArtsParam", L"SWORD_ARTS_PARAM_ST.xml" },
    { L"TalkParam", L"TALK_PARAM_ST.xml" },
    { L"ThrowDirectionSfxParam", L"THROW_DIRECTION_SFX_PARAM_ST.xml" },
    { L"ThrowParam", L"THROW_INFO_BANK.xml" },
    { L"ToughnessParam",  L"TOUGHNESS_PARAM_ST.xml" },
    { L"UpperArmParam", L"UPPER_ARM_PARAM_ST.xml" },
    { L"WeaponGenParam", L"WEAPON_GEN_PARAM_ST.xml" },
    { L"WepAbsorpPosParam", L"WEP_ABSORP_POS_PARAM_ST.xml" },
    { L"WetAspectParam", L"WET_ASPECT_PARAM_ST.xml" },
    { L"WhiteSignCoolTimeParam", L"WHITE_SIGN_COOL_TIME_PARAM_ST.xml" },
    { L"Wind", L"WIND_PARAM_ST.xml" },
};

}
