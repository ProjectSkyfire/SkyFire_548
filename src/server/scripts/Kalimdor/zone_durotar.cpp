/*
* This file is part of Project SkyFire https://www.projectskyfire.org. 
* See LICENSE.md file for Copyright information
*/

#include "ScriptMgr.h"
#include "ScriptedCreature.h"
#include "ScriptedGossip.h"
#include "SpellScript.h"
#include "Player.h"

enum darkspearshore
{
    NPC_ZUNI = 37988
};
Position const ZuniStartPos = { -1173.345f, -5267.1723f, 0.909172f, 0 };
Position const ZuniMovePos1 = { -1172.816f, -5299.0522f, 5.1754074f, 0 };
Position const ZuniMovePos2 = { -1176.1216f, -5326.733f, 13.596682f, 0 };
Position const ZuniMovePos3 = { -1172.816f, -5344.54f, 15.331013f, 0 };
Position const ZuniMovePos4 = { -1164.625f, -5369.5225f, 14.166052f, 0 };
Position const ZuniMovePos5 = { -1162.0435f, -5391.7466f, 12.003188f };
/*####
# npc_zuni
####*/
class npc_zuni : public CreatureScript
{
public:
    npc_zuni() : CreatureScript("npc_zuni") { }

    CreatureAI* GetAI(Creature* creature) const OVERRIDE
    {
        return new npc_zuniAI(creature);
    }

    struct npc_zuniAI : public ScriptedAI
    {
        bool wp1 = false;
        bool wp2 = false;
        bool wp3 = false;
        bool wp4 = false;
        bool wp5 = false;
        bool wp6 = false;
        uint32 MoveTimer = 5000;

        npc_zuniAI(Creature* creature) : ScriptedAI(creature) { }

        void MovementInform(uint32 type, uint32 id) OVERRIDE
        {
            if (type == POINT_MOTION_TYPE)
            {
                if (id == 0)
                    wp1 = true;

                if (id == 1)
                    wp2 = true;

                if (id == 2)
                    wp3 = true;

                if (id == 3)
                    wp4 = true;

                if (id == 4)
                    wp5 = true;

                if (id == 5)
                    wp6 = true;
            }
            else
                return;
        }

        void UpdateAI(uint32 diff) OVERRIDE
        {
            if (wp1 == true)
            {
                if (MoveTimer <= diff)
                {
                    me->GetMotionMaster()->MovePoint(1, ZuniMovePos1);
                    wp1 = false;
                    MoveTimer = 5000;
                }
                else MoveTimer -= diff;
            }

            if (wp2 == true)
            {
                me->GetMotionMaster()->MovePoint(2, ZuniMovePos2);
                wp2 = false;
            }

            if (wp3 == true)
            {
                me->GetMotionMaster()->MovePoint(3, ZuniMovePos3);
                wp3 = false;
            }

            if (wp4 == true)
            {
                me->GetMotionMaster()->MovePoint(4, ZuniMovePos4);
                wp4 = false;
            }

            if (wp5 == true)
            {
                me->GetMotionMaster()->MovePoint(5, ZuniMovePos5);
                wp5 = false;
            }

            if (wp6 == true)
            {
                me->MonsterSay("Ya trainer should be somewhere in the grounds'ere. I'll catch you lata mon.", Language::LANG_UNIVERSAL, me);
                me->SendPlaySound(21367, true);
                me->DespawnOrUnsummon(10000);
                wp6 = false;
            }
        }
    };
};

/*####
# npc_jinthala
####*/
class npc_jinthala : public CreatureScript
{
public:
    npc_jinthala() : CreatureScript("npc_jinthala") { }

    CreatureAI* GetAI(Creature* creature) const OVERRIDE
    {
        return new npc_jinthalaAI(creature);
    }

