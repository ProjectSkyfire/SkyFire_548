/*
* This file is part of the Pandaria 5.4.8 Project. See THANKS file for Copyright information
*
* This program is free software; you can redistribute it and/or modify it
* under the terms of the GNU General Public License as published by the
* Free Software Foundation; either version 2 of the License, or (at your
* option) any later version.
*
* This program is distributed in the hope that it will be useful, but WITHOUT
* ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
* FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for
* more details.
*
* You should have received a copy of the GNU General Public License along
* with this program. If not, see <http://www.gnu.org/licenses/>.
*/

#include "well_of_eternity.h"
#include "Group.h"

enum ScriptedTextMannoroth
{
    SAY_MANNOROTH_AGGRO     = 0,
    SAY_MANNOROTH_DEATH     = 1,
    SAY_MANNOROTH_SARGERAS  = 3,
    SAY_MANNOROTH_SWORD     = 4,
    SAY_MANNOROTH_VAROTHEN  = 5,
    SAY_MANNOROTH_EVENT     = 6,
    SAY_MANNOROTH_KILL      = 7,
    SAY_MANNOROTH_SPELL     = 8,
};
enum ScriptedTextVarothen
{
    SAY_VAROTHEN_AGGRO      = 0,
    SAY_VAROTHEN_DEATH      = 1,
    SAY_VAROTHEN_KILL       = 2,
};

enum Spells
{
    // Mannoroth
    SPELL_FEL_FIRESTORM             = 103888,
    SPELL_FEL_FIRESTORM_SUMMON      = 103889,
    SPELL_FEL_FLAMES_AURA           = 103892,
    SPELL_FEL_FLAMES_DMG            = 103891,
    SPELL_FELBLADE                  = 103966,
    SPELL_FELBURN                   = 103972,
    SPELL_EMBEDDED_BLADE_1          = 104820,
    SPELL_EMBEDDED_BLADE_2          = 109542,
    SPELL_EMBEDDED_BLADE_DUMMY      = 104823,
    SPELL_FEL_FIRE_NOVA             = 105093,
    SPELL_NETHER_PORTAL             = 104625,
    SPELL_NETHER_TEAR               = 105041,
    SPELL_FEL_DRAIN                 = 104961,
    SPELL_SUMMON_FELHOUND_AURA      = 105053,
    SPELL_SUMMON_FELHOUND           = 105054,
    SPELL_SUMMON_FELGUARD_AURA      = 105057,
    SPELL_SUMMON_FELGUARD           = 105058,
    SPELL_SUMMON_DEVASTATOR_AURA    = 105061,
    SPELL_SUMMON_DEVASTATOR         = 105059,
    SPELL_INFERNAL                  = 105141,
    SPELL_INFERNAL_MISSILE          = 105145,

    // Captain Varo'then
    SPELL_MAGNISTRIKE               = 103669,

    // Doomguard Debilitator
    SPELL_DEBILITATING_FLAY         = 104678,

    SPELL_COMPLETE_ENCOUNTER        = 105576,

    // Illidan 2
    SPELL_WATERS_OF_ETERNITY    = 103952,
    SPELL_TAUNT                 = 104461,
    SPELL_DEMON_RUSH            = 104205,
    SPELL_DARKLANCE             = 104394,
    SPELL_AURA_OF_IMMOLATION    = 104379,
    SPELL_GIFT_OF_SARGERAS      = 104998,
    SPELL_GIFT_OF_SARGERAS_AOE  = 105009,

    // Tyrande
    SPELL_BLESSING_OF_ELUNE     = 103917,
    SPELL_HAND_OF_ELUNE         = 105072,
    SPELL_WRATH_OF_ELUNE_1      = 105073, // 30k damage
    SPELL_WRATH_OF_ELUNE_2      = 105075, // 300k damage
    SPELL_LUNAR_SHOT_1          = 104214, // 30k damage, single
    SPELL_LUNAR_SHOT_2          = 104313, // 300k damage, single
    SPELL_LUNAR_SHOT_3          = 104688, // 300k damage, aoe
};

