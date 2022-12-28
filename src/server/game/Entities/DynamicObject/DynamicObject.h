/*
* This file is part of Project SkyFire https://www.projectskyfire.org. 
* See LICENSE.md file for Copyright information
*/

#ifndef SKYFIRESERVER_DYNAMICOBJECT_H
#define SKYFIRESERVER_DYNAMICOBJECT_H

#include "Object.h"

class Unit;
class Aura;
class SpellInfo;

enum DynamicObjectType
{
    DYNAMIC_OBJECT_PORTAL           = 0x0,      // unused
    DYNAMIC_OBJECT_AREA_SPELL       = 0x1,
    DYNAMIC_OBJECT_FARSIGHT_FOCUS   = 0x2
};

class DynamicObject : public WorldObject, public GridObject<DynamicObject>
{
    public:
        DynamicObject(bool isWorldObject);
        ~DynamicObject();

        void AddToWorld() override;
        void RemoveFromWorld() override;

        bool CreateDynamicObject(uint32 guidlow, Unit* caster, SpellInfo const* spell, Position const& pos, float radius, DynamicObjectType type);
        void Update(uint32 p_time) override;
        void Remove();
        void SetDuration(int32 newDuration);
        int32 GetDuration() const;
        void Delay(int32 delaytime);
        void SetAura(Aura* aura);
        void RemoveAura();
        void SetCasterViewpoint();
        void RemoveCasterViewpoint();
        Unit* GetCaster() const { return _caster; }
        void BindToCaster();
        void UnbindFromCaster();
        uint32 GetSpellId() const {  return GetUInt32Value(DYNAMICOBJECT_FIELD_SPELL_ID); }
        uint64 GetCasterGUID() const { return GetUInt64Value(DYNAMICOBJECT_FIELD_CASTER); }
        float GetRadius() const { return GetFloatValue(DYNAMICOBJECT_FIELD_RADIUS); }

    protected:
        Aura* _aura;
        Aura* _removedAura;
        Unit* _caster;
        int32 _duration; // for non-aura dynobjects
        bool _isViewpoint;
};
#endif
