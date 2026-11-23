/*
* This file is part of Project SkyFire https://www.projectskyfire.org.
* See LICENSE.md file for Copyright information
*/

#include "ScriptMgr.h"
#include "SpellScript.h"
#include "SpellAuraEffects.h"
#include "SpellMgr.h"
#include "Player.h"
#include "Creature.h"
#include "PassiveAI.h"
#include "Vehicle.h"

namespace
{
    uint8 const FullServing = 5;

    struct Dish
    {
        uint32 Chair, Server, Serving, CanEat, Feast, Helpings, WellFed, Pass, Credit, FoodFight, Plate;
    };

    Dish const Dishes[] =
    {
        {34812, 61796, 61807, 61801, 61784, 61842, 65414, 66250, 66373, 61928, 61835},
        {34823, 61793, 61804, 61798, 61785, 61841, 65412, 66261, 66372, 61925, 61833},
        {34819, 61795, 61806, 61800, 61788, 61843, 65416, 66259, 66375, 61927, 61836},
        {34824, 61797, 61808, 61802, 61786, 61844, 65410, 66262, 66376, 61929, 61837},
        {34822, 61794, 61805, 61799, 61787, 61845, 65415, 66260, 66374, 61926, 61838}
    };

    Dish const* FindDish(uint32 spell)
    {
        for (Dish const& dish : Dishes)
            if (spell == dish.Feast || spell == dish.Pass || spell == dish.Serving || spell == dish.Server)
                return &dish;
        return nullptr;
    }

    bool ValidateDishes()
    {
        for (Dish const& dish : Dishes)
            for (uint32 spell : {dish.Server, dish.Serving, dish.CanEat, dish.Feast, dish.Helpings,
                dish.WellFed, dish.Pass, dish.Credit, dish.FoodFight, dish.Plate})
                if (!sSpellMgr->GetSpellInfo(spell))
                    return false;
        return sSpellMgr->GetSpellInfo(61849) && sSpellMgr->GetSpellInfo(61826);
    }

    Unit* GetTable(Unit* chair)
    {
        // Dining chairs are independent vehicles. Boarding a vehicle accessory
        // makes the client place the diner relative to the table instead.
        return chair ? chair->FindNearestCreature(32823, 6.0f) : nullptr;
    }

    Unit* GetChair(Unit* unit)
    {
        if (!unit)
            return nullptr;
        if (unit->ToPlayer())
            unit = unit->GetVehicleBase();
        if (!unit || !unit->GetVehicleKit())
            return nullptr;
        for (Dish const& dish : Dishes)
            if (unit->GetEntry() == dish.Chair)
                if (GetTable(unit))
                    return unit;
        return nullptr;
    }

    Player* GetDiner(Unit* chair)
    {
        Unit* passenger = chair->GetVehicleKit()->GetPassenger(0);
        return passenger ? passenger->ToPlayer() : nullptr;
    }

    Unit* GetPlate(Unit* chair)
    {
        Unit* table = GetTable(chair);
        if (table && table->GetVehicleKit())
            if (Unit* holder = table->GetVehicleKit()->GetPassenger(6))
                if (holder->GetVehicleKit())
                    for (uint8 seat = 0; seat < 5; ++seat)
                        if (Dishes[seat].Chair == chair->GetEntry())
                            return holder->GetVehicleKit()->GetPassenger(seat);
        return nullptr;
    }
}