    bool OnQuestAccept(Player* player, Creature* creature, Quest const* /*quest*/) OVERRIDE
    {
        creature->MonsterSay("$n. Zuni. Ya'll find ya trainer in the trainin' grounds to the east. Bring pride to the Darkspear.", Language::LANG_UNIVERSAL, player);
        
        if (Creature* zuni = player->FindNearestCreature(NPC_ZUNI, 10.0f, true))
        {
            zuni->MonsterSay("Ya. mon. Let's crack some tiki target skulls! ", Language::LANG_UNIVERSAL, player);
            zuni->SendPlaySound(21366, true);
            zuni->GetMotionMaster()->MovePoint(0, ZuniStartPos);
            return true;
        }
        return false;
    }

    struct npc_jinthalaAI : public ScriptedAI
    {
        npc_jinthalaAI(Creature* creature) : ScriptedAI(creature) { }
    };
};

/*######
## Quest: Proving Pit: 24642, 24754, 24762, 24768, 24774, 24780, 24786, 26276, 31161
## npc_darkspear_jailor
######*/

Position DarkSpearJailorPos = { -1153.53f, -5519.42f, 11.98f, 6.27f };
Position DarkSpearJailorPos2 = { -1135.68f, -5417.78f, 13.26f, 1.47f };
Position NagaPos = { -1149.90f, -5527.76f, 8.10f, 4.76f };
Position NagaPos2 = { -1141.74f, -5414.54f, 10.59f, 3.42f };

class npc_darkspear_jailor : public CreatureScript
{
public:
    npc_darkspear_jailor() : CreatureScript("npc_darkspear_jailor") { }

    CreatureAI* GetAI(Creature* creature) const OVERRIDE
    {
        return new npc_darkspear_jailorAI(creature);
    }

    struct npc_darkspear_jailorAI : public ScriptedAI
    {
        enum JailorEvents
        {
            EVENT_JAILOR_OPENING = 1,
            EVENT_JAILOR_OPEN_CAGE = 2,
            EVENT_JAILOR_RESET = 3,
        };
        EventMap events;
        npc_darkspear_jailorAI(Creature* creature) : ScriptedAI(creature) { }

        void Reset() OVERRIDE
        {
            events.Reset();
        }

        void MovementInform(uint32 type, uint32 id) OVERRIDE
        {
            if (type != POINT_MOTION_TYPE)
                return;

            switch (id)
            {
                case 0:
                {
                    events.ScheduleEvent(EVENT_JAILOR_OPENING, 1000);
                    break;
                }
                default:
                    break;
            }
        }

        void UpdateAI(uint32 diff) OVERRIDE
        {
            events.Update(diff);
            while (uint32 eventId = events.ExecuteEvent())
            {
                switch (eventId)
                {
                    case EVENT_JAILOR_OPENING:
                    {
                        me->HandleEmoteCommand(EMOTE_ONESHOT_USE_STANDING);
                        events.ScheduleEvent(EVENT_JAILOR_OPEN_CAGE, 1000);
                        break;
                    }
                    case EVENT_JAILOR_OPEN_CAGE:
                    {
                        if (GameObject* cage = me->FindNearestGameObject(201968, 5.0f))
                            cage->UseDoorOrButton(120);

                        if (Creature* monk = me->FindNearestCreature(63310, 25.0f))
                        {
                            if (Creature* Naga = me->FindNearestCreature(38142, 5.0f))
                            {
                                Naga->setFaction(14);
                                Naga->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);
                                Naga->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_IMMUNE_TO_PC);
                                Naga->GetMotionMaster()->MovePoint(0, NagaPos2);
                                Naga->MonsterSay("I sshal ssslaughter you, Darksspear runt!", Language::LANG_UNIVERSAL, 0);
                            }
                        }
                        else
                        {
                            if (Creature* Naga2 = me->FindNearestCreature(38142, 5.0f))
                            {
                                Naga2->setFaction(14);
                                Naga2->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);
                                Naga2->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_IMMUNE_TO_PC);
                                Naga2->GetMotionMaster()->MovePoint(0, NagaPos);
                                Naga2->MonsterSay("I sshal ssslaughter you, Darksspear runt!", Language::LANG_UNIVERSAL, 0);
                            }
                        }

