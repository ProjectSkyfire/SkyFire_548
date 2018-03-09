/*
* Copyright (C) 2011-2018 Project SkyFire <http://www.projectskyfire.org/>
* Copyright (C) 2008-2018 TrinityCore <http://www.trinitycore.org/>
* Copyright (C) 2005-2018 MaNGOS <https://getmangos.com/>
*
* This program is free software; you can redistribute it and/or modify it
* under the terms of the GNU General Public License as published by the
* Free Software Foundation; either version 3 of the License, or (at your
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

// todo: cleanup and code rest

#include "Creature.h"
#include "GameObject.h"
#include "GameObjectAI.h"
#include "Object.h"
#include "ObjectMgr.h"
#include "Player.h"
#include "ScriptMgr.h"
#include "ScriptedCreature.h"
#include "Spell.h"
#include "SpellAuraEffects.h"
#include "SpellScript.h"
#include "Unit.h"
#include "UnitAI.h"
#include "timeless_isle.h"

enum TimeLostShrine
{
    // Says
    SAY_BLESSING_NIUZAO                 = 0,
    SAY_BLESSING_YULON                  = 1,
    SAY_BLESSING_CHI_JI                 = 2,
    SAY_BLESSING_XUEN                   = 3,
    SAY_CORRUPTION_ORDOS                = 4,

    // Spells
    SPELL_FORTITUDE_OF_NIUZAO           = 147281,
    SPELL_WISDOM_OF_YULON               = 147282,
    SPELL_CHI_JIS_HOPE                  = 147283,
    SPELL_XUENS_STRENGTH                = 147284,
    SPELL_ORDOS_BURNING_SACRIFICE       = 147285,
};

class go_time_lost_shrine_ti : public GameObjectScript
{
public:
    go_time_lost_shrine_ti() : GameObjectScript("go_time_lost_shrine_ti") { }

    struct go_time_lost_shrine_tiAI : public GameObjectAI
    {
        go_time_lost_shrine_tiAI(GameObject* go) : GameObjectAI(go) { }

        bool OnGossipHello(Player* player, GameObject* go)
        {
        	if (used == false)
        	{
                go->SetFlag(GAMEOBJECT_FIELD_FLAGS, GO_FLAG_INTERACT_COND);

            	used = true;

        		Choice = urand(1, 5);
        	}

        	if (player->HasAura(SPELL_FORTITUDE_OF_NIUZAO))
        		player->RemoveAurasDueToSpell(SPELL_FORTITUDE_OF_NIUZAO);

        	if (player->HasAura(SPELL_WISDOM_OF_YULON))
            	player->RemoveAurasDueToSpell(SPELL_WISDOM_OF_YULON);

        	if (player->HasAura(SPELL_CHI_JIS_HOPE))
            	player->RemoveAurasDueToSpell(SPELL_CHI_JIS_HOPE);

        	if (player->HasAura(SPELL_XUENS_STRENGTH))
            	player->RemoveAurasDueToSpell(SPELL_XUENS_STRENGTH);

        	Creature* trigger = go->FindNearestCreature(NPC_TIME_LOST_SHRINE_TRIGGER, 5.0f); // An npc needs to be spawned in the same coordinates for the shrine

            return true;
        }

		void UpdateAI(uint32 diff)
        {
            if (used == false)
                return;

            if (BackToUse <= diff)
            {
                go->RemoveFlag(GAMEOBJECT_FIELD_FLAGS, GO_FLAG_INTERACT_COND);
                used = false; // this will stop/prevent it from updating it
            }

            else
            	BackToUse -= diff;
        }

    private:
    	uint32 BackToUse;

    	uint8 Choice;

    	bool used;
    };

	GameObjectAI* GetAI(GameObject* go) const 
    {
        return new go_time_lost_shrine_tiAI(go);
    }
};

class go_gleaming_crane_statue_ti : public GameObjectScript
{
public:
    go_gleaming_crane_statue_ti() : GameObjectScript("go_gleaming_crane_statue_ti") { }

    struct go_gleaming_crane_statue_tiAI : public GameObjectAI
    {
        go_gleaming_crane_statue_tiAI(GameObject* go) : GameObjectAI(go) { }

        uint64 playerGUID;
        uint32 WingsTimer;

		bool OnGossipHello(Player* player, GameObject* go)
        {
            player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, "Touch the statue.", GOSSIP_SENDER_MAIN, 1);

            player->SEND_GOSSIP_MENU(player->GetGossipTextId(go), go->GetGUID());

            return true;
        }

		bool OnGossipSelect(Player* player, GameObject* go, uint32 sender, uint32 action)
        {
        	player->PlayerTalkClass->ClearMenus();
        	player->CLOSE_GOSSIP_MENU();

        	switch (action)
        	{
        		case 1:
        			player->CastSpell(player, 144387, true); // knockback in the air
        			playerGUID = player->GetGUID();
        			used = true;
        			WingsTimer = 6000;
        			break;
        		default:
        			break;
        	}

        	return true;
        }

		void UpdateAI(uint32 diff)
        {
            if (used == false)
            	return;

            if (WingsTimer <= diff)
            {
                if (Player* player = ObjectAccessor::GetPlayer(*go, playerGUID))
                {
                	player->CastSpell(player, 144385, true);
                	used = false;
                }
            }

            else 
                WingsTimer -= diff;
        }

    private:
        bool used;
    };

	GameObjectAI* GetAI(GameObject* go) const 
    {
        return new go_gleaming_crane_statue_tiAI(go);
    }
};

class spell_timeless_isle_crane_wings : public SpellScriptLoader
{
public:
    spell_timeless_isle_crane_wings() : SpellScriptLoader("spell_timeless_isle_crane_wings") { }

    class spell_timeless_isle_crane_wings_AuraScript : public AuraScript
    {
        PrepareAuraScript(spell_timeless_isle_crane_wings_AuraScript);

        void OnApply(AuraEffect const* /*aurEff*/, AuraEffectHandleModes /*mode*/)
        {
            if (Unit* caster = GetCaster())
             	caster->CastSpell(caster, 144391, true);
        }

        void OnRemove(AuraEffect const* /*aurEff*/, AuraEffectHandleModes /*mode*/)
        {
            if (Unit* caster = GetCaster())
            	caster->CastSpell(caster, 148162, true);
        }

		void Register()
        {
            OnEffectApply += AuraEffectApplyFn(spell_timeless_isle_crane_wings_AuraScript::OnApply, EFFECT_0, SPELL_AURA_FEATHER_FALL, AURA_EFFECT_HANDLE_REAL);
            OnEffectRemove += AuraEffectRemoveFn(spell_timeless_isle_crane_wings_AuraScript::OnRemove, EFFECT_0, SPELL_AURA_DUMMY, AURA_EFFECT_HANDLE_REAL);
        }
    };

	AuraScript* GetAuraScript() const 
    {
        return new spell_timeless_isle_crane_wings_AuraScript();
    }
};

class spell_timeless_isle_cauterize : public SpellScriptLoader
{
public:
    spell_timeless_isle_cauterize() : SpellScriptLoader("spell_timeless_isle_cauterize") { }

    class spell_timeless_isle_cauterize_AuraScript : public AuraScript
    {
        PrepareAuraScript(spell_timeless_isle_cauterize_AuraScript);

        void OnPeriodic(AuraEffect const* /*aurEff*/)
        {
            GetCaster()->SetMaxHealth(GetCaster()->GetHealthPct() - 1.0f);
        }

		void Register()
        {
            OnEffectPeriodic += AuraEffectPeriodicFn(spell_timeless_isle_cauterize_AuraScript::OnPeriodic, EFFECT_0, SPELL_AURA_PERIODIC_DUMMY);
        }
    };

	AuraScript* GetAuraScript() const 
    {
        return new spell_timeless_isle_cauterize_AuraScript();
    }
};

class spell_timeless_isle_burning_fury : public SpellScriptLoader
{
public:
    spell_timeless_isle_burning_fury() : SpellScriptLoader("spell_timeless_isle_burning_fury") { }

    class spell_timeless_isle_burning_fury_AuraScript : public AuraScript
    {
        PrepareAuraScript(spell_timeless_isle_burning_fury_AuraScript);

        void OnPeriodic(AuraEffect const* /*aurEff*/)
        {
            GetCaster()->DealDamage(GetCaster(), 50000, NULL, SELF_DAMAGE, SPELL_SCHOOL_MASK_FIRE);
        }

		void Register()
        {
            OnEffectPeriodic += AuraEffectPeriodicFn(spell_timeless_isle_burning_fury_AuraScript::OnPeriodic, EFFECT_0, SPELL_AURA_PERIODIC_DUMMY);
        }
    };

	AuraScript* GetAuraScript() const 
    {
        return new spell_timeless_isle_burning_fury_AuraScript();
    }
};

enum EmperorEvent
{
    EVENT_EMPEROR_ARRANGE_BOSS = 1
};

// Emperor Shaohao #73303#
class npc_emperor_shaohao : public CreatureScript
{
public:
    npc_emperor_shaohao() : CreatureScript("npc_emperor_shaohao") { }

    CreatureAI* GetAI(Creature* creature) const
    {
        return new npc_emperor_shaohaoAI(creature);
    }

    struct npc_emperor_shaohaoAI : public ScriptedAI
    {
        npc_emperor_shaohaoAI(Creature* creature) : ScriptedAI(creature) { }

        void Reset()
        {
            me->setActive(true);
            events.Reset();
            _yulon = true;
            _xuen = false;
            _chiji = false;
            _niuzao = false;
            events.ScheduleEvent(EVENT_EMPEROR_ARRANGE_BOSS, 5000);
        }

        void DoAction(const int32 action)
        {
            switch (action)
            {
                case ACTION_XUEN:
                    _xuen = true;
                    break;
                case ACTION_CHIJI:
                    _chiji = true;
                    break;
                case ACTION_NIUZAO:
                    _niuzao = true;
                    break;
                case ACTION_YULON:
                    _yulon = true;
                    break;
            }

            events.ScheduleEvent(EVENT_EMPEROR_ARRANGE_BOSS, 8500);
        }

        void UpdateAI(const uint32 diff)
        {
            events.Update(diff);

            switch (events.ExecuteEvent())
            {
                case EVENT_EMPEROR_ARRANGE_BOSS:
                {
                    if (_yulon)
                    {
                        if (Creature* yulon = me->FindNearestCreature(BOSS_YU_LON, 300.0f, true))
                        {
                            yulon->GetMotionMaster()->MovePoint(1, _timelessIsleMiddle);
                            me->AI()->Talk(EMPEROR_TALK_INTRO_YULON);
                        }
                        else
                            _xuen = true;

                        _yulon = false;
                    }
                    if (_xuen)
                    {
                        if (Creature* xuen = me->FindNearestCreature(BOSS_XUEN, 300.0f, true))
                        {
                            xuen->GetMotionMaster()->MovePoint(1, _timelessIsleMiddle);
                            me->AI()->Talk(EMPEROR_TALK_INTRO_XUEN);
                        }
                        else
                            _chiji = true;

                        _xuen = false;
                    }
                    if (_chiji)
                    {
                        if (Creature* chiji = me->FindNearestCreature(BOSS_CHI_JI, 300.0f, true))
                        {
                            chiji->GetMotionMaster()->MovePoint(1, _timelessIsleMiddle);
                            me->AI()->Talk(EMPEROR_TALK_INTRO_CHIJI);
                        }
                        else
                            _niuzao = true;

                        _chiji = false;
                    }
                    if (_niuzao)
                    {
                        if (Creature* niuzao = me->FindNearestCreature(BOSS_NIUZAO, 300.0f, true))
                        {
                            niuzao->GetMotionMaster()->MovePoint(1, _timelessIsleMiddle);
                            me->AI()->Talk(EMPEROR_TALK_INTRO_NIUZAO);
                        }
                        else
                            _yulon = true;

                        _niuzao = false;
                    }

                    events.ScheduleEvent(EVENT_EMPEROR_ARRANGE_BOSS, 5000);
                    break;
                }
                default:
                    break;
            }
        }

