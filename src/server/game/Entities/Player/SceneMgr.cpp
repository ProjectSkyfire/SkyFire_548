/*
* This file is part of Project SkyFire https://www.projectskyfire.org.
* See LICENSE.md file for Copyright information
*/

#include "SceneMgr.h"
#include "DB2Stores.h"
#include "Player.h"
#include "ScriptMgr.h"
#include "SpellAuraEffects.h"
#include "WorldPacket.h"

SceneMgr::SceneMgr(Player* player) : _player(player), _standaloneSceneInstanceID(0) { }

uint32 SceneMgr::PlayScene(uint32 sceneId, Position const* position /*= nullptr*/)
{
    return PlaySceneByTemplate(sObjectMgr->GetSceneTemplate(sceneId), position);
}

uint32 SceneMgr::PlaySceneByTemplate(SceneTemplate const* sceneTemplate, Position const* position /*= nullptr*/)
{
    if (!sceneTemplate)
        return 0;

    if (!sSceneScriptPackageStore.LookupEntry(sceneTemplate->ScenePackageId))
    {
        SF_LOG_ERROR("spells", "ScenePackageID: %u does not exist in db2.", sceneTemplate->ScenePackageId);
        return 0;
    }

    if (!position)
        position = GetPlayer();

    uint32 sceneInstanceID = GetNewStandaloneSceneInstanceID();
    ObjectGuid transportGUID = GetPlayer()->GetTransGUID();
    float orientation = position->GetOrientation();

    WorldPacket data(SMSG_PLAY_SCENE, 4 + 4 + 4 + 1 + 8 + 4 + 4 + 4 + 4 + 4);
    data << float(position->GetPositionY());
    data << float(position->GetPositionZ());
    data << float(position->GetPositionX());

    data.WriteBit(!sceneTemplate->PlaybackFlags);
    data.WriteBit(!sceneInstanceID);
    data.WriteBit(!orientation);
    data.WriteBit(!sceneTemplate->SceneId);
    data.WriteBit(!sceneTemplate->ScenePackageId);
    data.WriteBit(true);

    data.WriteGuidMask(transportGUID, 3, 2, 6, 4, 7, 1, 5, 0);
    data.FlushBits();
    data.WriteGuidBytes(transportGUID, 6, 3, 5, 4, 1, 2, 0, 7);

    if (sceneTemplate->PlaybackFlags)
        data << uint32(sceneTemplate->PlaybackFlags);
    if (orientation)
        data << float(orientation);
    if (sceneTemplate->SceneId)
        data << uint32(sceneTemplate->SceneId);
    if (sceneInstanceID)
        data << uint32(sceneInstanceID);
    if (sceneTemplate->ScenePackageId)
        data << uint32(sceneTemplate->ScenePackageId);

    GetPlayer()->SendDirectMessage(&data);

    AddInstanceIdToSceneMap(sceneInstanceID, sceneTemplate);

    sScriptMgr->OnSceneStart(GetPlayer(), sceneInstanceID, sceneTemplate);

    return sceneInstanceID;
}

uint32 SceneMgr::PlaySceneByPackageId(uint32 sceneScriptPackageId, uint32 playbackflags /*= SCENEFLAG_UNK16*/, Position const* position /*= nullptr*/)
{
    SceneTemplate sceneTemplate;
    sceneTemplate.SceneId = 0;
    sceneTemplate.ScenePackageId = sceneScriptPackageId;
    sceneTemplate.PlaybackFlags = playbackflags;
    sceneTemplate.ScriptId = 0;

    return PlaySceneByTemplate(&sceneTemplate, position);
}

void SceneMgr::CancelScene(uint32 sceneInstanceID, bool removeFromMap /*= true*/)
{
    if (removeFromMap)
        RemoveSceneInstanceId(sceneInstanceID);

    WorldPacket data(SMSG_CANCEL_SCENE, 4 + 1);
    data.WriteBit(!sceneInstanceID);
    data.FlushBits();
    if (sceneInstanceID)
        data << uint32(sceneInstanceID);

    GetPlayer()->SendDirectMessage(&data);
}

void SceneMgr::OnSceneTrigger(uint32 sceneInstanceID, std::string const& triggerName)
{
    SceneTemplate const* sceneTemplate = GetSceneTemplateFromInstanceId(sceneInstanceID);
    if (!sceneTemplate)
        return;

    sScriptMgr->OnSceneTrigger(GetPlayer(), sceneInstanceID, sceneTemplate, triggerName);
}