                        events.ScheduleEvent(EVENT_JAILOR_RESET, 120000);
                        break;
                    }
                    case EVENT_JAILOR_RESET:
                    {
                        me->GetMotionMaster()->MovePoint(1, me->GetHomePosition());
                        Reset();
                        break;
                    }
                    default:
                        break;
                }
            }
        }
    };

    bool OnGossipSelect(Player* player, Creature* creature, uint32 /*sender*/, uint32 action) OVERRIDE
    {
        player->PlayerTalkClass->ClearMenus();
        if (action == GOSSIP_ACTION_INFO_DEF)
        {
            player->CLOSE_GOSSIP_MENU();
            if (Creature* monk = creature->FindNearestCreature(63310, 25.0f))
            {
                creature->GetMotionMaster()->MovePoint(0, DarkSpearJailorPos2);
            }
            else
            {
                creature->GetMotionMaster()->MovePoint(0, DarkSpearJailorPos);
            }
            const char* text = player->getGender() == GENDER_MALE ? "Get in the pit and show us your stuff, boy." : "Get in the pit and show us your stuff, girl.";
            creature->MonsterSay(text, Language::LANG_UNIVERSAL, creature);
            player->KilledMonsterCredit(39062);            
        }
        return true;
    }

    bool OnGossipHello(Player* player, Creature* creature) OVERRIDE
    {
        player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, "I'm ready to face my challenge.", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF);

        player->SEND_GOSSIP_MENU(player->GetGossipTextId(creature), creature->GetGUID());
        return true;
    }
};



/*######
## Quest 25134: Lazy Peons
## npc_lazy_peon
######*/

enum LazyPeonYells
{
    SAY_SPELL_HIT                                 = 0
};

enum LazyPeon
{
    QUEST_LAZY_PEONS    = 25134,
    GO_LUMBERPILE       = 175784,
    SPELL_BUFF_SLEEP    = 17743,
    SPELL_AWAKEN_PEON   = 19938
};

class npc_lazy_peon : public CreatureScript
{
public:
    npc_lazy_peon() : CreatureScript("npc_lazy_peon") { }

    CreatureAI* GetAI(Creature* creature) const OVERRIDE
    {
        return new npc_lazy_peonAI(creature);
    }

    struct npc_lazy_peonAI : public ScriptedAI
    {
        npc_lazy_peonAI(Creature* creature) : ScriptedAI(creature) { }

        uint64 PlayerGUID;

        uint32 RebuffTimer;
        bool work;

        void Reset() OVERRIDE
        {
            PlayerGUID = 0;
            RebuffTimer = 0;
            work = false;
        }

        void MovementInform(uint32 /*type*/, uint32 id) OVERRIDE
        {
            if (id == 1)
                work = true;
        }

        void SpellHit(Unit* caster, const SpellInfo* spell) OVERRIDE
        {
            if (spell->Id != SPELL_AWAKEN_PEON)
                return;

            Player* player = caster->ToPlayer();
            if (player && player->GetQuestStatus(QUEST_LAZY_PEONS) == QUEST_STATUS_INCOMPLETE)
            {
                player->KilledMonsterCredit(me->GetEntry(), me->GetGUID());
                Talk(SAY_SPELL_HIT, caster);
                me->RemoveAllAuras();
                if (GameObject* Lumberpile = me->FindNearestGameObject(GO_LUMBERPILE, 20))
                    me->GetMotionMaster()->MovePoint(1, Lumberpile->GetPositionX()-1, Lumberpile->GetPositionY(), Lumberpile->GetPositionZ());
            }
        }

        void UpdateAI(uint32 diff) OVERRIDE
        {
            if (work == true)
                me->HandleEmoteCommand(EMOTE_ONESHOT_WORK_CHOPWOOD);
            if (RebuffTimer <= diff)
            {
                DoCast(me, SPELL_BUFF_SLEEP);
                RebuffTimer = 300000; //Rebuff agian in 5 minutes
            }
            else
                RebuffTimer -= diff;
            if (!UpdateVictim())
                return;
            DoMeleeAttackIfReady();
        }
    };
};