enum Events
{
    EVENT_MAGNISTRIKE           = 1,
    EVENT_MANNOROTH_AGGRO       = 2,
    EVENT_FEL_FIRESTORM         = 3,
    EVENT_SUMMON_DEBILITATOR    = 4,
    EVENT_SUMMON_DEBILITATOR_1  = 5,
    EVENT_FELBLADE              = 6,
    EVENT_SUMMON_DEVASTATOR     = 7,
    EVENT_EMBEDDED_BLADE        = 8,
    EVENT_EMBEDDED_BLADE_1      = 9,
    EVENT_DEBILITATING_FLAY     = 10,
    EVENT_CHECK_PLAYERS         = 12,
    EVENT_MANNOROTH_SARGERAS_1  = 13, // Illidan says
    EVENT_MANNOROTH_SARGERAS_2  = 14, // Tyrande says
    EVENT_INFERNAL              = 15, // Tyrande says
    EVENT_GIFT_OF_SARGERAS_1    = 16, // Illidan says
    EVENT_GIFT_OF_SARGERAS_2    = 17, // Tyrande says
    EVENT_GIFT_OF_SARGERAS_3    = 18, // Illidan says
    EVENT_GIFT_OF_SARGERAS_4    = 19, // Tyrande says
    EVENT_GIFT_OF_SARGERAS_5    = 20, // Illidan says
    EVENT_DESPAWN               = 21,
};

enum Adds
{
    NPC_FEL_FLAMES              = 55502,
    NPC_INFERNAL                = 56036,
    NPC_FELGUARD                = 56002,
    NPC_FELHOUND                = 56001,
    NPC_DOOMGUARD_DEVASTATOR    = 57410,
    NPC_DOOMGUARD_DEBILITATOR   = 55762,
    NPC_VAROTHEN_MAGICAL_BLADE  = 55837,
    NPC_EMBEDDED_BLADE          = 55838,
    NPC_PURPOSE_BUNNY_1         = 45979,
    NPC_PURPOSE_BUNNY_2         = 54020,
};

enum Actions
{
    ACTION_VAROTHEN_DIED    = 1,
    ACTION_DEBILITATING_OFF = 2,
};

const Position portalPos = {3338.699951f, -5699.775879f, 13.01f, 3.87f};

const Position varothenPos = {3319.669922f, -5716.470215f, 16.18f, 2.68f};

const Position debilitatorPos[2] =
{
    {3295.439941f, -5687.229980f, 14.19f, 5.74f}, // left dreadlord
    {3324.479980f, -5694.270020f, 13.97f, 3.17f}  // right dreadlord
};

const Position devastatorPos[3] =
{
    {3302.872803f, -5676.763672f, 12.53f, 4.74f},
    {3326.123047f, -5690.420410f, 13.14f, 3.81f},
    {3316.755127f, -5686.553711f, 14.03f, 4.26f}
};

const Position stalkerPos[3] =
{
    {3295.44f, -5687.23f, 14.19f, 5.74213f},
    {3324.48f, -5694.27f, 13.97f, 3.1765f},
    {3339.81f, -5698.5f, 15.5043f, 3.97935f},
};

class boss_mannoroth : public CreatureScript
{
    public:
        boss_mannoroth() : CreatureScript("boss_mannoroth") { }

        struct boss_mannorothAI : public BossAI
        {
            boss_mannorothAI(Creature* creature) : BossAI(creature, DATA_MANNOROTH)
            {
                me->ApplySpellImmune(0, IMMUNITY_EFFECT, SPELL_EFFECT_KNOCK_BACK, true);
                me->ApplySpellImmune(0, IMMUNITY_MECHANIC, MECHANIC_GRIP, true);
                me->ApplySpellImmune(0, IMMUNITY_MECHANIC, MECHANIC_STUN, true);
                me->ApplySpellImmune(0, IMMUNITY_MECHANIC, MECHANIC_FEAR, true);
                me->ApplySpellImmune(0, IMMUNITY_MECHANIC, MECHANIC_ROOT, true);
                me->ApplySpellImmune(0, IMMUNITY_MECHANIC, MECHANIC_FREEZE, true);
                me->ApplySpellImmune(0, IMMUNITY_MECHANIC, MECHANIC_POLYMORPH, true);
                me->ApplySpellImmune(0, IMMUNITY_MECHANIC, MECHANIC_HORROR, true);
                me->ApplySpellImmune(0, IMMUNITY_MECHANIC, MECHANIC_SAPPED, true);
                me->ApplySpellImmune(0, IMMUNITY_MECHANIC, MECHANIC_CHARM, true);
                me->ApplySpellImmune(0, IMMUNITY_MECHANIC, MECHANIC_DISORIENTED, true);
                me->ApplySpellImmune(0, IMMUNITY_STATE, SPELL_AURA_MOD_CONFUSE, true);
                me->SetReactState(REACT_PASSIVE);
            }

