/*
* This file is part of Project SkyFire https://www.projectskyfire.org.
* See LICENSE.md file for Copyright information
*/

#include "CellImpl.h"
#include "Common.h"
#include "DynamicObject.h"
#include "GridNotifiers.h"
#include "GridNotifiersImpl.h"
#include "Log.h"
#include "ObjectAccessor.h"
#include "ObjectMgr.h"
#include "Opcodes.h"
#include "Player.h"
#include "ScriptMgr.h"
#include "Spell.h"
#include "SpellAuraEffects.h"
#include "SpellMgr.h"
#include "SpellScript.h"
#include "Unit.h"
#include "Util.h"
#include "Vehicle.h"
#include "WorldPacket.h"

void AuraApplication::_Remove()
{
    uint8 slot = GetSlot();

    if (slot >= MAX_AURAS)
        return;

    if (AuraApplication* foundAura = _target->GetAuraApplication(GetBase()->GetId(), GetBase()->GetCasterGUID(), GetBase()->GetCastItemGUID()))
    {
        // Reuse visible aura slot by aura which is still applied - prevent storing dead pointers
        if (slot == foundAura->GetSlot())
        {
            if (GetTarget()->GetVisibleAura(slot) == this)
            {
                GetTarget()->SetVisibleAura(slot, foundAura);
                foundAura->SetNeedClientUpdate();
            }
            // set not valid slot for aura - prevent removing other visible aura
            slot = MAX_AURAS;
        }
    }

    // update for out of range group members
    if (slot < MAX_AURAS)
    {
        GetTarget()->RemoveVisibleAura(slot);
        ClientUpdate(true);
    }
}

void AuraApplication::_InitFlags(Unit* caster, uint32 effMask)
{
    // mark as selfcasted if needed
    _flags |= (GetBase()->GetCasterGUID() == GetTarget()->GetGUID()) ? AFLAG_CASTER : AFLAG_NONE;

    // aura is casted by self or an enemy
    // one negative effect and we know aura is negative
    if (IsSelfcasted() || !caster || !caster->IsFriendlyTo(GetTarget()))
    {
        bool negativeFound = false;
        for (uint32 i = 0; i < MAX_SPELL_EFFECTS; ++i)
        {
            if (((1 << i) & effMask) && !GetBase()->GetSpellInfo()->IsPositiveEffect(i))
            {
                negativeFound = true;
                break;
            }
        }
        _flags |= negativeFound ? AFLAG_NEGATIVE : AFLAG_POSITIVE;
    }
    // aura is casted by friend
    // one positive effect and we know aura is positive
    else
    {
        bool positiveFound = false;
        for (uint32 i = 0; i < MAX_SPELL_EFFECTS; ++i)
        {
            if (((1 << i) & effMask) && GetBase()->GetSpellInfo()->IsPositiveEffect(i))
            {
                positiveFound = true;
                break;
            }
        }
        _flags |= positiveFound ? AFLAG_POSITIVE : AFLAG_NEGATIVE;
    }

    if (GetBase()->GetSpellInfo()->AttributesEx8 & SPELL_ATTR8_AURA_SEND_AMOUNT)
        _flags |= AFLAG_ANY_EFFECT_AMOUNT_SENT;
}

void AuraApplication::_HandleEffect(uint8 effIndex, bool apply)
{
    AuraEffect* aurEff = GetBase()->GetEffect(effIndex);
    ASSERT(aurEff);
    ASSERT(HasEffect(effIndex) == (!apply));
    ASSERT((1 << effIndex) & _effectsToApply);
    SF_LOG_DEBUG("spells", "AuraApplication::_HandleEffect: %u, apply: %u: amount: %u", aurEff->GetAuraType(), apply, aurEff->GetAmount());

    if (apply)
    {
        ASSERT(!(_effMask & (1 << effIndex)));
        _effMask |= 1 << effIndex;
        aurEff->HandleEffect(this, AURA_EFFECT_HANDLE_REAL, true);
    }
    else
    {
        ASSERT(_effMask & (1 << effIndex));
        _effMask &= ~(1 << effIndex);
        aurEff->HandleEffect(this, AURA_EFFECT_HANDLE_REAL, false);

        // Remove all triggered by aura spells vs unlimited duration
        aurEff->CleanupTriggeredSpells(GetTarget());
    }
    SetNeedClientUpdate();
}