enum VoodooSpells
{
    SPELL_BREW      = 16712, // Special Brew
    SPELL_GHOSTLY   = 16713, // Ghostly
    SPELL_HEX1      = 16707, // Hex
    SPELL_HEX2      = 16708, // Hex
    SPELL_HEX3      = 16709, // Hex
    SPELL_GROW      = 16711, // Grow
    SPELL_LAUNCH    = 16716, // Launch (Whee!)
};

// 17009
class spell_voodoo : public SpellScriptLoader
{
    public:
        spell_voodoo() : SpellScriptLoader("spell_voodoo") { }

        class spell_voodoo_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_voodoo_SpellScript);

            bool Validate(SpellInfo const* /*spellInfo*/) OVERRIDE
            {
                if (!sSpellMgr->GetSpellInfo(SPELL_BREW) || !sSpellMgr->GetSpellInfo(SPELL_GHOSTLY) ||
                    !sSpellMgr->GetSpellInfo(SPELL_HEX1) || !sSpellMgr->GetSpellInfo(SPELL_HEX2) ||
                    !sSpellMgr->GetSpellInfo(SPELL_HEX3) || !sSpellMgr->GetSpellInfo(SPELL_GROW) ||
                    !sSpellMgr->GetSpellInfo(SPELL_LAUNCH))
                    return false;
                return true;
            }

            void HandleDummy(SpellEffIndex /*effIndex*/)
            {
                uint32 spellid = RAND(SPELL_BREW, SPELL_GHOSTLY, RAND(SPELL_HEX1, SPELL_HEX2, SPELL_HEX3), SPELL_GROW, SPELL_LAUNCH);
                if (Unit* target = GetHitUnit())
                    GetCaster()->CastSpell(target, spellid, false);
            }

            void Register() OVERRIDE
            {
                OnEffectHitTarget += SpellEffectFn(spell_voodoo_SpellScript::HandleDummy, EFFECT_0, SPELL_EFFECT_DUMMY);
            }
        };

        SpellScript* GetSpellScript() const OVERRIDE
        {
            return new spell_voodoo_SpellScript();
        }
};

// 3125
class npc_clattering_scorpid : public CreatureScript
{
    
public:
    npc_clattering_scorpid() : CreatureScript("npc_clattering_scorpid") { }


    CreatureAI* GetAI(Creature* creature) const OVERRIDE
    {
        return new npc_clattering_scorpidAI(creature);
    }

    struct npc_clattering_scorpidAI : public ScriptedAI
    {
        enum clatteringScorpidEvents
        {
            EVENT_CAST_VENOM = 1,
            EVENT_KC_TOTEM = 2
        };
        EventMap events;
        npc_clattering_scorpidAI(Creature* creature) : ScriptedAI(creature) { }

        void Reset() OVERRIDE
        {
            events.Reset();
        }

        void EnterCombat(Unit* /*who*/) OVERRIDE
        {
            events.ScheduleEvent(EVENT_CAST_VENOM, 2000);
        }

        void UpdateAI(uint32 diff) OVERRIDE
        {
            if (!UpdateVictim())
                return;

            events.Update(diff);
 
            while (uint32 eventId = events.ExecuteEvent())
            {
                switch (eventId)
                {
                    case EVENT_CAST_VENOM:
                    {
                        if (!me->FindNearestCreature(39236, 5.0f))
                            Talk(0, me);
            
                        me->CastSpell(me->GetVictim(), 73672);
                        events.ScheduleEvent(EVENT_KC_TOTEM, 2500);
                        events.ScheduleEvent(EVENT_CAST_VENOM, 7500);
                        break;
                    }
                    case EVENT_KC_TOTEM:
                    {
                        if (Creature* totem = me->FindNearestCreature(39236, 5.0f))
                            totem->GetOwner()->ToPlayer()->KilledMonsterCredit(39236);
                        break;
                    }
                    default:
                        break;
                }
            }

            DoMeleeAttackIfReady();
        }
    };
};

// 73705 - Injured Razor Hill Grunt - Cancel Feign Death
class spell_injured_razor_hill_grunt_cancel_feign_death : public SpellScript
{
    PrepareSpellScript(spell_injured_razor_hill_grunt_cancel_feign_death);