            void Reset() OVERRIDE
            {
                _Reset();

                me->SummonCreature(NPC_VAROTHEN, varothenPos, TempSummonType::TEMPSUMMON_CORPSE_TIMED_DESPAWN, 3000);

                phase = 0;
                bVarothen = false;
                bDebilitating = false;
                bEndEncounter = false;

                me->GetMap()->SetWorldState(WORLDSTATE_THATS_NOT_CANON, 0);
            }

            void JustReachedHome() OVERRIDE
            {
                if (Creature* pTyrande = me->FindNearestCreature(NPC_TYRANDE, 150.0f))
                    pTyrande->AI()->DoAction(7); // ACTION_MANNOROTH_RESET
                if (Creature* pIllidan = me->FindNearestCreature(NPC_ILLIDAN_2, 150.0f))
                    pIllidan->AI()->DoAction(7); // ACTION_MANNOROTH_RESET
            }

            void AttackStart(Unit* target) OVERRIDE
            {
                if (!target)
                    return;

                me->Attack(target, false);
            }

            void EnterCombat(Unit* /*who*/) OVERRIDE
            {
                if (Creature* pVarothen = ObjectAccessor::GetCreature(*me, instance->GetData64(DATA_VAROTHEN)))
                    if (!pVarothen->IsInCombat())
                        DoZoneInCombat(pVarothen);

                if (Creature* pIllidan = me->FindNearestCreature(NPC_ILLIDAN_2, 150.0f))
                    pIllidan->AI()->DoAction(6); // ACTION_MANNOROTH_AGGRO

                phase = 0;
                bVarothen = false;
                me->GetMap()->SetWorldState(WORLDSTATE_THATS_NOT_CANON, 0);
                bDebilitating = false;
                bEndEncounter = false;

                events.ScheduleEvent(EVENT_MANNOROTH_AGGRO, 5000);
                //events.ScheduleEvent(EVENT_FEL_FIRESTORM, 20000);
                events.ScheduleEvent(EVENT_SUMMON_DEBILITATOR, 65000);
                events.ScheduleEvent(EVENT_SUMMON_DEVASTATOR, urand(5000, 8000));
                events.ScheduleEvent(EVENT_SUMMON_DEVASTATOR, urand(5000, 8000));
                events.ScheduleEvent(EVENT_SUMMON_DEVASTATOR, urand(5000, 8000));
                events.ScheduleEvent(EVENT_CHECK_PLAYERS, 3000);

                for (uint8 i = 0; i < 2; ++i)
                    if (Creature* pStalker = me->SummonCreature(NPC_PURPOSE_BUNNY_1, stalkerPos[i]))
                        pStalker->RemoveAllAuras();

                if (Creature* pStalker = me->SummonCreature(NPC_PURPOSE_BUNNY_2, stalkerPos[2]))
                    pStalker->RemoveAllAuras();

                DoZoneInCombat();
                instance->SetBossState(DATA_MANNOROTH, IN_PROGRESS);
            }

            void KilledUnit(Unit* victim) OVERRIDE
            {
                if (victim && victim->GetTypeId() == TypeID::TYPEID_PLAYER)
                    Talk(SAY_MANNOROTH_KILL);
            }

