/*
* This file is part of Project SkyFire https://www.projectskyfire.org.
* See LICENSE.md file for Copyright information
*/

#ifndef SKYFIRESERVER_AREATRIGGER_H
#define SKYFIRESERVER_AREATRIGGER_H

#include "Object.h"
#include <memory>

class Unit;
class SpellInfo;
class AuraEffect;
class AreaTrigger;

class IAreaTrigger
{
public:
    void Initialize(AreaTrigger* trigger, Unit* caster, WorldObject* target);

    IAreaTrigger() { }
    virtual ~IAreaTrigger() { }

    virtual void OnInit() { }
    virtual void OnUpdate(uint32 /*diff*/) { }
    virtual void OnExpire() { }
    virtual void OnDestroy() { }

    virtual void OnRemoveFromWorld();

    Unit* GetCaster() { return m_caster; }
    WorldLocation* GetLocation() { return m_location; }
    float GetRange() { return m_range; }
    void SetRange(float radius) { m_range = radius; }
    WorldObject* GetTarget() { return m_target; }
    SpellInfo const* GetSpellInfo() { return m_spellInfo; }
    AreaTrigger* GetAreaTrigger() const { return m_areaTrigger; }

public:
    float m_range = 0.0f;

protected:
    WorldLocation* m_location = nullptr;
    Unit* m_caster = nullptr;
    WorldObject* m_target = nullptr;
    SpellInfo const* m_spellInfo = nullptr;
    bool m_duringRemoveFromWorld = false;
    AreaTrigger* m_areaTrigger = nullptr;
};

// Fires OnTrigger for the first object that passes CheckTriggering, then removes the area trigger
class IAreaTriggerOnce : public IAreaTrigger
{
public:
    IAreaTriggerOnce() : m_mapTypeMask(GRID_MAP_TYPE_MASK_CREATURE | GRID_MAP_TYPE_MASK_PLAYER) { }

    void OnUpdate(uint32 diff) OVERRIDE;

    virtual bool CheckTriggering(WorldObject* triggering) = 0;
    virtual void OnTrigger(WorldObject* triggering) = 0;
    void OnRemoveFromWorld() final;

protected:
    uint32 m_mapTypeMask;
};

class IAreaTriggerAuraUpdater;

// Tracks the objects inside the area and reports enter / stay / leave
class IAreaTriggerAura : public IAreaTrigger
{
    friend class IAreaTriggerAuraUpdater;

public:
    IAreaTriggerAura();
    ~IAreaTriggerAura();

    void OnUpdate(uint32 diff) OVERRIDE;

    virtual bool CheckTriggering(WorldObject* triggering) = 0;
    virtual void OnTriggeringApply(WorldObject* /*triggering*/) { }
    virtual void OnTriggeringUpdate(WorldObject* /*triggering*/) { }
    virtual void OnTriggeringRemove(WorldObject* /*triggering*/) { }

    void OnRemoveFromWorld() final;

protected:
    uint32 m_mapTypeMask;

private:
    struct Triggering
    {
        uint32 UpdateNumber;
        uint64 Guid;
    };
    typedef std::list<Triggering> TriggeringList;

    TriggeringList m_triggerings;
    uint32 m_updateNumber = 0;
};

class AreaTrigger : public WorldObject, public GridObject<AreaTrigger>
{
public:
    AreaTrigger();
    explicit AreaTrigger(AuraEffect const* eff);
    ~AreaTrigger();

    void AddToWorld();
    void RemoveFromWorld();

    bool CreateAreaTrigger(uint32 guidlow, uint32 triggerEntry, Unit* caster, SpellInfo const* spell, Position const& pos, Unit* effectTarget = nullptr);
    void Update(uint32 p_time);
    void Remove();
    void Expire();
    void SetRadius(float radius);
    uint32 GetSpellId() const { return GetUInt32Value(AREATRIGGER_FIELD_SPELL_ID); }
    uint32 GetEffectIndex() const { return _effectIndex; }
    int32 GetMaxDuration() const { return GetInt32Value(AREATRIGGER_FIELD_DURATION); }
    int32 GetDuration() const { return _duration; }
    void SetDuration(int32 newDuration) { _duration = newDuration; }
    void Delay(int32 delaytime) { SetDuration(GetDuration() - delaytime); }
    Unit* GetCaster() const { return _caster; }
    uint64 GetCasterGUID() const { return GetUInt64Value(AREATRIGGER_FIELD_CASTER); }
    void BindToCaster();
    void UnbindFromCaster();

    float GetScaleX() const { return _scaleX; }
    float GetScaleY() const { return _scaleY; }
    void SetVisualRadius(float radius) { _scaleX = radius; _scaleY = radius; }
    uint32 GetMovementTime() const { return 0; }

protected:
    int32 _duration;
    Unit* _caster;
    WorldObject* _effectTarget;
    uint64 _effectTargetGuid;
    uint64 _auraOwner;
    uint32 _effectIndex;

    float _scaleX;
    float _scaleY;
    std::unique_ptr<IAreaTrigger> _script;
};
#endif