    void HandleDummy(SpellEffIndex /*effIndex*/)
    {
        if (Unit* target = GetHitUnit())
            if (target->getStandState() == UNIT_STAND_STATE_DEAD)
                target->SetStandState(UNIT_STAND_STATE_STAND);
    }

    void Register() OVERRIDE
    {
        OnEffectHitTarget += SpellEffectFn(spell_injured_razor_hill_grunt_cancel_feign_death::HandleDummy, EFFECT_0, SPELL_EFFECT_DUMMY);
    }
};

enum NpcInjuredRazorHillGruntUtilities
{
    EVENT_TEXT_BEFORE_HELP = 1,
    EVENT_AFTER_HELP       = 2,
    EVENT_DESPAWN          = 3,

    TEXT_BEFORE_HELP       = 0,
    TEXT_AFTER_HELP        = 1,
    TEXT_DESPAWN           = 2,

    QUEST_LOSS_REDUCTION   = 25179
};

// 39270 - Injured Razor Hill Grunt
struct npc_injured_razor_hill_grunt : public ScriptedAI
{
    npc_injured_razor_hill_grunt(Creature* creature) : ScriptedAI(creature) { }

    void Reset() OVERRIDE
    {
        me->setRegeneratingHealth(false);
        me->SetHealth(me->CountPctFromMaxHealth(10));
        _events.ScheduleEvent(EVENT_TEXT_BEFORE_HELP, 60 * IN_MILLISECONDS, 120 * IN_MILLISECONDS);
    }

    void OnSpellClick(Unit* clicker, bool& /*result*/) OVERRIDE
    {
        if (Player* player = clicker->ToPlayer())
        {
            me->RemoveFlag(UNIT_FIELD_NPC_FLAGS, UNIT_NPC_FLAG_SPELLCLICK);
            me->SetFlag(UNIT_FIELD_NPC_FLAGS, UNIT_NPC_FLAG_QUESTGIVER);
            me->GetMotionMaster()->MoveFollow(player, 0.f, 0.f);
            player->KilledMonsterCredit(me->GetEntry());
            _events.ScheduleEvent(EVENT_AFTER_HELP, 2 * IN_MILLISECONDS);

            if (Quest const* quest = sObjectMgr->GetQuestTemplate(QUEST_LOSS_REDUCTION))
                if (player->GetQuestStatus(quest->GetQuestId()) == QUEST_STATUS_NONE)
                {
                    player->PlayerTalkClass->SendQuestGiverQuestDetails(quest, me->GetGUID(), true);
                    player->AddQuest(quest, nullptr);
                }
        }
    }

    void UpdateAI(uint32 diff) OVERRIDE
    {
        _events.Update(diff);

        while (uint32 eventId = _events.ExecuteEvent())
        {
            switch (eventId)
            {
                case EVENT_TEXT_BEFORE_HELP:
                {
                    Talk(TEXT_BEFORE_HELP);
                    _events.ScheduleEvent(EVENT_TEXT_BEFORE_HELP, 60 * IN_MILLISECONDS, 120 * IN_MILLISECONDS);
                    break;
                }
                case EVENT_AFTER_HELP:
                {
                    Talk(TEXT_AFTER_HELP);
                    _events.CancelEvent(EVENT_TEXT_BEFORE_HELP);
                    _events.ScheduleEvent(EVENT_DESPAWN, 60 * IN_MILLISECONDS, 80 * IN_MILLISECONDS);
                    break;
                }
                case EVENT_DESPAWN:
                {
                    Talk(TEXT_DESPAWN);
                    me->GetMotionMaster()->MovementExpired(true);
                    me->DespawnOrUnsummon(3 * IN_MILLISECONDS);
                    break;
                }
                default:
                    break;
            }
        }
    }

private:
    EventMap _events;
};

void AddSC_durotar()
{
    new npc_zuni();
    new npc_jinthala();
    new npc_darkspear_jailor();
    new npc_lazy_peon();
    new spell_voodoo();
    new npc_clattering_scorpid();
    RegisterCreatureAI(npc_injured_razor_hill_grunt);
    RegisterSpellScript(spell_injured_razor_hill_grunt_cancel_feign_death);
}
