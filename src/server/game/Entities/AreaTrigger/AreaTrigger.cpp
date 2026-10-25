/*
* This file is part of Project SkyFire https://www.projectskyfire.org.
* See LICENSE.md file for Copyright information
*/

#include "AreaTrigger.h"
#include "CellImpl.h"
#include "GridNotifiers.h"
#include "GridNotifiersImpl.h"
#include "Log.h"
#include "ObjectAccessor.h"
#include "ObjectMgr.h"
#include "Player.h"
#include "ScriptMgr.h"
#include "SpellAuraEffects.h"
#include "SpellInfo.h"
#include "SpellMgr.h"
#include "Unit.h"

class IAreaTriggerOnceChecker
{
public:
    explicit IAreaTriggerOnceChecker(IAreaTriggerOnce* areaTrigger) : m_areaTrigger(areaTrigger) { }

    bool operator()(WorldObject* object)
    {
        if (m_areaTrigger->GetTarget()->GetExactDist2d(object) > m_areaTrigger->GetRange() + object->GetObjectSize())
            return false;
        if (std::abs(object->GetPositionZ() - m_areaTrigger->GetTarget()->GetPositionZ()) > 10.0f)
            return false;

        return m_areaTrigger->CheckTriggering(object);
    }

private:
    IAreaTriggerOnce* m_areaTrigger;
};

class IAreaTriggerAuraUpdater
{
public:
    explicit IAreaTriggerAuraUpdater(IAreaTriggerAura* areaTrigger) : m_areaTrigger(areaTrigger) { }

    void operator()(WorldObject* object) const
    {
        if (!m_areaTrigger->GetAreaTrigger()->IsInWorld())
            return;

        if (!object->IsInWorld())
            return;

        if (m_areaTrigger->GetTarget()->GetExactDist2d(object) > m_areaTrigger->GetRange() + object->GetObjectSize())
            return;
        if (std::abs(object->GetPositionZ() - m_areaTrigger->GetTarget()->GetPositionZ()) > 10.0f)
            return;

        if (!m_areaTrigger->CheckTriggering(object))
            return;

        for (IAreaTriggerAura::TriggeringList::iterator iter = m_areaTrigger->m_triggerings.begin(); iter != m_areaTrigger->m_triggerings.end(); ++iter)
        {
            if (iter->Guid == object->GetGUID())
            {
                iter->UpdateNumber = m_areaTrigger->m_updateNumber;
                m_areaTrigger->OnTriggeringUpdate(object);
                return;
            }
        }

        IAreaTriggerAura::Triggering triggering;
        triggering.Guid = object->GetGUID();
        triggering.UpdateNumber = m_areaTrigger->m_updateNumber;
        m_areaTrigger->m_triggerings.push_back(triggering);
        m_areaTrigger->OnTriggeringApply(object);
    }

private:
    IAreaTriggerAura* m_areaTrigger;
};

void IAreaTrigger::Initialize(AreaTrigger* trigger, Unit* caster, WorldObject* target)
{
    m_caster = caster;
    m_target = target;
    m_spellInfo = sSpellMgr->GetSpellInfo(trigger->GetSpellId());
    m_location = m_target;
    m_areaTrigger = trigger;

    if (AreaTriggerTemplate const* atTemplate = sObjectMgr->GetAreaTriggerTemplate(trigger->GetEntry()))
        m_range = atTemplate->Radius;

    OnInit();
}

void IAreaTrigger::OnRemoveFromWorld()
{
    m_duringRemoveFromWorld = true;
    OnDestroy();
}

void IAreaTriggerOnce::OnUpdate(uint32 /*diff*/)
{
    WorldObject* triggering = NULL;
    IAreaTriggerOnceChecker checker(this);
    Skyfire::WorldObjectSearcher<IAreaTriggerOnceChecker> searcher(m_target, triggering, checker, m_mapTypeMask);
    m_target->VisitNearbyObject(m_range + 10.0f, searcher);

    if (triggering)
    {
        OnTrigger(triggering);
        GetAreaTrigger()->Remove();
    }
}

void IAreaTriggerOnce::OnRemoveFromWorld()
{
    IAreaTrigger::OnRemoveFromWorld();
}

IAreaTriggerAura::IAreaTriggerAura() : m_mapTypeMask(GRID_MAP_TYPE_MASK_CREATURE | GRID_MAP_TYPE_MASK_PLAYER) { }

IAreaTriggerAura::~IAreaTriggerAura() { }

void IAreaTriggerAura::OnUpdate(uint32 /*diff*/)
{
    ++m_updateNumber;

    IAreaTriggerAuraUpdater updater(this);
    Skyfire::WorldObjectWorker<IAreaTriggerAuraUpdater> worker(m_target, updater, m_mapTypeMask);
    m_target->VisitNearbyObject(m_range + 10.0f, worker);

    for (TriggeringList::iterator iter = m_triggerings.begin(); iter != m_triggerings.end();)
    {
        if (iter->UpdateNumber != m_updateNumber)
        {
            if (WorldObject* object = ObjectAccessor::GetWorldObject(*m_target, iter->Guid))
                OnTriggeringRemove(object);

            iter = m_triggerings.erase(iter);
        }
        else
            ++iter;
    }
}

void IAreaTriggerAura::OnRemoveFromWorld()
{
    m_duringRemoveFromWorld = true;

    for (TriggeringList::const_iterator itr = m_triggerings.begin(); itr != m_triggerings.end(); ++itr)
        if (WorldObject* object = ObjectAccessor::GetWorldObject(*m_target, itr->Guid))
            OnTriggeringRemove(object);
    m_triggerings.clear();

    OnDestroy();
}