        private:
            EventMap events;
            bool _yulon;
            bool _xuen;
            bool _chiji;
            bool _niuzao;
    };
};

/********* Timeless Isle Npcs *********/

// Chelon #72045# - Great Turtle Furyshell #73161# - Elder Great Turtle #72765#
// Great Turtle #72764# - Great Turtle Hatchling #72763#
class npc_timeless_turtles : public CreatureScript
{
public:

    npc_timeless_turtles() : CreatureScript("npc_timeless_turtles")
    {
    }

    struct npc_timeless_turtlesAI : public ScriptedAI
    {
        npc_timeless_turtlesAI(Creature* creature) : ScriptedAI(creature)
        {
        }

        uint32 uiGeyserTimer;
        uint32 uiShellSpinTimer;
        uint32 uiSnappingBiteTimer;

        void Reset() 
        {
            uiGeyserTimer = 8*IN_MILLISECONDS;
            uiShellSpinTimer = 10*IN_MILLISECONDS;
            uiSnappingBiteTimer = 6*IN_MILLISECONDS;
        }

        void EnterCombat(Unit* /*who*/) {}

        void UpdateAI(const uint32 diff)
        {
            if (!UpdateVictim())
                return;

            if (me->HasUnitState(UNIT_STATE_CASTING))
                return;

            switch (me->GetEntry())
            {
                case 72045:
                {
                    if (uiGeyserTimer <= diff)
                    {
                        me->CastSpell(me->GetVictim(), SPELL_GEYSER_1);
                        uiGeyserTimer = 8*IN_MILLISECONDS;
                    }
                    else uiGeyserTimer -= diff;

                    if (uiShellSpinTimer <= diff)
                    {
                        me->CastSpell(me->GetVictim(), SPELL_SHELL_SPINE_1);
                        uiShellSpinTimer = 10*IN_MILLISECONDS;
                    }
                    else uiShellSpinTimer -= diff;

                    if (uiSnappingBiteTimer <= diff)
                    {
                        me->CastSpell(me->GetVictim(), SPELL_SNAPPING_BITE_1);
                        uiSnappingBiteTimer = 6*IN_MILLISECONDS;
                    }
                    else uiSnappingBiteTimer -= diff;

                    break;
                }

                case 73161:
                {
                    if (uiGeyserTimer <= diff)
                    {
                        me->CastSpell(me->GetVictim(), SPELL_GEYSER_1);
                        uiGeyserTimer = 8*IN_MILLISECONDS;
                    }
                    else uiGeyserTimer -= diff;

                    if (uiShellSpinTimer <= diff)
                    {
                        me->CastSpell(me->GetVictim(), SPELL_SHELL_SPINE_1);
                        uiShellSpinTimer = 10*IN_MILLISECONDS;
                    }
                    else uiShellSpinTimer -= diff;

                    if (uiSnappingBiteTimer <= diff)
                    {
                        me->CastSpell(me->GetVictim(), SPELL_SNAPPING_BITE_1);
                        uiSnappingBiteTimer = 6*IN_MILLISECONDS;
                    }
                    else uiSnappingBiteTimer -= diff;

                    break;
                }

                case 72765:
                {
                    if (uiGeyserTimer <= diff)
                    {
                        me->CastSpell(me->GetVictim(), SPELL_GEYSER_2);
                        uiGeyserTimer = 8*IN_MILLISECONDS;
                    }
                    else uiGeyserTimer -= diff;

                    if (uiShellSpinTimer <= diff)
                    {
                        me->CastSpell(me->GetVictim(), SPELL_SHELL_SPINE_2);
                        uiShellSpinTimer = 10*IN_MILLISECONDS;
                    }
                    else uiShellSpinTimer -= diff;

                    if (uiSnappingBiteTimer <= diff)
                    {
                        me->CastSpell(me->GetVictim(), SPELL_SNAPPING_BITE_2);
                        uiSnappingBiteTimer = 6*IN_MILLISECONDS;
                    }
                    else uiSnappingBiteTimer -= diff;

                    break;
                }

                case 72764:
                {
                    if (uiShellSpinTimer <= diff)
                    {
                        me->CastSpell(me->GetVictim(), SPELL_SHELL_SPINE_1);
                        uiShellSpinTimer = 10*IN_MILLISECONDS;
                    }
                    else uiShellSpinTimer -= diff;

                    if (uiSnappingBiteTimer <= diff)
                    {
                        me->CastSpell(me->GetVictim(), SPELL_SNAPPING_BITE_3);
                        uiSnappingBiteTimer = 6*IN_MILLISECONDS;
                    }
                    else uiSnappingBiteTimer -= diff;

                    break;
                }

                case 72763:
                {
                    if (uiSnappingBiteTimer <= diff)
                    {
                        me->CastSpell(me->GetVictim(), SPELL_SNAPPING_BITE_3);
                        uiSnappingBiteTimer = 6*IN_MILLISECONDS;
                    }
                    else uiSnappingBiteTimer -= diff;

                    break;
                }

                default:
                    break;
            }

            DoMeleeAttackIfReady();
        }
    };

    CreatureAI* GetAI(Creature* creature) const 
    {
        return new npc_timeless_turtlesAI(creature);
    }
};

// Tushui Sentry #73361# - Huojin Sentry #73362# - Michi Windblossom #71940# - Chi-Ro the Skytamer #71939#
// Celestial Guard #73343# - Captain Huan #73347#
class npc_timeless_faction_sentries : public CreatureScript
{
public:

    npc_timeless_faction_sentries() : CreatureScript("npc_timeless_faction_sentries")
    {
    }

    struct npc_timeless_faction_sentriesAI : public ScriptedAI
    {
        npc_timeless_faction_sentriesAI(Creature* creature) : ScriptedAI(creature)
        {
        }

        uint32 uiIncapacitatingShoutTimer;
        uint32 uiMortalStrikeTimer;
        uint32 uiNetTimer;
        uint32 uiSnapKickTimer;

        void Reset() 
        {
            uiIncapacitatingShoutTimer = 9*IN_MILLISECONDS;
            uiMortalStrikeTimer = 4*IN_MILLISECONDS;
            uiNetTimer = 1*IN_MILLISECONDS;
            uiSnapKickTimer = 7*IN_MILLISECONDS;
        }

        void EnterCombat(Unit* /*who*/) {}

        void UpdateAI(const uint32 diff)
        {
            if (!UpdateVictim())
                return;

            if (me->HasUnitState(UNIT_STATE_CASTING))
                return;

            switch (me->GetEntry())
            {
                case 73361:
                {
                    if (uiIncapacitatingShoutTimer <= diff)
                    {
                        me->CastSpell(me->GetVictim(), SPELL_INCAPACITATING_SHOUT);
                        uiIncapacitatingShoutTimer = 25*IN_MILLISECONDS;
                    }
                    else uiIncapacitatingShoutTimer -= diff;

                    if (uiMortalStrikeTimer <= diff)
                    {
                        me->CastSpell(me->GetVictim(), SPELL_MORTAL_STRIKE);
                        uiMortalStrikeTimer = 8*IN_MILLISECONDS;
                    }
                    else uiMortalStrikeTimer -= diff;

                    if (uiNetTimer <= diff)
                    {
                        me->CastSpell(me->GetVictim(), SPELL_NET);
                        uiNetTimer = 15*IN_MILLISECONDS;
                    }
                    else uiNetTimer -= diff;

                    if (uiSnapKickTimer <= diff)
                    {
                        me->CastSpell(me->GetVictim(), SPELL_SNAP_KICK);
                        uiSnapKickTimer = 20*IN_MILLISECONDS;
                    }
                    else uiSnapKickTimer -= diff;

                    break;
                }

                case 73362:
                {
                    if (uiIncapacitatingShoutTimer <= diff)
                    {
                        me->CastSpell(me->GetVictim(), SPELL_INCAPACITATING_SHOUT);
                        uiIncapacitatingShoutTimer = 25*IN_MILLISECONDS;
                    }
                    else uiIncapacitatingShoutTimer -= diff;

                    if (uiMortalStrikeTimer <= diff)
                    {
                        me->CastSpell(me->GetVictim(), SPELL_MORTAL_STRIKE);
                        uiMortalStrikeTimer = 8*IN_MILLISECONDS;
                    }
                    else uiMortalStrikeTimer -= diff;

                    if (uiNetTimer <= diff)
                    {
                        me->CastSpell(me->GetVictim(), SPELL_NET);
                        uiNetTimer = 15*IN_MILLISECONDS;
                    }
                    else uiNetTimer -= diff;

                    if (uiSnapKickTimer <= diff)
                    {
                        me->CastSpell(me->GetVictim(), SPELL_SNAP_KICK);
                        uiSnapKickTimer = 20*IN_MILLISECONDS;
                    }
                    else uiSnapKickTimer -= diff;

                    break;
                }

                case 71940:
                {
                    if (uiMortalStrikeTimer <= diff)
                    {
                        me->CastSpell(me->GetVictim(), SPELL_MORTAL_STRIKE);
                        uiMortalStrikeTimer = 8*IN_MILLISECONDS;
                    }
                    else uiMortalStrikeTimer -= diff;

                    if (uiNetTimer <= diff)
                    {
                        me->CastSpell(me->GetVictim(), SPELL_NET);
                        uiNetTimer = 15*IN_MILLISECONDS;
                    }
                    else uiNetTimer -= diff;

                    if (uiSnapKickTimer <= diff)
                    {
                        me->CastSpell(me->GetVictim(), SPELL_SNAP_KICK);
                        uiSnapKickTimer = 20*IN_MILLISECONDS;
                    }
                    else uiSnapKickTimer -= diff;

                    break;
                }

                case 71939:
                {
                    if (uiMortalStrikeTimer <= diff)
                    {
                        me->CastSpell(me->GetVictim(), SPELL_MORTAL_STRIKE);
                        uiMortalStrikeTimer = 8*IN_MILLISECONDS;
                    }
                    else uiMortalStrikeTimer -= diff;

                    if (uiNetTimer <= diff)
                    {
                        me->CastSpell(me->GetVictim(), SPELL_NET);
                        uiNetTimer = 15*IN_MILLISECONDS;
                    }
                    else uiNetTimer -= diff;

                    if (uiSnapKickTimer <= diff)
                    {
                        me->CastSpell(me->GetVictim(), SPELL_SNAP_KICK);
                        uiSnapKickTimer = 20*IN_MILLISECONDS;
                    }
                    else uiSnapKickTimer -= diff;

                    break;
                }

                case 73343:
                {
                    if (uiIncapacitatingShoutTimer <= diff)
                    {
                        me->CastSpell(me->GetVictim(), SPELL_INCAPACITATING_SHOUT);
                        uiIncapacitatingShoutTimer = 25*IN_MILLISECONDS;
                    }
                    else uiIncapacitatingShoutTimer -= diff;

                    if (uiMortalStrikeTimer <= diff)
                    {
                        me->CastSpell(me->GetVictim(), SPELL_MORTAL_STRIKE);
                        uiMortalStrikeTimer = 8*IN_MILLISECONDS;
                    }
                    else uiMortalStrikeTimer -= diff;

                    if (uiNetTimer <= diff)
                    {
                        me->CastSpell(me->GetVictim(), SPELL_NET);
                        uiNetTimer = 15*IN_MILLISECONDS;
                    }
                    else uiNetTimer -= diff;

                    if (uiSnapKickTimer <= diff)
                    {
                        me->CastSpell(me->GetVictim(), SPELL_SNAP_KICK);
                        uiSnapKickTimer = 20*IN_MILLISECONDS;
                    }
                    else uiSnapKickTimer -= diff;

                    break;
                }

                case 73347:
                {
                    if (uiIncapacitatingShoutTimer <= diff)
                    {
                        me->CastSpell(me->GetVictim(), SPELL_INCAPACITATING_SHOUT);
                        uiIncapacitatingShoutTimer = 25*IN_MILLISECONDS;
                    }
                    else uiIncapacitatingShoutTimer -= diff;

                    if (uiMortalStrikeTimer <= diff)
                    {
                        me->CastSpell(me->GetVictim(), SPELL_MORTAL_STRIKE);
                        uiMortalStrikeTimer = 8*IN_MILLISECONDS;
                    }
                    else uiMortalStrikeTimer -= diff;

                    if (uiNetTimer <= diff)
                    {
                        me->CastSpell(me->GetVictim(), SPELL_NET);
                        uiNetTimer = 15*IN_MILLISECONDS;
                    }
                    else uiNetTimer -= diff;

                    if (uiSnapKickTimer <= diff)
                    {
                        me->CastSpell(me->GetVictim(), SPELL_SNAP_KICK);
                        uiSnapKickTimer = 20*IN_MILLISECONDS;
                    }
                    else uiSnapKickTimer -= diff;

                    break;
                }

                default:
                    break;
            }

            DoMeleeAttackIfReady();
        }
    };