void AuraApplication::ClientUpdate(bool remove)
{
    _needClientUpdate = false;

    ObjectGuid targetGuid = _target->GetGUID();
    Aura const* aura = GetBase();
    uint32 flags = _flags;

    if (aura->GetMaxDuration() > 0 && !(aura->GetSpellInfo()->AttributesEx5 & SPELL_ATTR5_HIDE_DURATION))
        flags |= AFLAG_DURATION;

    WorldPacket data(SMSG_AURA_UPDATE);
    data.WriteBit(targetGuid[7]);
    data.WriteBit(0);                                   // Is AURA_UPDATE_ALL
    data.WriteBits(1, 24);                              // Aura Count
    data.WriteBit(targetGuid[6]);
    data.WriteBit(targetGuid[1]);
    data.WriteBit(targetGuid[3]);
    data.WriteBit(targetGuid[0]);
    data.WriteBit(targetGuid[4]);
    data.WriteBit(targetGuid[2]);
    data.WriteBit(targetGuid[5]);
    data.WriteBit(!remove);                             // Not remove

    // Client indexes $wN by effect index. Count must be (highest effect index + 1) with
    // zero padding for gaps — not the number of active effects (e.g. ER $w2 with only EFFECT_1).
    uint8 effCount = 0;
    if (!remove)
    {
        if (flags & AFLAG_ANY_EFFECT_AMOUNT_SENT)
        {
            for (uint32 i = 0; i < MAX_SPELL_EFFECTS; ++i)
                if (HasEffect(i))
                    effCount = i + 1;

            data.WriteBits(effCount, 22);               // Effect Count
        }
        else
            data.WriteBits(0, 22);                      // Effect Count

        data.WriteBit(!(flags & AFLAG_CASTER));         // HasCasterGuid

        if (!(flags & AFLAG_CASTER))
        {
            ObjectGuid casterGuid = aura->GetCasterGUID();
            data.WriteBit(casterGuid[3]);
            data.WriteBit(casterGuid[4]);
            data.WriteBit(casterGuid[6]);
            data.WriteBit(casterGuid[1]);
            data.WriteBit(casterGuid[5]);
            data.WriteBit(casterGuid[2]);
            data.WriteBit(casterGuid[0]);
            data.WriteBit(casterGuid[7]);
        }

        data.WriteBits(0, 22);                          // Unk effect count
        data.WriteBit(flags & AFLAG_DURATION);          // HasDuration
        data.WriteBit(flags & AFLAG_DURATION);          // HasMaxDuration
    }

    data.FlushBits();

    if (!remove)
    {
        if (!(flags & AFLAG_CASTER))
        {
            ObjectGuid casterGuid = aura->GetCasterGUID();
            data.WriteByteSeq(casterGuid[3]);
            data.WriteByteSeq(casterGuid[2]);
            data.WriteByteSeq(casterGuid[1]);
            data.WriteByteSeq(casterGuid[6]);
            data.WriteByteSeq(casterGuid[4]);
            data.WriteByteSeq(casterGuid[0]);
            data.WriteByteSeq(casterGuid[5]);
            data.WriteByteSeq(casterGuid[7]);
        }

        data << uint8(flags);
        data << uint16(aura->GetCasterLevel());
        data << uint32(aura->GetId());

        if (flags & AFLAG_DURATION)
            data << uint32(aura->GetMaxDuration());

        if (flags & AFLAG_DURATION)
            data << uint32(aura->GetDuration());

        // send stack amount for aura which could be stacked (never 0 - causes incorrect display) or charges
        // stack amount has priority over charges (checked on retail with spell 50262)
        data << uint8(aura->GetSpellInfo()->StackAmount ? aura->GetStackAmount() : aura->GetCharges());
        data << uint32(GetEffectMask());

        if (flags & AFLAG_ANY_EFFECT_AMOUNT_SENT)
        {
            for (uint32 i = 0; i < effCount; ++i)
            {
                float amount = 0.0f;
                if (HasEffect(i))
                    if (AuraEffect const* eff = aura->GetEffect(i))
                        amount = float(eff->GetAmount());
                data << float(amount);
            }
        }
    }

    data << uint8(GetSlot());

    data.WriteByteSeq(targetGuid[2]);
    data.WriteByteSeq(targetGuid[6]);
    data.WriteByteSeq(targetGuid[7]);
    data.WriteByteSeq(targetGuid[1]);
    data.WriteByteSeq(targetGuid[3]);
    data.WriteByteSeq(targetGuid[4]);
    data.WriteByteSeq(targetGuid[0]);
    data.WriteByteSeq(targetGuid[5]);

    _target->SendMessageToSet(&data, true);
}
