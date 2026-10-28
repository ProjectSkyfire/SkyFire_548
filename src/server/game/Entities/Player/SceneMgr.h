/*
* This file is part of Project SkyFire https://www.projectskyfire.org.
* See LICENSE.md file for Copyright information
*/

#ifndef SKYFIRE_SCENEMGR_H
#define SKYFIRE_SCENEMGR_H

#include "Common.h"
#include "ObjectMgr.h"

enum SceneFlags
{
    SCENEFLAG_UNK1              = 0x01,
    SCENEFLAG_CANCEL_AT_END     = 0x02,
    SCENEFLAG_NOT_CANCELABLE    = 0x04,
    SCENEFLAG_UNK8              = 0x08,
    SCENEFLAG_UNK16             = 0x10,
};

class Player;
struct Position;

typedef std::map<uint32, SceneTemplate> SceneTemplateByInstance;

class SceneMgr
{
public:
    explicit SceneMgr(Player* player);

    Player* GetPlayer() const { return _player; }

    uint32 PlayScene(uint32 sceneId, Position const* position = nullptr);
    uint32 PlaySceneByTemplate(SceneTemplate const* sceneTemplate, Position const* position = nullptr);
    uint32 PlaySceneByPackageId(uint32 sceneScriptPackageId, uint32 playbackflags = SCENEFLAG_UNK16, Position const* position = nullptr);
    void CancelScene(uint32 sceneInstanceID, bool removeFromMap = true);

    void OnSceneTrigger(uint32 sceneInstanceID, std::string const& triggerName);
    void OnSceneCancel(uint32 sceneInstanceID);
    void OnSceneComplete(uint32 sceneInstanceID);

    void RecreateScene(uint32 sceneScriptPackageId, uint32 playbackflags = SCENEFLAG_UNK16, Position const* position = nullptr)
    {
        CancelSceneByPackageId(sceneScriptPackageId);
        PlaySceneByPackageId(sceneScriptPackageId, playbackflags, position);
    }

    bool HasScene(uint32 sceneInstanceID, uint32 sceneScriptPackageId = 0) const;

    void AddInstanceIdToSceneMap(uint32 sceneInstanceID, SceneTemplate const* sceneTemplate);
    void CancelSceneByPackageId(uint32 sceneScriptPackageId);
    void RemoveSceneInstanceId(uint32 sceneInstanceID);
    void RemoveAurasDueToSceneId(uint32 sceneId);

    SceneTemplate const* GetSceneTemplateFromInstanceId(uint32 sceneInstanceID) const;
    uint32 GetActiveSceneCount(uint32 sceneScriptPackageId = 0) const;
    SceneTemplateByInstance const& GetSceneTemplateByInstanceMap() const { return _scenesByInstance; }

    uint32 GetNewStandaloneSceneInstanceID() { return ++_standaloneSceneInstanceID; }

private:
    Player* _player;
    SceneTemplateByInstance _scenesByInstance;
    uint32 _standaloneSceneInstanceID;
};

#endif