    CreatureAI* GetAI(Creature* creature) const 
    {
        return new npc_timeless_faction_sentriesAI(creature);
    }
};

// Imperial Python #73163# - Death Adder #72841#
class npc_timeless_adders : public CreatureScript
{
public:

    npc_timeless_adders() : CreatureScript("npc_timeless_adders")
    {
    }

    struct npc_timeless_addersAI : public ScriptedAI
    {
        npc_timeless_addersAI(Creature* creature) : ScriptedAI(creature)
        {
        }

        uint32 uiHugeFangTimer;

        void Reset() 
        {
            uiHugeFangTimer = 1*IN_MILLISECONDS;
        }

        void EnterCombat(Unit* /*who*/) {}

        void UpdateAI(const uint32 diff)
        {
            if (!UpdateVictim())
                return;

            if (me->HasUnitState(UNIT_STATE_CASTING))
                return;

            switch (me->GetEntry())
            {
                case 73163:
                {
                    if (uiHugeFangTimer <= diff)
                    {
                        me->CastSpell(me->GetVictim(), SPELL_HUGE_FANG_1);
                        uiHugeFangTimer = 1*IN_MILLISECONDS;
                    }
                    else uiHugeFangTimer -= diff;

                    break;
                }

                case 72841:
                {
                    if (uiHugeFangTimer <= diff)
                    {
                        me->CastSpell(me->GetVictim(), SPELL_HUGE_FANG_2);
                        uiHugeFangTimer = 1*IN_MILLISECONDS;
                    }
                    else uiHugeFangTimer -= diff;

                    break;
                }

                default:
                    break;
            }

            DoMeleeAttackIfReady();
        }
    };

    CreatureAI* GetAI(Creature* creature) const 
    {
        return new npc_timeless_addersAI(creature);
    }
};

// Ironfur Steelhorn #73160# - Ironfur Herdling #72842# - Ironfur Grazer #72843#
// Ironfur Great Bull #72844#
class npc_timeless_yaks : public CreatureScript
{
public:

    npc_timeless_yaks() : CreatureScript("npc_timeless_yaks")
    {
    }

    struct npc_timeless_yaksAI : public ScriptedAI
    {
        npc_timeless_yaksAI(Creature* creature) : ScriptedAI(creature)
        {
        }

        uint32 uiHeadbuttTimer;
        uint32 uiIronFurTimer;
        uint32 uiOxChargeTimer;
        uint32 uiOxChargeTriggerTimer;
        bool oxCharge;

        void Reset() 
        {
            oxCharge = false;
            uiHeadbuttTimer = 8*IN_MILLISECONDS;
            uiIronFurTimer = 5*IN_MILLISECONDS;
            uiOxChargeTimer = 10*IN_MILLISECONDS;
            uiOxChargeTriggerTimer = 11*IN_MILLISECONDS;
        }

        void EnterCombat(Unit* /*who*/) {}

        void UpdateAI(const uint32 diff)
        {
            if (!UpdateVictim())
                return;

            if (me->HasUnitState(UNIT_STATE_CASTING))
                return;
            
            switch (me->GetEntry())
            {
                case 73160:
                {
                    if (!oxCharge)
                    {
                        if (uiHeadbuttTimer <= diff)
                        {
                            me->CastSpell(me->GetVictim(), SPELL_HEADBUTT_1);
                            uiHeadbuttTimer = 8*IN_MILLISECONDS;
                        }
                        else uiHeadbuttTimer -= diff;

                        if (uiIronFurTimer <= diff)
                        {
                            me->CastSpell(me, SPELL_IRON_FUR);
                            uiIronFurTimer = 25*IN_MILLISECONDS;
                        }
                        else uiIronFurTimer -= diff;
                    }

                    if (uiOxChargeTimer <= diff)
                    {
                        oxCharge = true;
                        me->CastSpell(SelectTarget(SELECT_TARGET_RANDOM), SPELL_OX_CHARGE);
                        uiOxChargeTimer = 10*IN_MILLISECONDS;
                    }
                    else uiOxChargeTimer -= diff;

                    if (uiOxChargeTriggerTimer <= diff)
                    {
                        me->CastSpell(me, SPELL_OX_CHARGE_TRIGGER);
                        oxCharge = false;
                        uiOxChargeTriggerTimer = 10*IN_MILLISECONDS;
                    }
                    else uiOxChargeTriggerTimer -= diff;

                    break;
                }

                case 72842:
                {
                    if (uiHeadbuttTimer <= diff)
                    {
                        me->CastSpell(me->GetVictim(), SPELL_HEADBUTT_2);
                        uiHeadbuttTimer = 8*IN_MILLISECONDS;
                    }
                    else uiHeadbuttTimer -= diff;

                    break;
                }

                case 72843:
                {
                    if (uiHeadbuttTimer <= diff)
                    {
                        me->CastSpell(me->GetVictim(), SPELL_HEADBUTT_3);
                        uiHeadbuttTimer = 8*IN_MILLISECONDS;
                    }
                    else uiHeadbuttTimer -= diff;

                    if (uiIronFurTimer <= diff)
                    {
                        me->CastSpell(me, SPELL_IRON_FUR);
                        uiIronFurTimer = 16*IN_MILLISECONDS;
                    }
                    else uiIronFurTimer -= diff;

                    break;
                }

                case 72844:
                {
                    if (!oxCharge)
                    {
                        if (uiIronFurTimer <= diff)
                        {
                            me->CastSpell(me, SPELL_IRON_FUR);
                            uiIronFurTimer = 25*IN_MILLISECONDS;
                        }
                        else uiIronFurTimer -= diff;
                    }

                    if (uiOxChargeTimer <= diff)
                    {
                        oxCharge = true;
                        me->CastSpell(SelectTarget(SELECT_TARGET_RANDOM), SPELL_OX_CHARGE);
                        uiOxChargeTimer = 10*IN_MILLISECONDS;
                    }
                    else uiOxChargeTimer -= diff;

                    if (uiOxChargeTriggerTimer <= diff)
                    {
                        me->CastSpell(me, SPELL_OX_CHARGE_TRIGGER);
                        oxCharge = false;
                        uiOxChargeTriggerTimer = 10*IN_MILLISECONDS;
                    }
                    else uiOxChargeTriggerTimer -= diff;

                    break;
                }

                default:
                    break;
            }

            DoMeleeAttackIfReady();
        }
    };

    CreatureAI* GetAI(Creature* creature) const 
    {
        return new npc_timeless_yaksAI(creature);
    }
};

// Emerald Gander #73158# - Brilliant Windfeather #72762# - Windfeather Chick #71143#
// Fishgorged Crane #72095# - Windfeather Nestkeeper #72761#
class npc_timeless_cranes : public CreatureScript
{
public:

    npc_timeless_cranes() : CreatureScript("npc_timeless_cranes")
    {
    }

    struct npc_timeless_cranesAI : public ScriptedAI
    {
        npc_timeless_cranesAI(Creature* creature) : ScriptedAI(creature)
        {
        }

        uint32 uiGustOfWindTimer;
        uint32 uiHealingSongTimer;
        uint32 uiWindfeatherTimer;

        void Reset() 
        {
            uiGustOfWindTimer = 8*IN_MILLISECONDS;
            uiHealingSongTimer = 15*IN_MILLISECONDS;
            uiWindfeatherTimer = 1*IN_MILLISECONDS;
        }

        void EnterCombat(Unit* /*who*/) {}