            void DoAction(int32 action) OVERRIDE
            {
                if (action == ACTION_VAROTHEN_DIED)
                {
                    bVarothen = true;
                    events.ScheduleEvent(EVENT_EMBEDDED_BLADE, 5000);
                }
                else if (action == ACTION_DEBILITATING_OFF)
                {
                    bDebilitating = true;
                }
            }

            void DamageTaken(Unit* /*attacker*/, uint32& damage) OVERRIDE
            {
                if (!bDebilitating)
                    if (me->HealthBelowPctDamaged(88, damage))
                        damage = 0;

                if (damage >= me->GetHealth())
                    damage = 0;
            }

            void UpdateAI(uint32 diff) OVERRIDE
            {
                if (!UpdateVictim())
                    return;

                events.Update(diff);

                if (me->HasUnitState(UNIT_STATE_CASTING))
                    return;

                if (me->HealthBelowPct(90) && !bVarothen)
                {
                    Talk(SAY_MANNOROTH_VAROTHEN);
                    DoCast(me, SPELL_FEL_DRAIN);
                    bVarothen = true;
                    me->GetMap()->SetWorldState(WORLDSTATE_THATS_NOT_CANON, 1);
                    return;
                }
                if (me->HealthBelowPct(85) && (phase == 0))
                {
                    phase = 1;
                    events.CancelEvent(EVENT_SUMMON_DEVASTATOR);
                    Talk(SAY_MANNOROTH_SARGERAS);
                    DoCast(me, SPELL_NETHER_TEAR);
                    events.ScheduleEvent(EVENT_MANNOROTH_SARGERAS_1, 15000);
                    events.ScheduleEvent(EVENT_MANNOROTH_SARGERAS_2, 23000);
                    return;
                }
                else if (me->HealthBelowPct(70) && (phase == 1))
                {
                    phase = 2;
                    DoCast(me, SPELL_SUMMON_FELGUARD_AURA, true);
                }
                else if (me->HealthBelowPct(60) && (phase == 2))
                {
                    phase = 3;
                    DoCast(me, SPELL_SUMMON_DEVASTATOR_AURA, true);
                }
                else if (me->HealthBelowPct(50) && (phase == 3))
                {
                    phase = 4;
                    Talk(SAY_MANNOROTH_EVENT);
                    DoCast(me, SPELL_INFERNAL);
                    events.ScheduleEvent(EVENT_INFERNAL, 20000);
                    return;
                }
                else if (me->HealthBelowPct(25) && (phase == 4))
                {
                    phase = 5;
                    bEndEncounter = true;
                    CompleteEncounter();
                    return;
                }

                if (uint32 eventId = events.ExecuteEvent())
                {
                    switch (eventId)
                    {
                        case EVENT_CHECK_PLAYERS:
                            if (!SelectTarget(SELECT_TARGET_RANDOM, 0, 100.0f, true))
                            {
                                events.Reset();
                                EnterEvadeMode();
                                return;
                            }
                            else
                                events.ScheduleEvent(EVENT_CHECK_PLAYERS, 3000);
                            break;
                        case EVENT_MANNOROTH_AGGRO:
                            Talk(SAY_MANNOROTH_AGGRO);
                            break;
                        case EVENT_FEL_FIRESTORM:
                            //DoCast(me, SPELL_FEL_FIRESTORM);
                            //events.ScheduleEvent(EVENT_FEL_FIRESTORM, urand(25000, 35000));
                            break;
                        case EVENT_FELBLADE:
                            DoCast(me, SPELL_FELBLADE);
                            events.ScheduleEvent(EVENT_FELBLADE, urand(15000, 25000));
                            break;
                        case EVENT_SUMMON_DEBILITATOR:
                            DoCast(me, SPELL_NETHER_PORTAL);
                            events.ScheduleEvent(EVENT_SUMMON_DEBILITATOR_1, 7000);
                            break;
                        case EVENT_SUMMON_DEBILITATOR_1:
                            for (uint8 i = 0; i < 2; ++i)
                                me->SummonCreature(NPC_DOOMGUARD_DEBILITATOR, debilitatorPos[i], TempSummonType::TEMPSUMMON_CORPSE_TIMED_DESPAWN, 3000);
                            break;
                        case EVENT_SUMMON_DEVASTATOR:
                        {
                            uint32 i = urand(0, 2);
                            me->SummonCreature(NPC_DOOMGUARD_DEVASTATOR, devastatorPos[i], TempSummonType::TEMPSUMMON_CORPSE_TIMED_DESPAWN, 5000);
                            events.ScheduleEvent(EVENT_SUMMON_DEVASTATOR, urand(4000, 7000));
                            break;
                        }
                        case EVENT_EMBEDDED_BLADE:
                            Talk(SAY_MANNOROTH_SWORD);
                            DoCast(me, SPELL_EMBEDDED_BLADE_2, true);
                            DoCast(me, SPELL_EMBEDDED_BLADE_DUMMY, true);
                            events.ScheduleEvent(EVENT_EMBEDDED_BLADE_1, 3000);

                            break;
                        case EVENT_EMBEDDED_BLADE_1:
                            if (Creature* pIllidan = me->FindNearestCreature(NPC_ILLIDAN_2, 100.0f))
                                pIllidan->AI()->Talk(15); // SAY_ILLIDAN_2_SWORD
                            //events.RescheduleEvent(EVENT_FEL_FIRESTORM, urand(25000, 30000));
                            //events.RescheduleEvent(EVENT_FELBLADE, urand(10000, 55000));
                            break;
                        case EVENT_MANNOROTH_SARGERAS_1:
                            if (Creature* pIllidan = me->FindNearestCreature(NPC_ILLIDAN_2, 100.0f))
                                pIllidan->AI()->Talk(19); // SAY_ILLIDAN_2_SPELL
                            DoCast(me, SPELL_SUMMON_FELHOUND_AURA, true);
                            break;
                        case EVENT_MANNOROTH_SARGERAS_2:
                            if (Creature* pTyrande = me->FindNearestCreature(NPC_TYRANDE, 150.0f))
                            {
                                pTyrande->AI()->Talk(10); // SAY_TYRANDE_ARROWS
                                pTyrande->AI()->DoAction(9); // ACTION_MANNOROTH_SARGERAS
                            }
                            break;
                        case EVENT_INFERNAL:
                            if (Creature* pTyrande = me->FindNearestCreature(NPC_TYRANDE, 150.0f))
                            {
                                pTyrande->AI()->Talk(14); // SAY_TYRANDE_MANY_DEMONS
                                pTyrande->AI()->DoAction(10); // ACTION_MANNOROTH_INFERNO
                            }
                            //events.RescheduleEvent(EVENT_FEL_FIRESTORM, urand(40000, 50000));
                            //events.RescheduleEvent(EVENT_FELBLADE, urand(45000, 55000));
                            events.ScheduleEvent(EVENT_GIFT_OF_SARGERAS_1, 5000);
                            break;
                        case EVENT_GIFT_OF_SARGERAS_1:
                            if (Creature* pIllidan = me->FindNearestCreature(NPC_ILLIDAN_2, 100.0f))
                            {
                                pIllidan->AI()->Talk(16); // SAY_ILLIDAN_2_BUFF_1
                                pIllidan->CastSpell(me, SPELL_GIFT_OF_SARGERAS);
                            }
                            events.ScheduleEvent(EVENT_GIFT_OF_SARGERAS_2, 8000);
                            break;
                        case EVENT_GIFT_OF_SARGERAS_2:
                            if (Creature* pTyrande = me->FindNearestCreature(NPC_TYRANDE, 150.0f))
                                pTyrande->AI()->Talk(11); // SAY_TYRANDE_NO_1
                            events.ScheduleEvent(EVENT_GIFT_OF_SARGERAS_3, 2000);
                            break;
                        case EVENT_GIFT_OF_SARGERAS_3:
                            if (Creature* pIllidan = me->FindNearestCreature(NPC_ILLIDAN_2, 100.0f))
                                pIllidan->AI()->Talk(17); // SAY_ILLIDAN_2_BUFF_2
                            events.ScheduleEvent(EVENT_GIFT_OF_SARGERAS_4, 8000);
                            break;
                        case EVENT_GIFT_OF_SARGERAS_4:
                            if (Creature* pTyrande = me->FindNearestCreature(NPC_TYRANDE, 150.0f))
                                pTyrande->AI()->Talk(12); // SAY_TYRANDE_NO_2
                            events.ScheduleEvent(EVENT_GIFT_OF_SARGERAS_5, 2000);
                            break;
                        case EVENT_GIFT_OF_SARGERAS_5:
                            if (Creature* pIllidan = me->FindNearestCreature(NPC_ILLIDAN_2, 100.0f))
                                pIllidan->AI()->Talk(18); // SAY_ILLIDAN_2_BUFF_3
                            break;
                        case EVENT_DESPAWN:
                            instance->DoRespawnGameObject(instance->GetData64(DATA_MINOR_CACHE), DAY);
                            me->DespawnOrUnsummon();
                            break;
                        default:
                            break;
                    }
                }

                //DoMeleeAttackIfReady();
            }