AreaTrigger::AreaTrigger() : AreaTrigger(nullptr) { }

AreaTrigger::AreaTrigger(AuraEffect const* eff) : WorldObject(false), _duration(0), _caster(nullptr), _effectTarget(nullptr), _effectTargetGuid(0),
    _auraOwner(eff ? eff->GetBase()->GetOwner()->GetGUID() : 0), _effectIndex(eff ? eff->GetEffIndex() : 0), _scaleX(0.0f), _scaleY(0.0f)
{
    m_objectType |= TYPEMASK_AREATRIGGER;
    m_objectTypeId = TypeID::TYPEID_AREATRIGGER;

    m_updateFlag = UPDATEFLAG_STATIONARY_POSITION;

    m_valuesCount = AREATRIGGER_END;
}

AreaTrigger::~AreaTrigger() { }

void AreaTrigger::AddToWorld()
{
    ///- Register the AreaTrigger for guid lookup and for caster
    if (!IsInWorld())
    {
        sObjectAccessor->AddObject(this);
        WorldObject::AddToWorld();
        BindToCaster();

        if (IAreaTrigger* script = sScriptMgr->CreateAreaTriggerInterface(GetEntry()))
        {
            _script.reset(script);
            _script->Initialize(this, _caster, _effectTarget);
        }
    }
}

void AreaTrigger::RemoveFromWorld()
{
    ///- Remove the AreaTrigger from the accessor and from all lists of objects in world
    if (IsInWorld())
    {
        if (_script)
            _script->OnRemoveFromWorld();

        UnbindFromCaster();
        WorldObject::RemoveFromWorld();
        sObjectAccessor->RemoveObject(this);
    }
}

bool AreaTrigger::CreateAreaTrigger(uint32 guidlow, uint32 triggerEntry, Unit* caster, SpellInfo const* spell, Position const& pos, Unit* effectTarget)
{
    SetMap(caster->GetMap());
    Relocate(pos);
    if (!IsPositionValid())
    {
        SF_LOG_ERROR("sql.sql", "AreaTrigger (spell %u) not created. Invalid coordinates (X: %f Y: %f)", spell->Id, GetPositionX(), GetPositionY());
        return false;
    }

    WorldObject::_Create(guidlow, HIGHGUID_AREATRIGGER, caster->GetPhaseMask());

    int32 duration = spell->GetDuration();
    if (Player* modOwner = caster->GetSpellModOwner())
        modOwner->ApplySpellMod(spell->Id, SPELLMOD_DURATION, duration);

    SetEntry(triggerEntry);
    SetDuration(duration);
    SetObjectScale(1.0f);

    SetUInt64Value(AREATRIGGER_FIELD_CASTER, caster->GetGUID());
    SetInt32Value(AREATRIGGER_FIELD_DURATION, duration);
    SetUInt32Value(AREATRIGGER_FIELD_SPELL_ID, spell->Id);
    SetUInt32Value(AREATRIGGER_FIELD_SPELL_VISUAL_ID, spell->SpellVisual[0]);
    SetFloatValue(AREATRIGGER_FIELD_EXPLICIT_SCALE, 1.0f);

    if (AreaTriggerTemplate const* atTemplate = sObjectMgr->GetAreaTriggerTemplate(triggerEntry))
    {
        _scaleX = atTemplate->ScaleX;
        _scaleY = atTemplate->ScaleY;
    }

    _effectTarget = effectTarget ? static_cast<WorldObject*>(effectTarget) : static_cast<WorldObject*>(this);
    _effectTargetGuid = effectTarget ? effectTarget->GetGUID() : 0;

    if (!GetMap()->AddToMap(this))
        return false;

    return true;
}

void AreaTrigger::Expire()
{
    if (_script)
        _script->OnExpire();

    Remove();
}

void AreaTrigger::SetRadius(float radius)
{
    if (_script)
        _script->SetRange(radius);
}

void AreaTrigger::Update(uint32 p_time)
{
    if (_effectTargetGuid)
    {
        Unit* target = ObjectAccessor::GetUnit(*this, _effectTargetGuid);
        if (!target || target != _effectTarget || !target->IsInWorld())
        {
            Remove();
            return;
        }
    }

    if (GetMaxDuration() != -1)
    {
        if (GetDuration() > int32(p_time))
            _duration -= p_time;
        else
        {
            Expire();
            return;
        }
    }

    WorldObject::Update(p_time);

    if (_script && IsInWorld())
        _script->OnUpdate(p_time);

    if (IsInWorld() && !GetCaster())
        Remove();
}

void AreaTrigger::Remove()
{
    if (!IsInWorld())
        return;

    uint64 auraOwnerGuid = _auraOwner;
    uint64 casterGuid = GetCasterGUID();

    SendObjectDeSpawnAnim(GetGUID());
    RemoveFromWorld();
    AddObjectToRemoveList();

    if (auraOwnerGuid)
        if (Unit* auraOwner = ObjectAccessor::GetUnit(*this, auraOwnerGuid))
            if (AuraEffect* effect = auraOwner->GetAuraEffect(GetSpellId(), _effectIndex, casterGuid))
                effect->GetBase()->Remove();
}

void AreaTrigger::BindToCaster()
{
    _caster = ObjectAccessor::GetUnit(*this, GetCasterGUID());
    if (_caster)
        _caster->RegisterAreaTrigger(this);
}

void AreaTrigger::UnbindFromCaster()
{
    if (!_caster)
        return;

    _caster->UnregisterAreaTrigger(this);
    _caster = nullptr;
}