        void UpdateAI(const uint32 diff)
        {
            if (!UpdateVictim())
                return;

            if (me->HasUnitState(UNIT_STATE_CASTING))
                return;

            switch (me->GetEntry())
            {
                case 73158:
                {
                    if (uiGustOfWindTimer <= diff)
                    {
                        me->CastSpell(me->GetVictim(), SPELL_GUST_OF_WIND);
                        uiGustOfWindTimer = 8*IN_MILLISECONDS;
                    }
                    else uiGustOfWindTimer -= diff;

                    if (uiHealingSongTimer <= diff)
                    {
                        me->CastSpell(me, SPELL_HEALING_SONG);
                        uiHealingSongTimer = 18*IN_MILLISECONDS;
                    }
                    else uiHealingSongTimer -= diff;

                    if (uiWindfeatherTimer <= diff)
                    {
                        me->CastSpell(me, SPELL_WINDFEATHER);
                        uiWindfeatherTimer = 15*IN_MILLISECONDS;
                    }
                    else uiWindfeatherTimer -= diff;

                    break;
                }

                case 72762:
                {
                    if (uiGustOfWindTimer <= diff)
                    {
                        me->CastSpell(me->GetVictim(), SPELL_GUST_OF_WIND);
                        uiGustOfWindTimer = 8*IN_MILLISECONDS;
                    }
                    else uiGustOfWindTimer -= diff;

                    if (uiWindfeatherTimer <= diff)
                    {
                        me->CastSpell(me, SPELL_WINDFEATHER);
                        uiWindfeatherTimer = 13*IN_MILLISECONDS;
                    }
                    else uiWindfeatherTimer -= diff;

                    break;
                }

                case 71143:
                {
                    if (uiWindfeatherTimer <= diff)
                    {
                        me->CastSpell(me, SPELL_WINDFEATHER);
                        uiWindfeatherTimer = 10*IN_MILLISECONDS;
                    }
                    else uiWindfeatherTimer -= diff;

                    break;
                }

                case 72095:
                {
                    if (uiWindfeatherTimer <= diff)
                    {
                        me->CastSpell(me, SPELL_WINDFEATHER);
                        uiWindfeatherTimer = 10*IN_MILLISECONDS;
                    }
                    else uiWindfeatherTimer -= diff;

                    break;
                }

                case 72761:
                {
                    if (uiHealingSongTimer <= diff)
                    {
                        me->CastSpell(me, SPELL_HEALING_SONG);
                        uiHealingSongTimer = 10*IN_MILLISECONDS;
                    }
                    else uiHealingSongTimer -= diff;

                    break;
                }

                default:
                    break;
            }

            DoMeleeAttackIfReady();
        }
    };

    CreatureAI* GetAI(Creature* creature) const 
    {
        return new npc_timeless_cranesAI(creature);
    }
};

// Gu'chi the Swarmbringer #72909# - Spotted Swarmer #72908#
class npc_timeless_silkworms : public CreatureScript
{
public:

    npc_timeless_silkworms() : CreatureScript("npc_timeless_silkworms")
    {
    }

    struct npc_timeless_silkwormsAI : public ScriptedAI
    {
        npc_timeless_silkwormsAI(Creature* creature) : ScriptedAI(creature)
        {
        }

        uint32 uiVenomSpitTimer;
        uint32 uiPoisonSpitTimer;

        void Reset() 
        {
            uiVenomSpitTimer = 7*IN_MILLISECONDS;
            uiPoisonSpitTimer = urand(7, 12)*IN_MILLISECONDS;
        }

        void EnterCombat(Unit* /*who*/) {}

        void UpdateAI(const uint32 diff)
        {
            if (!UpdateVictim())
                return;

            if (me->HasUnitState(UNIT_STATE_CASTING))
                return;

            switch (me->GetEntry())
            {
                case 72909:
                {
                    if (uiVenomSpitTimer <= diff)
                    {
                        me->CastSpell(me->GetVictim(), SPELL_VENOM_SPIT);
                        uiVenomSpitTimer = 10*IN_MILLISECONDS;
                    }
                    else uiVenomSpitTimer -= diff;

                    break;
                }

                case 72908:
                {
                    if (uiPoisonSpitTimer <= diff)
                    {
                        me->CastSpell(me->GetVictim(), SPELL_POISON_SPIT);
                        uiPoisonSpitTimer = urand(7, 12)*IN_MILLISECONDS;
                    }
                    else uiPoisonSpitTimer -= diff;

                    break;
                }

                default:
                    break;
            }

            DoMeleeAttackIfReady();
        }
    };

    CreatureAI* GetAI(Creature* creature) const 
    {
        return new npc_timeless_silkwormsAI(creature);
    }
};

// Ancient Spineclaw #72766# - Monstrous Spineclaw #73166#
class npc_timeless_crabs : public CreatureScript
{
public:

    npc_timeless_crabs() : CreatureScript("npc_timeless_crabs")
    {
    }

    struct npc_timeless_crabsAI : public ScriptedAI
    {
        npc_timeless_crabsAI(Creature* creature) : ScriptedAI(creature)
        {
        }

        uint32 uiClawFlurry;

        void Reset() 
        {
            uiClawFlurry = 5*IN_MILLISECONDS;
        }

        void EnterCombat(Unit* /*who*/) {}

        void UpdateAI(const uint32 diff)
        {
            if (!UpdateVictim())
                return;

            if (me->HasUnitState(UNIT_STATE_CASTING))
                return;

            switch (me->GetEntry())
            {
                case 72766:
                {
                    if (uiClawFlurry <= diff)
                    {
                        me->CastSpell(me->GetVictim(), SPELL_CLAW_FLURRY_1);
                        uiClawFlurry = 8*IN_MILLISECONDS;
                    }
                    else uiClawFlurry -= diff;

                    break;
                }

                case 73166:
                {
                    if (uiClawFlurry <= diff)
                    {
                        me->CastSpell(me->GetVictim(), SPELL_CLAW_FLURRY_2);
                        uiClawFlurry = 8*IN_MILLISECONDS;
                    }
                    else uiClawFlurry -= diff;

                    break;
                }

                default:
                    break;
            }

            DoMeleeAttackIfReady();
        }
    };

    CreatureAI* GetAI(Creature* creature) const 
    {
        return new npc_timeless_crabsAI(creature);
    }
};

// Spectral Windwalker #73021# - Spectral Mistweaver #73025# - Spectral Brewmaster #73018#
class npc_timeless_spectrals : public CreatureScript
{
public:

    npc_timeless_spectrals() : CreatureScript("npc_timeless_spectrals")
    {
    }

    struct npc_timeless_spectralsAI : public ScriptedAI
    {
        npc_timeless_spectralsAI(Creature* creature) : ScriptedAI(creature)
        {
        }

        uint32 uiSpecSpellTimer;
        bool alementalNear;

        void Reset() 
        {
            me->CastSpell(me, SPELL_GHOST_VISUAL);

            uiSpecSpellTimer = 6*IN_MILLISECONDS;
            alementalNear = false;
        }

        void EnterCombat(Unit* /*who*/) {}

        void UpdateAI(const uint32 diff)
        {
            if (!alementalNear && me->FindNearestCreature(NPC_SKUNKY_ALEMENTAL, 5.0f))
            {
                me->CastSpell(me, SPELL_DRUNKEN_STUPOR);
                alementalNear = true;
            }

            if (!UpdateVictim())
                return;

            if (me->HasUnitState(UNIT_STATE_CASTING))
                return;

            switch (me->GetEntry())
            {
                case 73021:
                {
                    if (uiSpecSpellTimer <= diff)
                    {
                        me->CastSpell(me->GetVictim(), SPELL_CHI_BURST);
                        uiSpecSpellTimer = urand(6, 8)*IN_MILLISECONDS;
                    }
                    else uiSpecSpellTimer -= diff;

                    break;
                }

                case 73025:
                {
                    if (uiSpecSpellTimer <= diff)
                    {
                        me->CastSpell(me, SPELL_HEALING_MISTS);
                        uiSpecSpellTimer = urand(13, 15)*IN_MILLISECONDS;
                    }
                    else uiSpecSpellTimer -= diff;

                    break;
                }

                case 73018:
                {
                    if (uiSpecSpellTimer <= diff)
                    {
                        me->CastSpell(me->GetVictim(), SPELL_SPINNING_CRANE_KICK);
                        uiSpecSpellTimer = urand(9, 12)*IN_MILLISECONDS;
                    }
                    else uiSpecSpellTimer -= diff;

                    break;
                }

                default:
                    break;
            }

            DoMeleeAttackIfReady();
        }
    };

    CreatureAI* GetAI(Creature* creature) const 
    {
        return new npc_timeless_spectralsAI(creature);
    }
};

// Crag Stalker #72807# - Primal Stalker #72805# - Tsavo'ka #72808#
class npc_timeless_cats : public CreatureScript
{
public:

    npc_timeless_cats() : CreatureScript("npc_timeless_cats")
    {
    }

    struct npc_timeless_catsAI : public ScriptedAI
    {
        npc_timeless_catsAI(Creature* creature) : ScriptedAI(creature)
        {
        }

        uint32 uiDefensiveLeapTimer;
        uint32 uiSwipeTimer;
        uint32 uiPounceChargeTimer;
        uint32 uiPounceStunTimer;
        bool pounce;

        void Reset() 
        {
            uiDefensiveLeapTimer = 8*IN_MILLISECONDS;
            uiSwipeTimer = 2*IN_MILLISECONDS;
            uiPounceChargeTimer = 9*IN_MILLISECONDS;
            uiPounceStunTimer = 10*IN_MILLISECONDS;
            pounce = false;
        }

        void EnterCombat(Unit* /*who*/) {}

        void UpdateAI(const uint32 diff)
        {
            if (!UpdateVictim())
                return;

            if (me->HasUnitState(UNIT_STATE_CASTING))
                return;
            
            switch (me->GetEntry())
            {
                case 72807:
                {
                    if (!pounce)
                    {
                        if (uiSwipeTimer <= diff)
                        {
                            me->CastSpell(me->GetVictim(), SPELL_RENDING_SWIPE);
                            uiSwipeTimer = 4*IN_MILLISECONDS;
                        }
                        else uiSwipeTimer -= diff;
                    }

                    if (uiDefensiveLeapTimer <= diff)
                    {
                        pounce = true;
                        me->CastSpell(me, SPELL_DEFENSIVE_LEAP);
                        uiDefensiveLeapTimer = 12*IN_MILLISECONDS;
                    }
                    else uiDefensiveLeapTimer -= diff;

                    if (uiPounceChargeTimer <= diff)
                    {
                        me->CastSpell(me->GetVictim(), SPELL_POUNCE_CHARGE);
                        uiPounceChargeTimer = 12*IN_MILLISECONDS;
                    }
                    else uiPounceChargeTimer -= diff;

                    if (uiPounceStunTimer <= diff)
                    {
                        me->CastSpell(me->GetVictim(), SPELL_POUNCE_STUN);
                        pounce = false;
                        uiPounceStunTimer = 12*IN_MILLISECONDS;
                    }
                    else uiPounceStunTimer -= diff;

                    break;
                }

                case 72805:
                {
                    if (!pounce)
                    {
                        if (uiSwipeTimer <= diff)
                        {
                            me->CastSpell(me->GetVictim(), SPELL_GREATER_SWIPE);
                            uiSwipeTimer = 4*IN_MILLISECONDS;
                        }
                        else uiSwipeTimer -= diff;
                    }

                    if (uiDefensiveLeapTimer <= diff)
                    {
                        pounce = true;
                        me->CastSpell(me, SPELL_DEFENSIVE_LEAP);
                        uiDefensiveLeapTimer = 12*IN_MILLISECONDS;
                    }
                    else uiDefensiveLeapTimer -= diff;

                    if (uiPounceChargeTimer <= diff)
                    {
                        me->CastSpell(me->GetVictim(), SPELL_POUNCE_CHARGE);
                        uiPounceChargeTimer = 12*IN_MILLISECONDS;
                    }
                    else uiPounceChargeTimer -= diff;

                    if (uiPounceStunTimer <= diff)
                    {
                        me->CastSpell(me->GetVictim(), SPELL_POUNCE_STUN);
                        pounce = false;
                        uiPounceStunTimer = 12*IN_MILLISECONDS;
                    }
                    else uiPounceStunTimer -= diff;

                    break;
                }

                case 72808:
                {
                    if (!pounce)
                    {
                        if (uiSwipeTimer <= diff)
                        {
                            me->CastSpell(me->GetVictim(), SPELL_GREATER_SWIPE);
                            uiSwipeTimer = 4*IN_MILLISECONDS;
                        }
                        else uiSwipeTimer -= diff;
                    }

                    if (uiDefensiveLeapTimer <= diff)
                    {
                        pounce = true;
                        me->CastSpell(me, SPELL_DEFENSIVE_LEAP);
                        uiDefensiveLeapTimer = 12*IN_MILLISECONDS;
                    }
                    else uiDefensiveLeapTimer -= diff;

                    if (uiPounceChargeTimer <= diff)
                    {
                        me->CastSpell(me->GetVictim(), SPELL_POUNCE_CHARGE);
                        uiPounceChargeTimer = 12*IN_MILLISECONDS;
                    }
                    else uiPounceChargeTimer -= diff;

                    if (uiPounceStunTimer <= diff)
                    {
                        me->CastSpell(me->GetVictim(), SPELL_POUNCE_STUN);
                        pounce = false;
                        uiPounceStunTimer = 12*IN_MILLISECONDS;
                    }
                    else uiPounceStunTimer -= diff;

                    break;
                }

                default:
                    break;
            }

            DoMeleeAttackIfReady();
        }
    };