        private:
            uint8 phase;
            bool bVarothen;
            bool bDebilitating;
            bool bEndEncounter;

            void CompleteEncounter()
            {
                Talk(SAY_MANNOROTH_DEATH);
                events.Reset();
                summons.DespawnAll();
                me->InterruptNonMeleeSpells(true);

                // Achievement
                instance->DoUpdateAchievementCriteria(ACHIEVEMENT_CRITERIA_TYPE_BE_SPELL_TARGET, SPELL_COMPLETE_ENCOUNTER, 0, me);

                // Guild Achievement
                Map::PlayerList const &PlayerList = instance->instance->GetPlayers();
                if (!PlayerList.isEmpty())
                {
                    for (Map::PlayerList::const_iterator i = PlayerList.begin(); i != PlayerList.end(); ++i)
                    {
                        if (Player* player = i->GetSource())
                            if (Group* pGroup = player->GetGroup())
                                if (player->GetGuildId() && pGroup->IsGuildGroup(player->GetGuildId(), player))
                                {
                                    if (Guild* guild = sGuildMgr->GetGuildById(player->GetGuildId()))
                                        guild->UpdateAchievementCriteria(ACHIEVEMENT_CRITERIA_TYPE_BE_SPELL_TARGET, SPELL_COMPLETE_ENCOUNTER, 0, 0, me, player);
                                    break;
                                }
                    }
                }
                WoEKilledMonsterCredit(instance, QUEST_THE_PATH_TO_THE_DRAGON_SOUL, 54969);
                instance->UpdateEncounterState(EncounterCreditType::ENCOUNTER_CREDIT_CAST_SPELL, SPELL_COMPLETE_ENCOUNTER, me);
                instance->SetBossState(DATA_MANNOROTH, DONE);
                WoEModifyPlayerCurrencies(instance, CURRENCY_TYPE_JUSTICE_POINTS, 7000);

                if (Creature* pTyrande = me->FindNearestCreature(NPC_TYRANDE, 150.0f))
                    pTyrande->AI()->DoAction(11); // ACTION_MANNOROTH_END

                events.ScheduleEvent(EVENT_DESPAWN, 5000);
            }
        };