// Vehicle abilities are cast by the chair. Never grant credit without a seated
// player, and never pass food between unrelated tables or back to the caster.
class spell_pilgrims_table_action : public SpellScriptLoader
{
public:
    spell_pilgrims_table_action() : SpellScriptLoader("spell_pilgrims_table_action") { }
    class Action : public SpellScript
    {
        PrepareSpellScript(Action);
        bool Validate(SpellInfo const* info) override { return FindDish(info->Id) && ValidateDishes(); }
        SpellCastResult CheckCast()
        {
            Dish const* dish = FindDish(GetSpellInfo()->Id);
            Unit* chair = GetChair(GetCaster());
            if (!dish || !chair || !GetDiner(chair))
                return SPELL_FAILED_CASTER_AURASTATE;

            if (GetSpellInfo()->Id == dish->Pass)
            {
                Unit* recipient = GetChair(GetExplTargetUnit());
                if (!recipient || recipient == chair || GetTable(recipient) != GetTable(chair)
                    || chair->GetEntry() != dish->Chair)
                    return SPELL_FAILED_BAD_TARGETS;
            }
            else if (!chair->HasAura(dish->Server) && !chair->HasAura(dish->Serving))
                return SPELL_FAILED_CASTER_AURASTATE;

            return SPELL_CAST_OK;
        }
        void Handle(SpellEffIndex)
        {
            Dish const* dish = FindDish(GetSpellInfo()->Id);
            Unit* chair = GetChair(GetCaster());
            if (!dish || !chair)
                return;
            Player* diner = GetDiner(chair);
            if (!diner)
                return;

            if (GetSpellInfo()->Id == dish->Pass)
            {
                Unit* recipient = GetChair(GetHitUnit());
                if (!recipient || recipient == chair || GetTable(recipient) != GetTable(chair)
                    || chair->GetEntry() != dish->Chair)
                    return;
                // A pass refills the plate with five helpings. Only an already
                // full plate bounces food, regardless of whether the player
                // selected the chair or its passenger.
                Aura* previous = recipient->GetAura(dish->Serving);
                bool full = recipient->HasAura(dish->Server)
                    || (previous && previous->GetStackAmount() >= FullServing);
                recipient->CastSpell(recipient, dish->Serving, true);
                Aura* serving = recipient->GetAura(dish->Serving);
                if (!serving)
                    return;
                serving->SetStackAmount(FullServing);
                diner->CastSpell(diner, dish->Credit, true);
                if (full)
                    if (Player* target = GetDiner(recipient))
                        diner->CastSpell(target, dish->FoodFight, true);
                return;
            }

            if (!chair->HasAura(dish->Server) && !chair->HasAura(dish->Serving))
                return;
            diner->CastSpell(diner, 61826, true);
            diner->CastSpell(diner, dish->Helpings, true);
            if (Aura* helpings = diner->GetAura(dish->Helpings))
                if (helpings->GetStackAmount() >= FullServing)
                    diner->CastSpell(diner, dish->WellFed, true);
            if (!chair->HasAura(dish->Server))
                if (Aura* serving = chair->GetAura(dish->Serving))
                    serving->ModStackAmount(-1);

            for (Dish const& food : Dishes)
            {
                Aura* helpings = diner->GetAura(food.Helpings);
                if (!helpings || helpings->GetStackAmount() < FullServing)
                    return;
            }
            diner->CastSpell(diner, 61849, true);
            for (Dish const& food : Dishes)
                diner->RemoveAurasDueToSpell(food.Helpings);
        }
        void Register() override
        {
            OnCheckCast += SpellCheckCastFn(Action::CheckCast);
            OnEffectHitTarget += SpellEffectFn(Action::Handle, EFFECT_0, SPELL_EFFECT_DUMMY);
        }
    };
    SpellScript* GetSpellScript() const override { return new Action(); }
};

// The permanent server aura supplies the chair's own food; received servings
// supply the other dishes. Removing the last serving must not remove the server.
class spell_pilgrims_serving : public SpellScriptLoader
{
public:
    spell_pilgrims_serving() : SpellScriptLoader("spell_pilgrims_serving") { }
    class Serving : public AuraScript
    {
        PrepareAuraScript(Serving);
        bool Validate(SpellInfo const* info) override { return FindDish(info->Id) && ValidateDishes(); }
        void Apply(AuraEffect const*, AuraEffectHandleModes)
        {
            Dish const* dish = FindDish(GetSpellInfo()->Id);
            GetTarget()->CastSpell(GetTarget(), dish->CanEat, true);
            if (Unit* chair = GetChair(GetTarget()))
                if (Unit* plate = GetPlate(chair))
                    chair->CastSpell(plate, dish->Plate, true);
        }
        void Remove(AuraEffect const*, AuraEffectHandleModes)
        {
            Dish const* dish = FindDish(GetSpellInfo()->Id);
            if (GetTarget()->HasAura(dish->Server) || GetTarget()->HasAura(dish->Serving))
                return;
            GetTarget()->RemoveAurasDueToSpell(dish->CanEat);
            if (Unit* chair = GetChair(GetTarget()))
                if (Unit* plate = GetPlate(chair))
                    plate->RemoveAurasDueToSpell(dish->Plate);
        }
        void Register() override
        {
            AfterEffectApply += AuraEffectApplyFn(Serving::Apply, EFFECT_0, SPELL_AURA_DUMMY, AURA_EFFECT_HANDLE_REAL);
            AfterEffectRemove += AuraEffectRemoveFn(Serving::Remove, EFFECT_0, SPELL_AURA_DUMMY, AURA_EFFECT_HANDLE_REAL);
        }
    };
    AuraScript* GetAuraScript() const override { return new Serving(); }
};

class npc_pilgrims_wild_turkey : public CreatureScript
{
public:
    npc_pilgrims_wild_turkey() : CreatureScript("npc_pilgrims_wild_turkey") { }
    struct TurkeyAI : public CritterAI
    {
        explicit TurkeyAI(Creature* creature) : CritterAI(creature) { }
        void JustDied(Unit* killer) override
        {
            // Turkey Caller summons must not advance The Turkinator.
            if (!me->GetDBTableGUIDLow() || !killer)
                return;
            if (Player* player = killer->GetCharmerOrOwnerPlayerOrPlayerItself())
            {
                me->CastSpell(player, 62014, true);
                if (Aura* tracker = player->GetAura(62014))
                {
                    uint8 stacks = tracker->GetStackAmount();
                    if (stacks == 10 || stacks == 20 || stacks == 30)
                        player->CastSpell(player, 62015, true);
                    if (stacks >= 40)
                    {
                        player->CastSpell(player, 62021, true);
                        player->RemoveAurasDueToSpell(62014);
                    }
                }
            }
        }
    };
    CreatureAI* GetAI(Creature* creature) const override { return new TurkeyAI(creature); }
};

void AddSC_event_pilgrims_bounty()
{
    new spell_pilgrims_table_action();
    new spell_pilgrims_serving();
    new npc_pilgrims_wild_turkey();
}