    CreatureAI* GetAI(Creature* creature) const 
    {
        return new npc_timeless_catsAI(creature);
    }
};

// Zhu-Gon the Sour #71919#
class npc_timeless_elementals : public CreatureScript
{
public:

    npc_timeless_elementals() : CreatureScript("npc_timeless_elementals")
    {
    }

    struct npc_timeless_elementalsAI : public ScriptedAI
    {
        npc_timeless_elementalsAI(Creature* creature) : ScriptedAI(creature)
        {
        }

        uint32 uiBrewBoltTimer;
        uint32 uiRainOfSkunkyAleTimer;
        uint32 uiSkunkyAleCrashTimer;

        void Reset() 
        {
            uiBrewBoltTimer = 1*IN_MILLISECONDS;
            uiRainOfSkunkyAleTimer = 14*IN_MILLISECONDS;
            uiSkunkyAleCrashTimer = 6*IN_MILLISECONDS;
        }

        void EnterCombat(Unit* /*who*/) {}

        void UpdateAI(const uint32 diff)
        {
            if (!UpdateVictim())
                return;

            if (me->HasUnitState(UNIT_STATE_CASTING))
                return;

            if (uiBrewBoltTimer <= diff)
            {
                me->CastSpell(me->GetVictim(), SPELL_BREW_BOLT);
                uiBrewBoltTimer = 3*IN_MILLISECONDS;
            }
            else uiBrewBoltTimer -= diff;

            if (uiRainOfSkunkyAleTimer <= diff)
            {
                me->CastSpell(me->GetVictim(), SPELL_RAIN_OF_SKUNKY_ALE);
                uiRainOfSkunkyAleTimer = 14*IN_MILLISECONDS;
            }
            else uiRainOfSkunkyAleTimer -= diff;

            if (uiSkunkyAleCrashTimer <= diff)
            {
                me->CastSpell(me->GetVictim(), SPELL_SKUNKY_ALE_CRASH);
                uiSkunkyAleCrashTimer = 6*IN_MILLISECONDS;
            }
            else uiSkunkyAleCrashTimer -= diff;

            DoMeleeAttackIfReady();
        }
    };

    CreatureAI* GetAI(Creature* creature) const 
    {
        return new npc_timeless_elementalsAI(creature);
    }
};

// Foreboding Flame #73162# - Spirit of Jadefire #72769#
class npc_timeless_spirits : public CreatureScript
{
public:

    npc_timeless_spirits() : CreatureScript("npc_timeless_spirits")
    {
    }

    struct npc_timeless_spiritsAI : public ScriptedAI
    {
        npc_timeless_spiritsAI(Creature* creature) : ScriptedAI(creature)
        {
        }

        uint32 uiFireBoltTimer;
        uint32 uiFlameStrikeTimer;

        void Reset() 
        {
            uiFireBoltTimer = 1*IN_MILLISECONDS;
            uiFlameStrikeTimer = 10*IN_MILLISECONDS;
        }

        void EnterCombat(Unit* /*who*/) {}

        void UpdateAI(const uint32 diff)
        {
            if (!UpdateVictim())
                return;

            if (me->HasUnitState(UNIT_STATE_CASTING))
                return;
            
            switch (me->GetEntry())
            {
                case 73162:
                {
                    if (uiFireBoltTimer <= diff)
                    {
                        me->CastSpell(me->GetVictim(), SPELL_SPIRITFIRE_BOLT);
                        uiFireBoltTimer = 5*IN_MILLISECONDS;
                    }
                    else uiFireBoltTimer -= diff;

                    if (uiFlameStrikeTimer <= diff)
                    {
                        me->CastSpell(me->GetVictim(), SPELL_SPIRITFLAME_STRIKE);
                        uiFlameStrikeTimer = 12*IN_MILLISECONDS;
                    }
                    else uiFlameStrikeTimer -= diff;

                    break;
                }

                case 72769:
                {
                    if (uiFireBoltTimer <= diff)
                    {
                        me->CastSpell(me->GetVictim(), SPELL_JADEFIRE_BOLT);
                        uiFireBoltTimer = 6*IN_MILLISECONDS;
                    }
                    else uiFireBoltTimer -= diff;

                    if (uiFlameStrikeTimer <= diff)
                    {
                        me->CastSpell(me->GetVictim(), SPELL_JADEFLAME_STRIKE);
                        uiFlameStrikeTimer = 15*IN_MILLISECONDS;
                    }
                    else uiFlameStrikeTimer -= diff;

                    break;
                }

                default:
                    break;
            }

            DoMeleeAttackIfReady();
        }
    };

    CreatureAI* GetAI(Creature* creature) const 
    {
        return new npc_timeless_spiritsAI(creature);
    }
};

// Eroded Cliffdweller #72809# - Golganarr #72970#
class npc_timeless_giants : public CreatureScript
{
public:

    npc_timeless_giants() : CreatureScript("npc_timeless_giants")
    {
    }

    struct npc_timeless_giantsAI : public ScriptedAI
    {
        npc_timeless_giantsAI(Creature* creature) : ScriptedAI(creature)
        {
        }

        uint32 uiBoulderTimer;
        uint32 uiStompTimer;
        uint32 uiFrenzyTimer;
        bool boulderOrStomp;

        void Reset() 
        {
            uiBoulderTimer = 0*IN_MILLISECONDS;
            uiStompTimer = 1*IN_MILLISECONDS;
            uiFrenzyTimer = 9*IN_MILLISECONDS;
            boulderOrStomp = false; // false = Boulder, true = Stomp
        }

        void EnterCombat(Unit* /*who*/) {}

        void UpdateAI(const uint32 diff)
        {
            if (!UpdateVictim())
                return;

            if (me->HasUnitState(UNIT_STATE_CASTING) || me->HasAura(SPELL_EXHAUSTION))
                return;

            if (!boulderOrStomp && me->GetVictim() && me->GetVictim()->GetDistance2d(me) <= 10.0f)
                boulderOrStomp = true;

            if (boulderOrStomp && me->GetVictim() && me->GetVictim()->GetDistance2d(me) > 10.0f)
                boulderOrStomp = false;

            switch (me->GetEntry())
            {
                case 72809:
                {
                    if (!boulderOrStomp)
                    {
                        if (uiBoulderTimer <= diff)
                        {
                            me->CastSpell(SelectTarget(SELECT_TARGET_RANDOM), SPELL_BOULDER);
                            uiBoulderTimer = 0*IN_MILLISECONDS;
                        }
                        else uiBoulderTimer -= diff;
                    }
                    else
                    {
                        if (uiStompTimer <= diff)
                        {
                            if (me->HasAura(SPELL_FRENZY))
                                me->CastSpell(me->GetVictim(), SPELL_FRENZY_STOMP);
                            else
                                me->CastSpell(me->GetVictim(), SPELL_STOMP);

                            uiStompTimer = 1*IN_MILLISECONDS;
                        }
                        else uiStompTimer -= diff;
                    }

                    if (uiFrenzyTimer <= diff)
                    {
                        me->CastSpell(me, SPELL_FRENZY);
                        uiFrenzyTimer = 16*IN_MILLISECONDS;
                    }
                    else uiFrenzyTimer -= diff;

                    break;
                }

                case 72970:
                {
                    if (!boulderOrStomp)
                    {
                        if (uiBoulderTimer <= diff)
                        {
                            me->CastSpell(SelectTarget(SELECT_TARGET_RANDOM), SPELL_BOULDER);
                            uiBoulderTimer = 0*IN_MILLISECONDS;
                        }
                        else uiBoulderTimer -= diff;
                    }
                    else
                    {
                        if (uiStompTimer <= diff)
                        {
                            if (me->HasAura(SPELL_FRENZY))
                                me->CastSpell(me->GetVictim(), SPELL_FRENZY_STOMP);
                            else
                                me->CastSpell(me->GetVictim(), SPELL_STOMP);

                            uiStompTimer = 1*IN_MILLISECONDS;
                        }
                        else uiStompTimer -= diff;
                    }

                    if (uiFrenzyTimer <= diff)
                    {
                        me->CastSpell(me, SPELL_FRENZY);
                        uiFrenzyTimer = 16*IN_MILLISECONDS;
                    }
                    else uiFrenzyTimer -= diff;

                    break;
                }

                default:
                    break;
            }

            DoMeleeAttackIfReady();
        }
    };

    CreatureAI* GetAI(Creature* creature) const 
    {
        return new npc_timeless_giantsAI(creature);
    }
};

// Gulp Frog #72777# - Bufo #72775#
class npc_timeless_frogs : public CreatureScript
{
public:

    npc_timeless_frogs() : CreatureScript("npc_timeless_frogs")
    {
    }

    struct npc_timeless_frogsAI : public ScriptedAI
    {
        npc_timeless_frogsAI(Creature* creature) : ScriptedAI(creature)
        {
        }

        void Reset() 
        {
            me->CastSpell(me, SPELL_TOXIC_SKIN);
        }

        void EnterCombat(Unit* /*who*/) {}

        void UpdateAI(const uint32 diff)
        {
            if (!UpdateVictim())
                return;

            DoMeleeAttackIfReady();
        }
    };

    CreatureAI* GetAI(Creature* creature) const 
    {
        return new npc_timeless_frogsAI(creature);
    }
};

// Crimsonscale Firestorm #72876# - Huolon #73167#
class npc_timeless_dragons : public CreatureScript
{
public:

    npc_timeless_dragons() : CreatureScript("npc_timeless_dragons")
    {
    }

    struct npc_timeless_dragonsAI : public ScriptedAI
    {
        npc_timeless_dragonsAI(Creature* creature) : ScriptedAI(creature)
        {
			//me->SetUnitMovementFlags
			me->SetCanFly(true);
			me->SetDisableGravity(true);
        }