void SceneMgr::OnSceneCancel(uint32 sceneInstanceID)
{
    SceneTemplate const* found = GetSceneTemplateFromInstanceId(sceneInstanceID);
    if (!found)
        return;

    SceneTemplate sceneTemplate = *found;

    // Must be done before removing aura
    RemoveSceneInstanceId(sceneInstanceID);

    if (sceneTemplate.SceneId != 0)
        RemoveAurasDueToSceneId(sceneTemplate.SceneId);

    sScriptMgr->OnSceneCancel(GetPlayer(), sceneInstanceID, &sceneTemplate);

    if (sceneTemplate.PlaybackFlags & SCENEFLAG_CANCEL_AT_END)
        CancelScene(sceneInstanceID, false);
}

void SceneMgr::OnSceneComplete(uint32 sceneInstanceID)
{
    SceneTemplate const* found = GetSceneTemplateFromInstanceId(sceneInstanceID);
    if (!found)
        return;

    SceneTemplate sceneTemplate = *found;

    // Must be done before removing aura
    RemoveSceneInstanceId(sceneInstanceID);

    if (sceneTemplate.SceneId != 0)
        RemoveAurasDueToSceneId(sceneTemplate.SceneId);

    sScriptMgr->OnSceneComplete(GetPlayer(), sceneInstanceID, &sceneTemplate);

    if (sceneTemplate.PlaybackFlags & SCENEFLAG_CANCEL_AT_END)
        CancelScene(sceneInstanceID, false);
}

bool SceneMgr::HasScene(uint32 sceneInstanceID, uint32 sceneScriptPackageId /*= 0*/) const
{
    SceneTemplate const* sceneTemplate = GetSceneTemplateFromInstanceId(sceneInstanceID);
    return sceneTemplate && (!sceneScriptPackageId || sceneScriptPackageId == sceneTemplate->ScenePackageId);
}

void SceneMgr::AddInstanceIdToSceneMap(uint32 sceneInstanceID, SceneTemplate const* sceneTemplate)
{
    _scenesByInstance[sceneInstanceID] = *sceneTemplate;
}

void SceneMgr::CancelSceneByPackageId(uint32 sceneScriptPackageId)
{
    std::vector<uint32> instancesIds;

    for (SceneTemplateByInstance::const_iterator itr = _scenesByInstance.begin(); itr != _scenesByInstance.end(); ++itr)
        if (itr->second.ScenePackageId == sceneScriptPackageId)
            instancesIds.push_back(itr->first);

    for (uint32 sceneInstanceID : instancesIds)
        CancelScene(sceneInstanceID);
}

void SceneMgr::RemoveSceneInstanceId(uint32 sceneInstanceID)
{
    _scenesByInstance.erase(sceneInstanceID);
}

void SceneMgr::RemoveAurasDueToSceneId(uint32 sceneId)
{
    Unit::AuraEffectList const& scenePlayAuras = GetPlayer()->GetAuraEffectsByType(SPELL_AURA_PLAY_SCENE);
    for (Unit::AuraEffectList::const_iterator itr = scenePlayAuras.begin(); itr != scenePlayAuras.end(); ++itr)
    {
        if (uint32((*itr)->GetMiscValue()) == sceneId)
        {
            GetPlayer()->RemoveAura((*itr)->GetBase());
            break;
        }
    }
}

SceneTemplate const* SceneMgr::GetSceneTemplateFromInstanceId(uint32 sceneInstanceID) const
{
    SceneTemplateByInstance::const_iterator itr = _scenesByInstance.find(sceneInstanceID);
    if (itr != _scenesByInstance.end())
        return &itr->second;

    return nullptr;
}

uint32 SceneMgr::GetActiveSceneCount(uint32 sceneScriptPackageId /*= 0*/) const
{
    uint32 activeSceneCount = 0;

    for (SceneTemplateByInstance::const_iterator itr = _scenesByInstance.begin(); itr != _scenesByInstance.end(); ++itr)
        if (!sceneScriptPackageId || itr->second.ScenePackageId == sceneScriptPackageId)
            ++activeSceneCount;

    return activeSceneCount;
}