        CreatureAI* GetAI(Creature* creature) const OVERRIDE
        {
            return GetWoEAI<boss_mannorothAI>(creature);
        }
};

class npc_mannoroth_varothen : public CreatureScript
{
    public:
        npc_mannoroth_varothen() : CreatureScript("npc_mannoroth_varothen") { }

        struct npc_mannoroth_varothenAI : public ScriptedAI
        {
            npc_mannoroth_varothenAI(Creature* creature) : ScriptedAI(creature)
            {
                me->ApplySpellImmune(0, IMMUNITY_EFFECT, SPELL_EFFECT_KNOCK_BACK, true);
                me->ApplySpellImmune(0, IMMUNITY_MECHANIC, MECHANIC_GRIP, true);
                me->ApplySpellImmune(0, IMMUNITY_MECHANIC, MECHANIC_STUN, true);
                me->ApplySpellImmune(0, IMMUNITY_MECHANIC, MECHANIC_FEAR, true);
                me->ApplySpellImmune(0, IMMUNITY_MECHANIC, MECHANIC_ROOT, true);
                me->ApplySpellImmune(0, IMMUNITY_MECHANIC, MECHANIC_FREEZE, true);
                me->ApplySpellImmune(0, IMMUNITY_MECHANIC, MECHANIC_POLYMORPH, true);
                me->ApplySpellImmune(0, IMMUNITY_MECHANIC, MECHANIC_HORROR, true);
                me->ApplySpellImmune(0, IMMUNITY_MECHANIC, MECHANIC_SAPPED, true);
                me->ApplySpellImmune(0, IMMUNITY_MECHANIC, MECHANIC_CHARM, true);
                me->ApplySpellImmune(0, IMMUNITY_MECHANIC, MECHANIC_DISORIENTED, true);
                me->ApplySpellImmune(0, IMMUNITY_STATE, SPELL_AURA_MOD_CONFUSE, true);
                instance = me->GetInstanceScript();

            }