        uint32 uiBreathTimer;
        uint32 uiBlossomTimer;

        void Reset() 
        {
            uiBreathTimer = 4*IN_MILLISECONDS;
            uiBlossomTimer = 10*IN_MILLISECONDS;
			me->SetCanFly(true);
			me->SetDisableGravity(true);
        }

        void EnterCombat(Unit* /*who*/) 
        {
			me->SetCanFly(true);
			me->SetDisableGravity(true);
        }

        void UpdateAI(const uint32 diff)
        {
            if (!UpdateVictim())
                return;

            if (me->HasUnitState(UNIT_STATE_CASTING))
                return;
            
            switch (me->GetEntry())
            {
                case 72876:
                {
                    if (uiBreathTimer <= diff)
                    {
                        me->CastSpell(me->GetVictim(), SPELL_FLAME_BREATH);
                        uiBreathTimer = 8*IN_MILLISECONDS;
                    }
                    else uiBreathTimer -= diff;

                    if (uiBlossomTimer <= diff)
                    {
                        me->CastSpell(me->GetVictim(), SPELL_FIRE_BLOSSOM);
                        uiBlossomTimer = 12*IN_MILLISECONDS;
                    }
                    else uiBlossomTimer -= diff;

                    break;
                }

                case 73167:
                {
                    if (uiBreathTimer <= diff)
                    {
                        me->CastSpell(me->GetVictim(), SPELL_LIGHTNING_BREATH);
                        uiBreathTimer = 8*IN_MILLISECONDS;
                    }
                    else uiBreathTimer -= diff;

                    if (uiBlossomTimer <= diff)
                    {
                        me->CastSpell(me->GetVictim(), SPELL_STORM_BLOSSOM);
                        uiBlossomTimer = 12*IN_MILLISECONDS;
                    }
                    else uiBlossomTimer -= diff;

                    break;
                }

                default:
                    break;
            }

            DoMeleeAttackIfReady();
        }
    };

    CreatureAI* GetAI(Creature* creature) const 
    {
        return new npc_timeless_dragonsAI(creature);
    }
};

// Blazebound Chanter #72897# - Eternal Kilnmaster #72896# - High Priest of Ordos #72898#
// Flintlord Gairan #73172# - Urdur the Cauterizer #73173# - Archiereus of Flame #73666#
class npc_ordos_minions : public CreatureScript
{
public:

    npc_ordos_minions() : CreatureScript("npc_ordos_minions")
    {
    }

    struct npc_ordos_minionsAI : public ScriptedAI
    {
        npc_ordos_minionsAI(Creature* creature) : ScriptedAI(creature)
        {
        }

        uint32 uiCauterizeTimer;
        uint32 uiConjureTimer;
        uint32 uiFireStormTimer;
        uint32 uiGreaterPyroblastTimer;
        uint32 uiBlazingBlowTimer;

        void Reset() 
        {
            uiCauterizeTimer = 15*IN_MILLISECONDS;
            uiConjureTimer = 8*IN_MILLISECONDS;
            uiFireStormTimer = 20*IN_MILLISECONDS;
            uiGreaterPyroblastTimer = 1*IN_MILLISECONDS;
            uiBlazingBlowTimer = 5*IN_MILLISECONDS;
        }

        void EnterCombat(Unit* /*who*/) 
        {
            switch (me->GetEntry())
            {
                case 72896:
                    me->CastSpell(me->GetVictim(), SPELL_FIERY_CHARGE);
                    break;

                case 73172:
                    me->CastSpell(me->GetVictim(), SPELL_FIERY_CHARGE);
                    break;

                default:
                    break;
            }
        }

        void UpdateAI(const uint32 diff)
        {
            if (!UpdateVictim())
                return;

            if (me->HasUnitState(UNIT_STATE_CASTING))
                return;

            switch (me->GetEntry())
            {
                case 72897:
                {
                    if (uiCauterizeTimer <= diff)
                    {
                        me->CastSpell(me, SPELL_CAUTERIZE);
                        uiCauterizeTimer = 35*IN_MILLISECONDS;
                    }
                    else uiCauterizeTimer -= diff;
                    
                    if (uiConjureTimer <= diff)
                    {
                        me->CastSpell(me, SPELL_CONJURE_FLARECORE_GOLEM);
                        uiConjureTimer = 25*IN_MILLISECONDS;
                    }
                    else uiConjureTimer -= diff;

                    if (uiFireStormTimer <= diff)
                    {
                        me->CastSpell(me->GetVictim(), SPELL_FIRE_STORM);
                        uiFireStormTimer = 25*IN_MILLISECONDS;
                    }
                    else uiFireStormTimer -= diff;

                    if (uiGreaterPyroblastTimer <= diff)
                    {
                        me->CastSpell(me->GetVictim(), SPELL_GREATER_PYROBLAST);
                        uiGreaterPyroblastTimer = 1500;
                    }
                    else uiGreaterPyroblastTimer -= diff;

                    break;
                }

                case 72896:
                {
                    if (uiBlazingBlowTimer <= diff)
                    {
                        me->CastSpell(me, SPELL_BLAZING_BLOW);
                        uiBlazingBlowTimer = 8*IN_MILLISECONDS;
                    }
                    else uiBlazingBlowTimer -= diff;

                    if (uiConjureTimer <= diff)
                    {
                        me->CastSpell(me, SPELL_CONJURE_ETERNAL_KILN);
                        uiConjureTimer = 25*IN_MILLISECONDS;
                    }
                    else uiConjureTimer -= diff;

                    break;
                }

                case 72898:
                {
                    if (uiCauterizeTimer <= diff)
                    {
                        me->CastSpell(me, SPELL_CAUTERIZE);
                        uiCauterizeTimer = 35*IN_MILLISECONDS;
                    }
                    else uiCauterizeTimer -= diff;
                    
                    if (uiConjureTimer <= diff)
                    {
                        me->CastSpell(me, RAND(SPELL_CONJURE_FLARECORE_GOLEM, SPELL_CONJURE_ETERNAL_KILN));
                        uiConjureTimer = 25*IN_MILLISECONDS;
                    }
                    else uiConjureTimer -= diff;

                    if (uiFireStormTimer <= diff)
                    {
                        me->CastSpell(me->GetVictim(), SPELL_FIRE_STORM);
                        uiFireStormTimer = 25*IN_MILLISECONDS;
                    }
                    else uiFireStormTimer -= diff;

                    if (uiBlazingBlowTimer <= diff)
                    {
                        me->CastSpell(me, SPELL_BLAZING_BLOW);
                        uiBlazingBlowTimer = 8*IN_MILLISECONDS;
                    }
                    else uiBlazingBlowTimer -= diff;

                    break;
                }

                case 73172:
                {
                    if (uiBlazingBlowTimer <= diff)
                    {
                        me->CastSpell(me, SPELL_BLAZING_BLOW);
                        uiBlazingBlowTimer = 8*IN_MILLISECONDS;
                    }
                    else uiBlazingBlowTimer -= diff;

                    if (uiConjureTimer <= diff)
                    {
                        me->CastSpell(me, SPELL_CONJURE_ETERNAL_KILN);
                        uiConjureTimer = 25*IN_MILLISECONDS;
                    }
                    else uiConjureTimer -= diff;

                    break;
                }

                case 73173:
                {
                    if (uiCauterizeTimer <= diff)
                    {
                        me->CastSpell(me, SPELL_CAUTERIZE);
                        uiCauterizeTimer = 35*IN_MILLISECONDS;
                    }
                    else uiCauterizeTimer -= diff;
                    
                    if (uiConjureTimer <= diff)
                    {
                        me->CastSpell(me, SPELL_CONJURE_FLARECORE_GOLEM);
                        uiConjureTimer = 25*IN_MILLISECONDS;
                    }
                    else uiConjureTimer -= diff;

                    if (uiFireStormTimer <= diff)
                    {
                        me->CastSpell(me->GetVictim(), SPELL_FIRE_STORM);
                        uiFireStormTimer = 25*IN_MILLISECONDS;
                    }
                    else uiFireStormTimer -= diff;

                    if (uiGreaterPyroblastTimer <= diff)
                    {
                        me->CastSpell(me->GetVictim(), SPELL_GREATER_PYROBLAST);
                        uiGreaterPyroblastTimer = 1500;
                    }
                    else uiGreaterPyroblastTimer -= diff;

                    break;
                }

                case 73666:
                {
                    if (uiCauterizeTimer <= diff)
                    {
                        me->CastSpell(me, SPELL_CAUTERIZE);
                        uiCauterizeTimer = 35*IN_MILLISECONDS;
                    }
                    else uiCauterizeTimer -= diff;
                    
                    if (uiConjureTimer <= diff)
                    {
                        me->CastSpell(me, RAND(SPELL_CONJURE_FLARECORE_GOLEM, SPELL_CONJURE_ETERNAL_KILN));
                        uiConjureTimer = 25*IN_MILLISECONDS;
                    }
                    else uiConjureTimer -= diff;

                    if (uiFireStormTimer <= diff)
                    {
                        me->CastSpell(me->GetVictim(), SPELL_FIRE_STORM);
                        uiFireStormTimer = 25*IN_MILLISECONDS;
                    }
                    else uiFireStormTimer -= diff;

                    if (uiBlazingBlowTimer <= diff)
                    {
                        me->CastSpell(me, SPELL_BLAZING_BLOW);
                        uiBlazingBlowTimer = 8*IN_MILLISECONDS;
                    }
                    else uiBlazingBlowTimer -= diff;

                    break;
                }

                default:
                    break;
            }

            DoMeleeAttackIfReady();
        }
    };

    CreatureAI* GetAI(Creature* creature) const 
    {
        return new npc_ordos_minionsAI(creature);
    }
};

// Eternal Kiln #73528#
class npc_eternal_kiln : public CreatureScript
{
public:

    npc_eternal_kiln() : CreatureScript("npc_eternal_kiln")
    {
    }

    struct npc_eternal_kilnAI : public ScriptedAI
    {
        npc_eternal_kilnAI(Creature* creature) : ScriptedAI(creature)
        {
            SetCombatMovement(false);
        }

        bool kilnFire;

        void Reset() 
        {
            me->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_DISABLE_MOVE);
            me->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_PACIFIED);

            me->AddUnitState(UNIT_STATE_ROTATING); // Restric movements

            kilnFire = false;
        }

        void JustDied(Unit* /*killer*/) 
        {
            me->DespawnOrUnsummon();
        }

        void EnterCombat(Unit* /*who*/) {}

        void UpdateAI(const uint32 diff)
        {
            if (!kilnFire)
            {
                if (TempSummon* summon = me->ToTempSummon())
                    if (Unit* summoner = summon->GetSummoner())
                    {
                        summon->CastSpell(summoner, SPELL_KILNFIRE);
                        kilnFire = true;
                    }
            }
        }
    };

    CreatureAI* GetAI(Creature* creature) const 
    {
        return new npc_eternal_kilnAI(creature);
    }
};

// Damp Shambler #72771# - Rock Moss #73157# - Spelurk #71864#
class npc_damp_shamblers : public CreatureScript
{
public:

    npc_damp_shamblers() : CreatureScript("npc_damp_shamblers")
    {
    }

    struct npc_damp_shamblersAI : public ScriptedAI
    {
        npc_damp_shamblersAI(Creature* creature) : ScriptedAI(creature)
        {
        }

        uint32 uiMurkyCloudTimer;
        uint32 uiRenewingMistsTimer;
        uint32 uiPoisonBreathTimer;

        void Reset() 
        {
            uiMurkyCloudTimer = 5*IN_MILLISECONDS;
            uiRenewingMistsTimer = 10*IN_MILLISECONDS;
            uiPoisonBreathTimer = 8*IN_MILLISECONDS;
        }

        void EnterCombat(Unit* /*who*/) 
        {
            if (me->GetEntry() == 72896)
                me->CastSpell(me->GetVictim(), SPELL_FIERY_CHARGE);
        }

        void UpdateAI(const uint32 diff)
        {
            if (!UpdateVictim())
                return;

            if (me->HasUnitState(UNIT_STATE_CASTING))
                return;

            switch (me->GetEntry())
            {
                case 72771:
                {
                    if (uiMurkyCloudTimer <= diff)
                    {
                        me->CastSpell(me->GetVictim(), SPELL_MURKY_CLOUD);
                        uiMurkyCloudTimer = 20*IN_MILLISECONDS;
                    }
                    else uiMurkyCloudTimer -= diff;
                    
                    if (uiRenewingMistsTimer <= diff)
                    {
                        me->CastSpell(me, SPELL_RENEWING_MISTS);
                        uiRenewingMistsTimer = 18*IN_MILLISECONDS;
                    }
                    else uiRenewingMistsTimer -= diff;

                    break;
                }

                case 73157:
                {
                    if (uiMurkyCloudTimer <= diff)
                    {
                        me->CastSpell(me->GetVictim(), SPELL_MURKY_CLOUD);
                        uiMurkyCloudTimer = 20*IN_MILLISECONDS;
                    }
                    else uiMurkyCloudTimer -= diff;
                    
                    if (uiRenewingMistsTimer <= diff)
                    {
                        me->CastSpell(me, SPELL_RENEWING_MISTS);
                        uiRenewingMistsTimer = 18*IN_MILLISECONDS;
                    }
                    else uiRenewingMistsTimer -= diff;

                    break;
                }

                case 71864:
                {
                    if (uiMurkyCloudTimer <= diff)
                    {
                        me->CastSpell(me->GetVictim(), SPELL_MURKY_CLOUD);
                        uiMurkyCloudTimer = 20*IN_MILLISECONDS;
                    }
                    else uiMurkyCloudTimer -= diff;
                    
                    if (uiRenewingMistsTimer <= diff)
                    {
                        me->CastSpell(me, SPELL_RENEWING_MISTS);
                        uiRenewingMistsTimer = 18*IN_MILLISECONDS;
                    }
                    else uiRenewingMistsTimer -= diff;

                    if (uiPoisonBreathTimer <= diff)
                    {
                        me->CastSpell(me->GetVictim(), SPELL_POISON_BREATH);
                        uiPoisonBreathTimer = 13*IN_MILLISECONDS;
                    }
                    else uiPoisonBreathTimer -= diff;

                    break;
                }

                default:
                    break;
            }

            DoMeleeAttackIfReady();
        }
    };

    CreatureAI* GetAI(Creature* creature) const 
    {
        return new npc_damp_shamblersAI(creature);
    }
};

// Ordon Oathguard #72892# - Jakur of Ordon #73169#
class npc_ordon_guards : public CreatureScript
{
public:

    npc_ordon_guards() : CreatureScript("npc_ordon_guards")
    {
    }

    struct npc_ordon_guardsAI : public ScriptedAI
    {
        npc_ordon_guardsAI(Creature* creature) : ScriptedAI(creature)
        {
        }

        uint32 uiCrackingBlowTimer;
        uint32 uiDefensiveShieldTimer;
        bool oathOfGuardianship;

        void Reset() 
        {
            uiCrackingBlowTimer = 4*IN_MILLISECONDS;
            uiDefensiveShieldTimer = 10*IN_MILLISECONDS;
            oathOfGuardianship = false;
        }

        void EnterCombat(Unit* /*who*/) {}

        void UpdateAI(const uint32 diff)
        {
            if (!UpdateVictim())
                return;

            if (me->HasUnitState(UNIT_STATE_CASTING))
                return;
            
            if (!oathOfGuardianship && HealthBelowPct(20))
            {
                if (!me->HasAura(SPELL_OATH_OF_GUARDIANSHIP))
                    me->CastSpell(me, SPELL_OATH_OF_GUARDIANSHIP);

                oathOfGuardianship = true;
            }

            switch (me->GetEntry())
            {
                case 72892:
                {
                    if (uiCrackingBlowTimer <= diff)
                    {
                        me->CastSpell(me->GetVictim(), SPELL_CRACKING_BLOW);
                        uiCrackingBlowTimer = 6*IN_MILLISECONDS;
                    }
                    else uiCrackingBlowTimer -= diff;
                    
                    if (uiDefensiveShieldTimer <= diff)
                    {
                        me->CastSpell(me, SPELL_DEFENSIVE_SHIELD);
                        uiDefensiveShieldTimer = 25*IN_MILLISECONDS;
                    }
                    else uiDefensiveShieldTimer -= diff;

                    break;
                }

                case 73169:
                {
                    if (uiCrackingBlowTimer <= diff)
                    {
                        me->CastSpell(me->GetVictim(), SPELL_CRACKING_BLOW);
                        uiCrackingBlowTimer = 6*IN_MILLISECONDS;
                    }
                    else uiCrackingBlowTimer -= diff;
                    
                    if (uiDefensiveShieldTimer <= diff)
                    {
                        me->CastSpell(me, SPELL_DEFENSIVE_SHIELD);
                        uiDefensiveShieldTimer = 25*IN_MILLISECONDS;
                    }
                    else uiDefensiveShieldTimer -= diff;

                    break;
                }

                default:
                    break;
            }

            DoMeleeAttackIfReady();
        }
    };

    CreatureAI* GetAI(Creature* creature) const 
    {
        return new npc_ordon_guardsAI(creature);
    }
};

// Burning Berserker #72895# - Champion of the Black Flame #73171#
class npc_ordos_warriors : public CreatureScript
{
public:

    npc_ordos_warriors() : CreatureScript("npc_ordos_warriors")
    {
    }

    struct npc_ordos_warriorsAI : public ScriptedAI
    {
        npc_ordos_warriorsAI(Creature* creature) : ScriptedAI(creature)
        {
        }

        uint32 uiForageTimer;
        uint32 uiBlazingCleaveTimer;
        uint32 uiBurningFuryTimer;

        void Reset() 
        {
            uiForageTimer = 5*IN_MILLISECONDS;
            uiBlazingCleaveTimer = 4*IN_MILLISECONDS;
            uiBurningFuryTimer = 10*IN_MILLISECONDS;
        }

        void EnterCombat(Unit* /*who*/) 
        {
            me->CastSpell(me->GetVictim(), SPELL_FIERY_CHARGE);
        }

        void UpdateAI(const uint32 diff)
        {
            if (!me->IsInCombat())
            {
                if (uiForageTimer <= diff)
                {
                    me->CastSpell(me, SPELL_FORAGE);
                    uiForageTimer = 25*IN_MILLISECONDS;
                }
                else uiForageTimer -= diff;
            }

            if (!UpdateVictim())
                return;

            if (me->HasUnitState(UNIT_STATE_CASTING))
                return;
            
            switch (me->GetEntry())
            {
                case 72895:
                {
                    if (uiBlazingCleaveTimer <= diff)
                    {
                        me->CastSpell(me->GetVictim(), SPELL_BLAZING_CLEAVE);
                        uiBlazingCleaveTimer = 6*IN_MILLISECONDS;
                    }
                    else uiBlazingCleaveTimer -= diff;

                    if (uiBurningFuryTimer <= diff)
                    {
                        me->CastSpell(me->GetVictim(), SPELL_BURNING_FURY);
                        uiBurningFuryTimer = 20*IN_MILLISECONDS;
                    }
                    else uiBurningFuryTimer -= diff;

                    break;
                }

                case 73171:
                {
                    if (uiBlazingCleaveTimer <= diff)
                    {
                        me->CastSpell(me->GetVictim(), SPELL_BLAZING_CLEAVE);
                        uiBlazingCleaveTimer = 6*IN_MILLISECONDS;
                    }
                    else uiBlazingCleaveTimer -= diff;

                    if (uiBurningFuryTimer <= diff)
                    {
                        me->CastSpell(me->GetVictim(), SPELL_BURNING_FURY);
                        uiBurningFuryTimer = 20*IN_MILLISECONDS;
                    }
                    else uiBurningFuryTimer -= diff;

                    break;
                }

                default:
                    break;
            }

            DoMeleeAttackIfReady();
        }
    };

    CreatureAI* GetAI(Creature* creature) const 
    {
        return new npc_ordos_warriorsAI(creature);
    }
};

// Ordon Fire-Watcher #72894# - Watcher Osu #73170#
class npc_ordon_watchers : public CreatureScript
{
public:

    npc_ordon_watchers() : CreatureScript("npc_ordon_watchers")
    {
    }

    struct npc_ordon_watchersAI : public ScriptedAI
    {
        npc_ordon_watchersAI(Creature* creature) : ScriptedAI(creature)
        {
        }

        uint32 uiFallingFlamesTimer;
        uint32 uiPyroblastTimer;

        void Reset() 
        {
            uiFallingFlamesTimer = 4*IN_MILLISECONDS;
            uiPyroblastTimer = 1*IN_MILLISECONDS;
        }

        void EnterCombat(Unit* /*who*/) {}

        void UpdateAI(const uint32 diff)
        {
            if (!UpdateVictim())
                return;

            if (me->HasUnitState(UNIT_STATE_CASTING))
                return;

            switch (me->GetEntry())
            {
                case 72894:
                {
                    if (uiFallingFlamesTimer <= diff)
                    {
                        me->CastSpell(me, SPELL_FALLING_FLAMES);
                        uiFallingFlamesTimer = 4*IN_MILLISECONDS;
                    }
                    else uiFallingFlamesTimer -= diff;

                    if (uiPyroblastTimer <= diff)
                    {
                        me->CastSpell(me->GetVictim(), SPELL_PYROBLAST);
                        uiPyroblastTimer = 2*IN_MILLISECONDS;
                    }
                    else uiPyroblastTimer -= diff;

                    break;
                }

                case 73170:
                {
                    if (uiFallingFlamesTimer <= diff)
                    {
                        me->CastSpell(me, SPELL_FALLING_FLAMES);
                        uiFallingFlamesTimer = 4*IN_MILLISECONDS;
                    }
                    else uiFallingFlamesTimer -= diff;

                    if (uiPyroblastTimer <= diff)
                    {
                        me->CastSpell(me->GetVictim(), SPELL_PYROBLAST);
                        uiPyroblastTimer = 2*IN_MILLISECONDS;
                    }
                    else uiPyroblastTimer -= diff;

                    break;
                }

                default:
                    break;
            }

            DoMeleeAttackIfReady();
        }
    };