            void Reset() OVERRIDE
            {
                events.Reset();
                if (!me->FindNearestCreature(NPC_ILLIDAN_2, 100.0f))
                    me->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_IMMUNE_TO_PC);
            }

            void EnterCombat(Unit* /*who*/) OVERRIDE
            {
                Talk(SAY_VAROTHEN_AGGRO);
                events.ScheduleEvent(EVENT_MAGNISTRIKE, urand(3000, 7000));

                if (instance)
                    if (Creature* pMannoroth = ObjectAccessor::GetCreature(*me, instance->GetData64(DATA_MANNOROTH)))
                        if (!pMannoroth->IsInCombat())
                            DoZoneInCombat(pMannoroth);
            }

            void KilledUnit(Unit* victim) OVERRIDE
            {
                if (victim && victim->GetTypeId() == TypeID::TYPEID_PLAYER)
                    Talk(SAY_VAROTHEN_KILL);
            }

            void SpellHit(Unit* caster, const SpellInfo* spell) OVERRIDE
            {
                if (spell->Id == SPELL_ARCHIVED_VAROTHEN_1)
                    DoCast(caster, SPELL_ARCHIVED_VAROTHEN_2, true);
            }

            void JustDied(Unit* /*killer*/) OVERRIDE
            {
                Talk(SAY_VAROTHEN_DEATH);

                if (Creature* pMannoroth = ObjectAccessor::GetCreature(*me, instance->GetData64(DATA_MANNOROTH)))
                    pMannoroth->AI()->DoAction(ACTION_VAROTHEN_DIED);

                Map::PlayerList const &PlayerList = instance->instance->GetPlayers();
                if (!PlayerList.isEmpty())
                    for (Map::PlayerList::const_iterator i = PlayerList.begin(); i != PlayerList.end(); ++i)
                        if (Player* player = i->GetSource())
                            if (me->GetDistance(player) <= 50.0f && player->GetQuestStatus(QUEST_DOCUMENTING_THE_TIMEWAYS) == QUEST_STATUS_INCOMPLETE)
                                player->CastSpell(me, SPELL_ARCHIVED_VAROTHEN_1, true);
            }

            void UpdateAI(uint32 diff) OVERRIDE
            {
                if (!UpdateVictim())
                    return;

                events.Update(diff);

                if (me->HasUnitState(UNIT_STATE_CASTING))
                    return;

                if (uint32 eventId = events.ExecuteEvent())
                {
                    if (eventId == EVENT_MAGNISTRIKE)
                    {
                        DoCastVictim(SPELL_MAGNISTRIKE);
                        events.ScheduleEvent(EVENT_MAGNISTRIKE, urand(10000, 15000));
                    }
                }

                DoMeleeAttackIfReady();
            }

        private:
            EventMap events;
            InstanceScript* instance;
        };

        CreatureAI* GetAI(Creature* creature) const OVERRIDE
        {
            return GetWoEAI<npc_mannoroth_varothenAI>(creature);
        }
};