    CreatureAI* GetAI(Creature* creature) const 
    {
        return new npc_ordon_watchersAI(creature);
    }
};

// Molten Guardian #72888# - Cinderfall #73175#
class npc_molten_guards : public CreatureScript
{
public:

    npc_molten_guards() : CreatureScript("npc_molten_guards")
    {
    }

    struct npc_molten_guardsAI : public ScriptedAI
    {
        npc_molten_guardsAI(Creature* creature) : ScriptedAI(creature)
        {
        }

        uint32 uiMoltenInfernoTimer;
        uint8 spellRotation;

        void Reset() 
        {
            spellRotation = 0;
            uiMoltenInfernoTimer = 6*IN_MILLISECONDS;
        }

        void EnterCombat(Unit* /*who*/) {}

        void UpdateAI(const uint32 diff)
        {
            if (!UpdateVictim())
                return;

            if (me->HasUnitState(UNIT_STATE_CASTING))
                return;

            switch (me->GetEntry())
            {
                case 72888:
                {
                    if (uiMoltenInfernoTimer <= diff)
                    {
                        switch (spellRotation)
                        {
                            case 0:
                            {
                                me->CastSpell(me->GetVictim(), SPELL_MOLTEN_INFERNO_1);
                                spellRotation++;
                                break;
                            }

                            case 1:
                            {
                                me->CastSpell(me->GetVictim(), SPELL_MOLTEN_INFERNO_2);
                                spellRotation++;
                                break;
                            }

                            case 2:
                            {
                                me->CastSpell(me->GetVictim(), SPELL_MOLTEN_INFERNO_3);
                                spellRotation = 0;
                                break;
                            }
                        }

                        uiMoltenInfernoTimer = 10*IN_MILLISECONDS;
                    }
                    else uiMoltenInfernoTimer -= diff;

                    break;
                }

                case 73175:
                {
                    if (uiMoltenInfernoTimer <= diff)
                    {
                        switch (spellRotation)
                        {
                            case 0:
                            {
                                me->CastSpell(me->GetVictim(), SPELL_MOLTEN_INFERNO_1);
                                spellRotation++;
                                break;
                            }

                            case 1:
                            {
                                me->CastSpell(me->GetVictim(), SPELL_MOLTEN_INFERNO_2);
                                spellRotation++;
                                break;
                            }

                            case 2:
                            {
                                me->CastSpell(me->GetVictim(), SPELL_MOLTEN_INFERNO_3);
                                spellRotation = 0;
                                break;
                            }
                        }

                        uiMoltenInfernoTimer = 10*IN_MILLISECONDS;
                    }
                    else uiMoltenInfernoTimer -= diff;

                    break;
                }

                default:
                    break;
            }

            DoMeleeAttackIfReady();
        }
    };

    CreatureAI* GetAI(Creature* creature) const 
    {
        return new npc_molten_guardsAI(creature);
    }
};

// Leafmender #73277# - Ashleaf Sprite #72877#
class npc_leaf_sprites : public CreatureScript
{
public:

    npc_leaf_sprites() : CreatureScript("npc_leaf_sprites")
    {
    }

    struct npc_leaf_spritesAI : public ScriptedAI
    {
        npc_leaf_spritesAI(Creature* creature) : ScriptedAI(creature)
        {
        }

        uint32 uiGreathWrathTimer;
        uint32 uiHealingTouchTimer;
        uint32 uiSunWorshipTimer;

        void Reset() 
        {
            uiGreathWrathTimer = 1*IN_MILLISECONDS;
            uiHealingTouchTimer = 10*IN_MILLISECONDS;
            uiSunWorshipTimer = 6*IN_MILLISECONDS;
        }

        void EnterCombat(Unit* /*who*/) {}

        void UpdateAI(const uint32 diff)
        {
            if (!UpdateVictim())
                return;

            if (me->HasUnitState(UNIT_STATE_CASTING))
                return;

            switch (me->GetEntry())
            {
                case 73277:
                {
                    if (uiGreathWrathTimer <= diff)
                    {
                        me->CastSpell(me->GetVictim(), SPELL_GREAT_WRATH);
                        uiGreathWrathTimer = 2*IN_MILLISECONDS;
                    }
                    else uiGreathWrathTimer -= diff;

                    if (uiHealingTouchTimer <= diff)
                    {
                        me->CastSpell(me, SPELL_HEALING_TOUCH);
                        uiHealingTouchTimer = 15*IN_MILLISECONDS;
                    }
                    else uiHealingTouchTimer -= diff;

                    if (uiSunWorshipTimer <= diff)
                    {
                        me->CastSpell(me->GetVictim(), SPELL_SUN_WORSHIP);
                        uiSunWorshipTimer = 6*IN_MILLISECONDS;
                    }
                    else uiSunWorshipTimer -= diff;

                    break;
                }

                case 72877:
                {
                    if (uiGreathWrathTimer <= diff)
                    {
                        me->CastSpell(me->GetVictim(), SPELL_GREAT_WRATH);
                        uiGreathWrathTimer = 2*IN_MILLISECONDS;
                    }
                    else uiGreathWrathTimer -= diff;

                    if (uiHealingTouchTimer <= diff)
                    {
                        me->CastSpell(me, SPELL_HEALING_TOUCH);
                        uiHealingTouchTimer = 15*IN_MILLISECONDS;
                    }
                    else uiHealingTouchTimer -= diff;

                    if (uiSunWorshipTimer <= diff)
                    {
                        me->CastSpell(me->GetVictim(), SPELL_SUN_WORSHIP);
                        uiSunWorshipTimer = 6*IN_MILLISECONDS;
                    }
                    else uiSunWorshipTimer -= diff;

                    break;
                }

                default:
                    break;
            }

            DoMeleeAttackIfReady();
        }
    };

    CreatureAI* GetAI(Creature* creature) const 
    {
        return new npc_leaf_spritesAI(creature);
    }
};

// Jademist Dancer #72767#
class npc_jademist_dancer : public CreatureScript
{
public:

    npc_jademist_dancer() : CreatureScript("npc_jademist_dancer")
    {
    }

    struct npc_jademist_dancerAI : public ScriptedAI
    {
        npc_jademist_dancerAI(Creature* creature) : ScriptedAI(creature)
        {
        }

        uint32 uiSteamBlastTimer;

        void Reset() 
        {
            uiSteamBlastTimer = 6*IN_MILLISECONDS;
        }

        void EnterCombat(Unit* /*who*/) {}

        void UpdateAI(const uint32 diff)
        {
            if (!UpdateVictim())
                return;

            if (me->HasUnitState(UNIT_STATE_CASTING))
                return;

            if (uiSteamBlastTimer <= diff)
            {
                me->CastSpell(me->GetVictim(), SPELL_STEAM_BLAST);
                uiSteamBlastTimer = 10*IN_MILLISECONDS;
            }
            else uiSteamBlastTimer -= diff;

            DoMeleeAttackIfReady();
        }
    };

    CreatureAI* GetAI(Creature* creature) const 
    {
        return new npc_jademist_dancerAI(creature);
    }
};

// Nice Sprite #71823#
class npc_nice_sprite : public CreatureScript
{
public:

    npc_nice_sprite() : CreatureScript("npc_nice_sprite")
    {
    }

    struct npc_nice_spriteAI : public ScriptedAI
    {
        npc_nice_spriteAI(Creature* creature) : ScriptedAI(creature)
        {
        }

        uint32 uiNatureSWrathTimer;
        uint32 uiStickySapTimer;

        void Reset() 
        {
            uiNatureSWrathTimer = 1*IN_MILLISECONDS;
            uiStickySapTimer = 3*IN_MILLISECONDS;
        }

        void EnterCombat(Unit* /*who*/) {}

        void UpdateAI(const uint32 diff)
        {
            if (!UpdateVictim())
                return;

            if (me->HasUnitState(UNIT_STATE_CASTING))
                return;

            if (uiNatureSWrathTimer <= diff)
            {
                me->CastSpell(me->GetVictim(), SPELL_NATURE_S_WRATH);
                uiNatureSWrathTimer = 1*IN_MILLISECONDS;
            }
            else uiNatureSWrathTimer -= diff;

            if (uiStickySapTimer <= diff)
            {
                me->CastSpell(me->GetVictim(), SPELL_STICKY_SAP);
                uiStickySapTimer = 2*IN_MILLISECONDS;
            }
            else uiStickySapTimer -= diff;

            DoMeleeAttackIfReady();
        }
    };

    CreatureAI* GetAI(Creature* creature) const 
    {
        return new npc_nice_spriteAI(creature);
    }
};

// Ordon Candlekeeper #72875#
class npc_ordon_candlekeeper : public CreatureScript
{
public:

    npc_ordon_candlekeeper() : CreatureScript("npc_ordon_candlekeeper")
    {
    }

    struct npc_ordon_candlekeeperAI : public ScriptedAI
    {
        npc_ordon_candlekeeperAI(Creature* creature) : ScriptedAI(creature)
        {
        }

        uint32 uiBreathOfFireTimer;

        void Reset() 
        {
            uiBreathOfFireTimer = 1*IN_MILLISECONDS;
        }

        void EnterCombat(Unit* /*who*/) {}

        void UpdateAI(const uint32 diff)
        {
            if (!UpdateVictim())
                return;

            if (me->HasUnitState(UNIT_STATE_CASTING))
                return;

            if (HealthBelowPct(25))
                me->CastSpell(me, SPELL_BURNING_SACRIFICE);

            if (uiBreathOfFireTimer <= diff)
            {
                me->CastSpell(me->GetVictim(), SPELL_BREATH_OF_FIRE);
                uiBreathOfFireTimer = 2*IN_MILLISECONDS;
            }
            else uiBreathOfFireTimer -= diff;

            DoMeleeAttackIfReady();
        }
    };

    CreatureAI* GetAI(Creature* creature) const 
    {
        return new npc_ordon_candlekeeperAI(creature);
    }
};

void AddSC_timeless_isle()
{
	// creatures
    new npc_timeless_turtles();
    new npc_timeless_faction_sentries();
    new npc_timeless_adders();
    new npc_timeless_yaks();
    new npc_timeless_cranes();
    new npc_timeless_silkworms();
    new npc_timeless_crabs();
    new npc_timeless_spectrals();
    new npc_timeless_cats();
    new npc_timeless_elementals();
    new npc_timeless_spirits();
    new npc_timeless_giants();
    new npc_timeless_frogs();
    new npc_timeless_dragons();
    new npc_ordos_minions();
    new npc_eternal_kiln();
    new npc_damp_shamblers();
    new npc_ordon_guards();
    new npc_ordos_warriors();
    new npc_ordon_watchers();
    new npc_molten_guards();
    new npc_leaf_sprites();
    new npc_jademist_dancer();
    new npc_nice_sprite();
    new npc_ordon_candlekeeper();
    new npc_emperor_shaohao();
	
	// gameobject
    new go_time_lost_shrine_ti();
    new go_gleaming_crane_statue_ti();
	
	// spells
    new spell_timeless_isle_crane_wings();
    new spell_timeless_isle_cauterize();
    new spell_timeless_isle_burning_fury();
}