class npc_mannoroth_doomguard_debilitator : public CreatureScript
{
    public:
        npc_mannoroth_doomguard_debilitator() : CreatureScript("npc_mannoroth_doomguard_debilitator") { }

        struct npc_mannoroth_doomguard_debilitatorAI : public ScriptedAI
        {
            npc_mannoroth_doomguard_debilitatorAI(Creature* creature) : ScriptedAI(creature)
            {
                me->ApplySpellImmune(0, IMMUNITY_EFFECT, SPELL_EFFECT_KNOCK_BACK, true);
                me->ApplySpellImmune(0, IMMUNITY_MECHANIC, MECHANIC_GRIP, true);
                me->ApplySpellImmune(0, IMMUNITY_MECHANIC, MECHANIC_STUN, true);
                me->ApplySpellImmune(0, IMMUNITY_MECHANIC, MECHANIC_FEAR, true);
                me->ApplySpellImmune(0, IMMUNITY_MECHANIC, MECHANIC_ROOT, true);
                me->ApplySpellImmune(0, IMMUNITY_MECHANIC, MECHANIC_FREEZE, true);
                me->ApplySpellImmune(0, IMMUNITY_MECHANIC, MECHANIC_POLYMORPH, true);
                me->ApplySpellImmune(0, IMMUNITY_MECHANIC, MECHANIC_HORROR, true);
                me->ApplySpellImmune(0, IMMUNITY_MECHANIC, MECHANIC_SAPPED, true);
                me->ApplySpellImmune(0, IMMUNITY_MECHANIC, MECHANIC_CHARM, true);
                me->ApplySpellImmune(0, IMMUNITY_MECHANIC, MECHANIC_DISORIENTED, true);
                me->ApplySpellImmune(0, IMMUNITY_STATE, SPELL_AURA_MOD_CONFUSE, true);
                me->SetReactState(REACT_PASSIVE);
            }

            void Reset() OVERRIDE
            {
                events.Reset();
            }

            void IsSummonedBy(Unit* /*summoner*/) OVERRIDE
            {
                events.ScheduleEvent(EVENT_DEBILITATING_FLAY, 1000);
            }

            void JustDied(Unit* /*killer*/) OVERRIDE
            {
                if (Creature* pTyrande = me->FindNearestCreature(NPC_TYRANDE, 100.0f))
                    pTyrande->AI()->DoAction(8); // ACTION_DEBILITATING_OFF
            }

            void UpdateAI(uint32 diff) OVERRIDE
            {
                events.Update(diff);

                if (uint32 eventId = events.ExecuteEvent())
                {
                    if (eventId == EVENT_DEBILITATING_FLAY)
                    {
                        DoCast(me, SPELL_DEBILITATING_FLAY);
                    }
                }
            }

        private:
            EventMap events;
        };

        CreatureAI* GetAI(Creature* creature) const OVERRIDE
        {
            return GetWoEAI<npc_mannoroth_doomguard_debilitatorAI>(creature);
        }
};

class spell_mannoroth_gift_of_sargeras : public SpellScriptLoader
{
    public:
        spell_mannoroth_gift_of_sargeras() : SpellScriptLoader("spell_mannoroth_gift_of_sargeras") { }

        class spell_mannoroth_gift_of_sargeras_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_mannoroth_gift_of_sargeras_SpellScript);

            void HandleAfterHit()
            {
                if (!GetCaster())
                    return;

                GetCaster()->CastSpell(GetCaster(), SPELL_GIFT_OF_SARGERAS_AOE, true);
            }

            void Register() OVERRIDE
            {
                AfterHit += SpellHitFn(spell_mannoroth_gift_of_sargeras_SpellScript::HandleAfterHit);
            }
        };

        SpellScript* GetSpellScript() const OVERRIDE
        {
            return new spell_mannoroth_gift_of_sargeras_SpellScript();
        }
};

void AddSC_boss_mannoroth()
{
    new boss_mannoroth();
    new npc_mannoroth_varothen();
    new npc_mannoroth_doomguard_debilitator();
    new spell_mannoroth_gift_of_sargeras();
}
