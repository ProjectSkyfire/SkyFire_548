/*
* This file is part of Project SkyFire https://www.projectskyfire.org.
* See LICENSE.md file for Copyright information
*/

#include "ScriptMgr.h"
#include "ScriptedCreature.h"
#include "InstanceScript.h"
#include "Player.h"
#include "SpellAuras.h"
#include "SpellMgr.h"
#include "Vehicle.h"
#include "deadmines.h"

#define MAX_ENCOUNTER 6

DoorData const doorData[] =
{
    { GO_FACTORY_DOOR,   DATA_GLUBTOK,   DOOR_TYPE_PASSAGE, BOUNDARY_NONE },
    { GO_MAST_ROOM_DOOR, DATA_HELIX,     DOOR_TYPE_PASSAGE, BOUNDARY_NONE },
    { GO_HEAVY_DOOR,     DATA_HELIX,     DOOR_TYPE_ROOM,    BOUNDARY_NONE },
    { GO_FOUNDRY_DOOR,   DATA_FOEREAPER, DOOR_TYPE_PASSAGE, BOUNDARY_NONE },
    { GO_HEAVY_DOOR_2,   DATA_FOEREAPER, DOOR_TYPE_ROOM,    BOUNDARY_NONE },
    { 0,                 0,              DOOR_TYPE_ROOM,    BOUNDARY_NONE },
};

enum NightmareEvents
{
    EVENT_NM_BOARD_TRAP = 1,
    EVENT_NM_SUMMON_VALVES,
    EVENT_NM_TRAP_MOVE_DOWN,
    EVENT_NM_TRAP_MAGMA,
    EVENT_NM_EJECT,
    EVENT_NM_GLUBTOK_AURA,
    EVENT_NM_GLUBTOK_SETUP,
    EVENT_NM_GLUBTOK_SPAWN,
    EVENT_NM_ICICLE,
    EVENT_NM_HELIX_AURA,
    EVENT_NM_HELIX_DESPAWN_FIRE,
    EVENT_NM_HELIX_SETUP,
    EVENT_NM_HELIX_UNFLAG,
    EVENT_NM_HELIX_SPIDERS,
    EVENT_NM_HELIX_SPIRIT_STRIKE,
    EVENT_NM_REAPER_DESPAWN_SPIDERS,
    EVENT_NM_REAPER_SETUP,
    EVENT_NM_REAPER_SPAWN,
    EVENT_NM_RIPSNARL_AURA,
    EVENT_NM_RIPSNARL_SETUP,
    EVENT_NM_EMME,
    EVENT_NM_ERIK,
    EVENT_NM_JAMES,
    EVENT_NM_VANESSA_BOSS,
    EVENT_NM_CHECK_DEAD_PLAYERS,
    EVENT_NM_RESPAWN_NOTE,
    EVENT_NM_TRAP_SEATED,
};

enum NightmareSpells
{
    SPELL_ROPE_BEAM                     = 43785,
    SPELL_RIDE_MAGMA_VEHICLE            = 92379,
    SPELL_MOLTEN_MAGMA                  = 92424,
    SPELL_NIGHTMARE_SLOW                = 92559,
    SPELL_NIGHTMARE_AURA                = 92563,
    SPELL_NIGHTMARE_AURA_GLUBTOK        = 92565,
    SPELL_NIGHTMARE_AURA_HELIX          = 92566,
    SPELL_NIGHTMARE_AURA_MECHANICAL     = 92567,
    SPELL_NIGHTMARE_AURA_RIPSNARL       = 92568,
    SPELL_NIGHTMARE_FLAMES              = 92169,
    SPELL_ICICLE                        = 92201,
    SPELL_SPIRIT_STRIKE                 = 59304,
    SPELL_ADRENALINE                    = 92604,
    SPELL_CHOKING                       = 92608,
    SPELL_NIGHTMARES_END                = 92603,
};

enum NightmareTexts
{
    // General Purpose Bunny JMF 45979
    SAY_NM_ELIXIR_TAKES_HOLD    = 4,
    SAY_NM_NIGHTMARE_SHIFTS     = 5,
    SAY_NM_SAVE_EMME            = 6,
    SAY_NM_SAVE_ERIK            = 7,
    SAY_NM_SAVE_CALISSA         = 8,
    SAY_NM_ELIXIR_WEARS_OFF     = 9,
    // Vanessa's Trap Bunny 49454
    SAY_NM_ACTIVATE_VALVES      = 0,
    // Vanessa VanCleef (nightmare) 49671
    SAY_NM_GLUBTOK_1            = 0,
    SAY_NM_GLUBTOK_2            = 1,
    SAY_NM_ENTERED_GLUBTOK      = 2,
    SAY_NM_BACK_TO_THE_SHIP     = 3,
    SAY_NM_HELIX_1              = 4,
    SAY_NM_HELIX_2              = 5,
    SAY_NM_MECHANICAL_1         = 6,
    SAY_NM_MECHANICAL_2         = 7,
    SAY_NM_ENTERED_MECHANICAL   = 8,
    SAY_NM_RIPSNARL_1           = 9,
    SAY_NM_RIPSNARL_2           = 10,
    SAY_NM_ENTERED_RIPSNARL     = 11,
    // Helix Gearbreaker (nightmare) 49674
    SAY_NM_ENTERED_HELIX        = 0,
    SAY_NM_SPIDERS              = 1,
};

uint8 const NM_TALK_DESPAWN = 0xFF;
uint32 const NM_STUCK_CHECK_INTERVAL = 5000;
float const LIGHTNING_PLATTER_OMEGA = 0.4f;

struct NmTalk
{
    uint32 at;
    uint64 who;
    uint8 group;
};

class instance_deadmines : public InstanceMapScript
{
    public:
        instance_deadmines() : InstanceMapScript("instance_deadmines", 36) { }

        struct instance_deadmines_InstanceMapScript : public InstanceScript
        {
            instance_deadmines_InstanceMapScript(Map* map) : InstanceScript(map)
            {
                SetBossNumber(MAX_ENCOUNTER);
                LoadDoorData(doorData);

                uiGlubtokGUID        = 0;
                uiHelixGUID          = 0;
                uiOafGUID            = 0;
                uiFoereaperGUID      = 0;
                uiAdmiralGUID        = 0;
                uiCaptainGUID        = 0;
                uiVanessaGUID        = 0;
                anchorBunnyGUID      = 0;
                FoundryDoorGUID      = 0;
                trapGUID = announcerGUID = vanessaNmGUID = glubtokNmGUID = helixNmGUID = reaperNmGUID = 0;
                emmeGUID = erikGUID = calissaGUID = jamesGUID = 0;

                GoblinTeleporterGUID = 0;
                HeavyDoorGUID        = 0;
                HeavyDoor2GUID       = 0;
                IronCladDoorGUID     = 0;
                DefiasCannonGUID     = 0;
                DoorLeverGUID        = 0;
                TeamInInstance       = 0;
                uiVanessaEvent       = 0;
                OverchargeCount      = 0;
                nmClock              = 0;
                valvesActivated      = 0;
                deadWorgen           = 0;
                noteSpawnChecked     = false;
                State                = CANNON_NOT_USED;
                instance->SetWorldState(WORLDSTATE_PROTOTYPE_PRODIGY, 1);
            }

            void OnPlayerEnter(Player* player) OVERRIDE
            {
                // An instance loaded with Cookie dead and Vanessa not gets her Note back
                if (!noteSpawnChecked)
                {
                    noteSpawnChecked = true;
                    if (instance->IsHeroic() && GetBossState(DATA_CAPTAIN) == DONE && GetBossState(DATA_VANESSA) != DONE &&
                        uiVanessaEvent == NOT_STARTED)
                        instance->SummonCreature(NPC_NOTE_FROM_VANESSA, notePos);
                }

                if (TeamInInstance)
                    return;

                TeamInInstance = player->GetTeam();

                for (uint64 guid : FactionNpcGUIDs)
                    if (Creature* creature = instance->GetCreature(guid))
                        UpdateFactionNpc(creature);
            }

            void OnCreatureCreate(Creature* creature) OVERRIDE
            {
                if (!TeamInInstance)
                {
                    Map::PlayerList const& players = instance->GetPlayers();
                    if (!players.isEmpty())
                        if (Player* player = players.begin()->GetSource())
                            TeamInInstance = player->GetTeam();
                }

                switch (creature->GetEntry())
                {
                    case NPC_GLUBTOK:
                        uiGlubtokGUID = creature->GetGUID();
                        break;
                    case NPC_HELIX_GEARBREAKER:
                        uiHelixGUID = creature->GetGUID();
                        break;
                    case NPC_LUMBERING_OAF:
                        uiOafGUID = creature->GetGUID();
                        break;
                    case NPC_FOE_REAPER_5000:
                        uiFoereaperGUID = creature->GetGUID();
                        break;
                    case NPC_ADMIRAL_RIPSNARL:
                        uiAdmiralGUID = creature->GetGUID();
                        break;
                    case NPC_CAPTAIN_COOKIE:
                        uiCaptainGUID = creature->GetGUID();
                        break;
                    case NPC_VANESSA_VANCLEEF:
                        // The spawn of the database, shown at the end of the Nightmare
                        if (!creature->IsSummon())
                            uiVanessaGUID = creature->GetGUID();
                        break;
                    case NPC_VANESSA_ANCHOR_BUNNY_JMF:
                        anchorBunnyGUID = creature->GetGUID();
                        break;
                    case NPC_KAGTHA:
                    case NPC_SLINKY_SHARPSHIV:
                    case NPC_MISS_MAYHEM:
                    case NPC_MAYHEM_REAPER:
                    case NPC_HAND_ASSASIN:
                    case NPC_HORATIO_LAINE:
                    case NPC_DEFENDER:
                    case NPC_INVESTIGATOR:
                    case NPC_CRIME_SCENE_BOT:
                        FactionNpcGUIDs.push_back(creature->GetGUID());
                        UpdateFactionNpc(creature);
                        break;
                }
            }

            void UpdateFactionNpc(Creature* creature)
            {
                switch (creature->GetEntry())
                {
                    case NPC_KAGTHA:
                    case NPC_SLINKY_SHARPSHIV:
                    case NPC_MISS_MAYHEM:
                    case NPC_MAYHEM_REAPER:
                    case NPC_HAND_ASSASIN:
                        if (TeamInInstance == ALLIANCE)
                            HideFactionNpc(creature);
                        break;
                    default:
                        if (TeamInInstance == HORDE)
                            HideFactionNpc(creature);
                        break;
                }
            }

            // The other faction's NPCs are hostile to the group, so they must not engage while hidden
            void HideFactionNpc(Creature* creature)
            {
                if (creature->IsInCombat())
                    creature->CombatStop(true);
                creature->DeleteThreatList();

                creature->SetVisible(false);
                creature->setFaction(35);
                creature->SetReactState(REACT_PASSIVE);
                creature->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE | UNIT_FLAG_NOT_SELECTABLE | UNIT_FLAG_IMMUNE_TO_PC | UNIT_FLAG_IMMUNE_TO_NPC);
                creature->SetUInt32Value(UNIT_FIELD_NPC_FLAGS, UNIT_NPC_FLAG_NONE);
            }

            void OnUnitDeath(Unit* unit) OVERRIDE
            {
                // The stage bosses of the Nightmare, the Harringtons and the wipe check
                if (uiVanessaEvent >= NIGHTMARE_STAGE_TRAP)
                    NightmareUnitDeath(unit);

                if (!unit->ToCreature())
                    return;

                switch (unit->GetEntry())
                {
                    case NPC_MINING_POWDER:
                        unit->CastSpell(unit, SPELL_EXPLODE_MINE, false);
                        break;
                    case NPC_DEFIAS_WATCHER:
                    case NPC_DEFIAS_REAPER:
                        OverchargeCount++;

                        if (OverchargeCount >= 4)
                            if (Creature* FoeReaper = instance->GetCreature(uiFoereaperGUID))
                                FoeReaper->RemoveAurasDueToSpell(SPELL_OFF_LINE);
                        break;
                }
            }

            void OnGameObjectCreate(GameObject* go) OVERRIDE
            {
                switch (go->GetEntry())
                {
                    case GO_HEAVY_DOOR:
                        HeavyDoorGUID = go->GetGUID();
                        AddDoor(go, true);
                        break;
                    case GO_HEAVY_DOOR_2:
                        HeavyDoor2GUID = go->GetGUID();
                        AddDoor(go, true);
                        break;
                    case GO_FOUNDRY_DOOR:
                        FoundryDoorGUID = go->GetGUID();   // closed during the Nightmare, opened for the mechanical stage
                        AddDoor(go, true);
                        break;
                    case GO_FACTORY_DOOR:
                    case GO_MAST_ROOM_DOOR:
                        AddDoor(go, true);
                        break;
                    case GO_GOBLIN_TELEPORTER:
                        GoblinTeleporterGUID = go->GetGUID();
                        break;
                    case GO_IRONCLAD_DOOR:
                        IronCladDoorGUID = go->GetGUID();
                        if (State != CANNON_NOT_USED)
                            go->SetGoState(GOState::GO_STATE_ACTIVE_ALTERNATIVE);
                        break;
                    case GO_DEFIAS_CANNON:
                        DefiasCannonGUID = go->GetGUID();
                        break;
                    case GO_DOOR_LEVER:
                        DoorLeverGUID = go->GetGUID();
                        break;
                }
            }

            void OnGameObjectRemove(GameObject* go) OVERRIDE
            {
                switch (go->GetEntry())
                {
                    case GO_HEAVY_DOOR:
                    case GO_HEAVY_DOOR_2:
                    case GO_FACTORY_DOOR:
                    case GO_MAST_ROOM_DOOR:
                    case GO_FOUNDRY_DOOR:
                        AddDoor(go, false);
                        break;
                }
            }

            void ShootCannon()
            {
                if (GameObject* pDefiasCannon = instance->GetGameObject(DefiasCannonGUID))
                {
                    pDefiasCannon->SetGoState(GOState::GO_STATE_ACTIVE);
                    pDefiasCannon->PlayDirectSound(SOUND_CANNONFIRE);
                }
            }

            void BlastOutDoor()
            {
                if (GameObject* pIronCladDoor = instance->GetGameObject(IronCladDoorGUID))
                {
                    pIronCladDoor->SetGoState(GOState::GO_STATE_ACTIVE_ALTERNATIVE);
                    pIronCladDoor->PlayDirectSound(SOUND_DESTROYDOOR);
                }
            }

            void SetData(uint32 type, uint32 data) OVERRIDE
            {
                switch (type)
                {
                    case DATA_CANNON_EVENT:
                        State = data;
                        if (data == CANNON_BLAST_INITIATED)
                        {
                            ShootCannon();
                            BlastOutDoor();
                            SaveToDB();
                        }
                        break;
                    case DATA_VANESSA_EVENT:
                        // The end of the intro starts the Nightmare, FAIL ends it
                        if (data == NIGHTMARE_STAGE_TRAP)
                        {
                            if (uiVanessaEvent == IN_PROGRESS)
                                StartNightmare();
                            break;
                        }
                        if (data == FAIL)
                        {
                            ResetNightmare();
                            break;
                        }
                        uiVanessaEvent = data;
                        if (data == DONE)
                            SaveToDB();
                        break;
                    case DATA_ACTIVATED_VALVE:
                        ValveActivated();
                        break;
                }
            }

            void Update(uint32 diff) OVERRIDE
            {
                UpdateNightmare(diff);
            }

            uint32 GetData(uint32 type) const OVERRIDE
            {
                switch (type)
                {
                    case DATA_CANNON_EVENT:
                        return State;
                    case DATA_VANESSA_EVENT:
                        return uiVanessaEvent;
                    case DATA_TEAM_IN_INSTANCE:
                        return TeamInInstance;
                }

                return 0;
            }

            uint64 GetData64(uint32 type) const OVERRIDE
            {
                switch (type)
                {
                    case DATA_GLUBTOK:
                        return uiGlubtokGUID;
                    case DATA_HELIX:
                        return uiHelixGUID;
                    case DATA_OAF:
                        return uiOafGUID;
                    case DATA_FOEREAPER:
                        return uiFoereaperGUID;
                    case DATA_ADMIRAL:
                        return uiAdmiralGUID;
                    case DATA_CAPTAIN:
                        return uiCaptainGUID;
                }

                return 0;
            }

            bool SetBossState(uint32 type, EncounterState state) OVERRIDE
            {
                if (!InstanceScript::SetBossState(type, state))
                    return false;

                switch (type)
                {
                    case DATA_HELIX:
                        if (GameObject* go = instance->GetGameObject(HeavyDoorGUID))
                        {
                            if (state == IN_PROGRESS)
                                go->SetFlag(GAMEOBJECT_FIELD_FLAGS, GO_FLAG_NOT_SELECTABLE);
                            else if (state == DONE)
                                go->RemoveFlag(GAMEOBJECT_FIELD_FLAGS, GO_FLAG_NOT_SELECTABLE);
                        }
                        break;
                    case DATA_FOEREAPER:
                        if (GameObject* go = instance->GetGameObject(HeavyDoor2GUID))
                        {
                            if (state == IN_PROGRESS)
                                go->SetFlag(GAMEOBJECT_FIELD_FLAGS, GO_FLAG_NOT_SELECTABLE);
                            else if (state == DONE)
                                go->RemoveFlag(GAMEOBJECT_FIELD_FLAGS, GO_FLAG_NOT_SELECTABLE);
                        }
                        break;
                }

                return true;
            }

            // Vanessa's Nightmare
            Creature* GetNmCreature(uint64 guid) const
            {
                return guid ? instance->GetCreature(guid) : NULL;
            }

            // a creature of the Nightmare: kept for the cleanup, optionally a hazard that must not fight
            Creature* NmSummon(uint32 entry, Position const& pos, bool hazard = false)
            {
                TempSummon* summon = instance->SummonCreature(entry, pos);
                if (!summon)
                    return NULL;

                summon->setActive(true);
                nightmareGUIDs.insert(summon->GetGUID());
                if (hazard)
                {
                    // a hazard that entered combat would evade home and stop (or leave) its path
                    summon->SetReactState(REACT_PASSIVE);
                    summon->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_IMMUNE_TO_PC | UNIT_FLAG_IMMUNE_TO_NPC);
                    summon->ApplySpellImmune(0, IMMUNITY_DAMAGE, SPELL_SCHOOL_MASK_ALL, true);
                }
                return summon;
            }

            void NmTrack(Creature* creature)
            {
                if (creature)
                    nightmareGUIDs.insert(creature->GetGUID());
            }

            // `group` is said by `guid` `delayMs` from now
            void QueueTalk(uint64 guid, uint8 group, uint32 delayMs)
            {
                nmTalks.push_back({ nmClock + delayMs, guid, group });
            }

            void QueueDespawn(uint64 guid, uint32 delayMs)
            {
                nmTalks.push_back({ nmClock + delayMs, guid, NM_TALK_DESPAWN });
            }

            template <class F>
            void ForEachAlivePlayer(F f)
            {
                Map::PlayerList const& players = instance->GetPlayers();
                for (Map::PlayerList::const_iterator itr = players.begin(); itr != players.end(); ++itr)
                    if (Player* player = itr->GetSource())
                        if (player->IsAlive() && !player->IsGameMaster())
                            f(player);
            }

            // At every stage change the purpose bunny casts Nightmare Aura and Nightmare Slow on everyone and the screen aura
            // of the previous stage goes
            void NmStageAura(uint32 previousStageAura)
            {
                if (Creature* bunny = GetNmCreature(announcerGUID))
                {
                    bunny->CastSpell(bunny, SPELL_NIGHTMARE_AURA, true);
                    bunny->CastSpell(bunny, SPELL_NIGHTMARE_SLOW, true);
                }
                if (previousStageAura)
                    DoRemoveAurasDueToSpellOnPlayers(previousStageAura);
            }

            // The intro is over: the players hang in Vanessa's trap over the slag
            void StartNightmare()
            {
                uiVanessaEvent = NIGHTMARE_STAGE_TRAP;
                nmClock = 0;
                valvesActivated = 0;
                deadWorgen = 0;
                nmTalks.clear();
                nightmareEvents.Reset();

                if (GameObject* door = instance->GetGameObject(FoundryDoorGUID))
                    door->SetGoState(GOState::GO_STATE_READY);
                if (GameObject* door = instance->GetGameObject(IronCladDoorGUID))
                    door->SetGoState(GOState::GO_STATE_READY);

                instance->LoadGrid(trapBunnyPos.GetPositionX(), trapBunnyPos.GetPositionY());
                if (Creature* bunny = NmSummon(NPC_GENERAL_PURPOSE_DUMMY_JMF, generalPurposeBunnyPos, true))
                    announcerGUID = bunny->GetGUID();

                if (Creature* trap = NmSummon(NPC_VANESSAS_TRAP_BUNNY, trapBunnyPos, true))
                {
                    trapGUID = trap->GetGUID();
                    trap->SetDisableGravity(true);
                    if (Creature* anchor = GetNmCreature(anchorBunnyGUID))
                        anchor->CastSpell(trap, SPELL_ROPE_BEAM, true);
                }

                // next to the trap first, then seated 2.5 seconds later
                ForEachAlivePlayer([](Player* player)
                {
                    player->NearTeleportTo(magmaTrapThrowPos.GetPositionX(), magmaTrapThrowPos.GetPositionY(), magmaTrapThrowPos.GetPositionZ(), player->GetOrientation());
                });
                nightmareEvents.ScheduleEvent(EVENT_NM_BOARD_TRAP, 2500);
                nightmareEvents.ScheduleEvent(EVENT_NM_CHECK_DEAD_PLAYERS, NM_STUCK_CHECK_INTERVAL);
            }

            // Each rider gets Ride Magma Vehicle on the trap with its own seat: a second Ride Vehicle aura from another caster
            // would replace the first on a vehicle without a free usable seat, and only the last player would stay on. The aura is
            // put on the trap directly (the spell has no target conditions) with the player as caster and the seat in its base
            // points, as Unit::HandleSpellClick does for a vehicle enter aura. Returns the players boarded now.
            uint32 BoardTrap(Creature* trap)
            {
                Vehicle* kit = trap ? trap->GetVehicleKit() : NULL;
                SpellInfo const* ride = sSpellMgr->GetSpellInfo(SPELL_RIDE_MAGMA_VEHICLE);
                if (!kit || !ride)
                    return 0;

                uint32 boarded = 0;
                int8 seat = 0;
                ForEachAlivePlayer([&](Player* player)
                {
                    if (player->GetVehicleBase() == trap)
                        return;
                    while (seat < 5 && !kit->HasEmptySeat(seat))
                        ++seat;
                    if (seat >= 5)
                        return;

                    int32 bp[MAX_SPELL_EFFECTS];
                    for (uint32 i = 0; i < MAX_SPELL_EFFECTS; ++i)
                        bp[i] = ride->Effects[i].BasePoints;
                    bp[EFFECT_0] = seat + 1;   // HandleAuraControlVehicle seats the caster on m_amount - 1
                    if (Aura::TryRefreshStackOrCreate(ride, MAX_EFFECT_MASK, trap, player, bp))
                        ++boarded;
                    ++seat;
                });
                return boarded;
            }

            void EjectFromTrap()
            {
                Creature* trap = GetNmCreature(trapGUID);
                if (!trap || !trap->GetVehicleKit())
                    return;

                std::vector<Unit*> riders;
                for (int8 seat = 0; seat < 5; ++seat)
                    if (Unit* passenger = trap->GetVehicleKit()->GetPassenger(seat))
                        riders.push_back(passenger);

                // everyone lands on the ledge of Glubtok's nightmare
                for (Unit* rider : riders)
                {
                    rider->ExitVehicle();
                    if (Player* player = rider->ToPlayer())
                        player->NearTeleportTo(magmaTrapThrowPos.GetPositionX(), magmaTrapThrowPos.GetPositionY(), magmaTrapThrowPos.GetPositionZ(), player->GetOrientation());
                }
                trap->RemoveAllAuras();
                trap->DespawnOrUnsummon(1000);
            }

            void DespawnNightmareSet(GuidSet& set)
            {
                for (uint64 guid : set)
                    if (Creature* creature = GetNmCreature(guid))
                        creature->DespawnOrUnsummon();
                set.clear();
            }

            void CleanupNightmare()
            {
                EjectFromTrap();
                DespawnNightmareSet(spiderGUIDs);    // Helix's summons; every other gauntlet creature is in nightmareGUIDs
                fireGUIDs.clear();
                sparkGUIDs.clear();
                DespawnNightmareSet(nightmareGUIDs);
                nightmareEvents.Reset();
                nmTalks.clear();
                nmClock = 0;
                trapGUID = announcerGUID = vanessaNmGUID = glubtokNmGUID = helixNmGUID = reaperNmGUID = 0;
                emmeGUID = erikGUID = calissaGUID = jamesGUID = 0;
                static uint32 const playerAuras[] = { SPELL_NIGHTMARE_ELIXIR, SPELL_NIGHTMARE_AURA, SPELL_NIGHTMARE_SLOW, SPELL_VANESSAS_BLACKOUT,
                    SPELL_NIGHTMARE_AURA_GLUBTOK, SPELL_NIGHTMARE_AURA_HELIX, SPELL_NIGHTMARE_AURA_MECHANICAL, SPELL_NIGHTMARE_AURA_RIPSNARL };
                for (uint32 spell : playerAuras)
                    DoRemoveAurasDueToSpellOnPlayers(spell);
            }

            // A wipe, a dead Harrington or a dead group: the Note comes back after 30 seconds. Only a running Nightmare
            // resets, so a second call in the same tick does not bring a second Note.
            void ResetNightmare()
            {
                if (uiVanessaEvent < NIGHTMARE_STAGE_TRAP)
                    return;

                CleanupNightmare();
                uiVanessaEvent = NOT_STARTED;
                if (GameObject* door = instance->GetGameObject(FoundryDoorGUID))
                    door->SetGoState(GOState::GO_STATE_ACTIVE);
                if (GameObject* door = instance->GetGameObject(IronCladDoorGUID))
                    door->SetGoState(State != CANNON_NOT_USED ? GOState::GO_STATE_ACTIVE_ALTERNATIVE : GOState::GO_STATE_READY);
                nightmareEvents.ScheduleEvent(EVENT_NM_RESPAWN_NOTE, 30000);
                SaveToDB();
            }

            void SetNightmareStage(uint32 stage)
            {
                uiVanessaEvent = stage;
                switch (stage)
                {
                    case NIGHTMARE_STAGE_GLUBTOK:
                        nightmareEvents.ScheduleEvent(EVENT_NM_EJECT, 0);
                        nightmareEvents.ScheduleEvent(EVENT_NM_GLUBTOK_AURA, 4000);
                        break;
                    case NIGHTMARE_STAGE_HELIX:
                        nightmareEvents.CancelEvent(EVENT_NM_ICICLE);
                        nightmareEvents.ScheduleEvent(EVENT_NM_HELIX_AURA, 2900);
                        break;
                    case NIGHTMARE_STAGE_FOE_REAPER:
                        nightmareEvents.CancelEvent(EVENT_NM_HELIX_SPIRIT_STRIKE);
                        nightmareEvents.ScheduleEvent(EVENT_NM_REAPER_DESPAWN_SPIDERS, 2000);
                        break;
                    case NIGHTMARE_STAGE_RIPSNARL:
                        nightmareEvents.ScheduleEvent(EVENT_NM_RIPSNARL_AURA, 2500);
                        break;
                    default:
                        break;
                }
            }

            // a boss copy that fights (the copy of the intro stays immune to players and creatures)
            Creature* NmFighter(uint32 entry, Position const& pos)
            {
                Creature* creature = NmSummon(entry, pos);
                if (creature)
                {
                    creature->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_IMMUNE_TO_PC | UNIT_FLAG_IMMUNE_TO_NPC | UNIT_FLAG_STUNNED);
                    creature->SetReactState(REACT_AGGRESSIVE);
                }
                return creature;
            }

            void SummonWorgen(Position const* positions, Creature* victim)
            {
                for (uint8 i = 0; i < 3; ++i)
                {
                    Creature* worgen = NmSummon(NPC_ENRAGED_WORGEN, positions[i]);
                    if (worgen && victim)
                    {
                        worgen->AddThreat(victim, 1000.0f);
                        worgen->AI()->AttackStart(victim);
                    }
                }
            }

            void UpdateNightmare(uint32 diff)
            {
                // The clock only has to run while something is queued
                if (nightmareEvents.Empty() && nmTalks.empty())
                {
                    nmClock = 0;
                    return;
                }

                nmClock += diff;

                // the talk / despawn queue
                for (size_t i = 0; i < nmTalks.size();)
                {
                    if (nmTalks[i].at > nmClock)
                    {
                        ++i;
                        continue;
                    }
                    NmTalk const talk = nmTalks[i];
                    nmTalks.erase(nmTalks.begin() + i);
                    if (Creature* who = GetNmCreature(talk.who))
                    {
                        if (talk.group == NM_TALK_DESPAWN)
                            who->DespawnOrUnsummon();
                        else if (who->IsAIEnabled && who->AI())
                            who->AI()->Talk(talk.group);
                    }
                }

                nightmareEvents.Update(diff);
                while (uint32 eventId = nightmareEvents.ExecuteEvent())
                {
                    switch (eventId)
                    {
                        case EVENT_NM_BOARD_TRAP:
                        {
                            Creature* trap = GetNmCreature(trapGUID);
                            if (!trap)
                                break;
                            BoardTrap(trap);
                            nightmareEvents.ScheduleEvent(EVENT_NM_TRAP_SEATED, 1000);
                            nightmareEvents.ScheduleEvent(EVENT_NM_SUMMON_VALVES, 2900);
                            nightmareEvents.ScheduleEvent(EVENT_NM_TRAP_MOVE_DOWN, 6000);
                            break;
                        }
                        case EVENT_NM_TRAP_SEATED:
                        {
                            // Seat the riders that the first boarding missed
                            Creature* trap = GetNmCreature(trapGUID);
                            Vehicle* kit = trap ? trap->GetVehicleKit() : NULL;
                            if (!kit)
                                break;
                            uint32 alive = 0;
                            uint32 seated = 0;
                            ForEachAlivePlayer([&](Player* player)
                            {
                                ++alive;
                                if (player->GetVehicleBase() == trap)
                                    ++seated;
                            });
                            if (seated < alive)
                                BoardTrap(trap);
                            break;
                        }
                        case EVENT_NM_SUMMON_VALVES:
                            for (uint8 i = 0; i < 4; ++i)
                                NmSummon(NPC_STEAM_VALVE, steamValvePos[i]);
                            if (Creature* trap = GetNmCreature(trapGUID))
                                QueueTalk(trap->GetGUID(), SAY_NM_ACTIVATE_VALVES, 3300);
                            break;
                        case EVENT_NM_TRAP_MOVE_DOWN:
                            if (Creature* trap = GetNmCreature(trapGUID))
                            {
                                // 1.25 yards a second: the 7 yards to the slag take 5.6 seconds, then Molten Magma
                                trap->SetSpeed(MOVE_RUN, 1.25f / 7.0f);
                                trap->GetMotionMaster()->MovePoint(1, trapBunnyMoveDownPos.GetPositionX(), trapBunnyMoveDownPos.GetPositionY(), trapBunnyMoveDownPos.GetPositionZ(), false);
                                nightmareEvents.ScheduleEvent(EVENT_NM_TRAP_MAGMA, 5600);
                            }
                            break;
                        case EVENT_NM_TRAP_MAGMA:
                            if (Creature* trap = GetNmCreature(trapGUID))
                                trap->CastSpell(trap, SPELL_MOLTEN_MAGMA, true);
                            break;
                        case EVENT_NM_EJECT:
                            EjectFromTrap();
                            trapGUID = 0;
                            break;
                        case EVENT_NM_GLUBTOK_AURA:
                            NmStageAura(0);
                            QueueTalk(announcerGUID, SAY_NM_ELIXIR_TAKES_HOLD, 200);
                            nightmareEvents.ScheduleEvent(EVENT_NM_GLUBTOK_SETUP, 800);
                            break;
                        case EVENT_NM_GLUBTOK_SETUP:
                        {
                            if (Creature* vanessa = NmSummon(NPC_VANESSA_VANCLEEF_NIGHTMARE, vanessaNightmareSpawnPos[0]))
                            {
                                vanessaNmGUID = vanessa->GetGUID();
                                QueueTalk(vanessaNmGUID, SAY_NM_GLUBTOK_1, 1000);
                                QueueTalk(vanessaNmGUID, SAY_NM_GLUBTOK_2, 9400);
                                QueueTalk(vanessaNmGUID, SAY_NM_ENTERED_GLUBTOK, 14100);
                                QueueTalk(vanessaNmGUID, SAY_NM_BACK_TO_THE_SHIP, 18900);
                                QueueDespawn(vanessaNmGUID, 21400);
                            }
                            if (Creature* intro = NmSummon(NPC_GLUBTOK_NIGHTMARE, glubtokNightmareIntroPos))
                                QueueDespawn(intro->GetGUID(), 21400);
                            for (Position const& pos : glubtokNightmareFireBunnyPos)
                            {
                                if (Creature* fire = NmSummon(NPC_GLUBTOK_NIGHTMARE_FIRE_BUNNY, pos, true))
                                {
                                    fire->CastSpell(fire, SPELL_NIGHTMARE_FLAMES, true);
                                    fireGUIDs.insert(fire->GetGUID());
                                }
                            }
                            nightmareEvents.ScheduleEvent(EVENT_NM_GLUBTOK_SPAWN, 14400);
                            nightmareEvents.ScheduleEvent(EVENT_NM_ICICLE, 14000);
                            break;
                        }
                        case EVENT_NM_GLUBTOK_SPAWN:
                            if (Creature* glubtok = NmFighter(NPC_GLUBTOK_NIGHTMARE, glubtokNightmareSpawnPos))
                                glubtokNmGUID = glubtok->GetGUID();
                            break;
                        case EVENT_NM_ICICLE:
                        {
                            // an icicle falls on a random player every 2.4 seconds until Glubtok dies
                            std::vector<Player*> targets;
                            ForEachAlivePlayer([&targets](Player* player)
                            {
                                if (player->GetExactDist2d(trapBunnyPos.GetPositionX(), trapBunnyPos.GetPositionY()) < 100.0f)
                                    targets.push_back(player);
                            });
                            if (!targets.empty())
                            {
                                Player* target = targets[urand(0, uint32(targets.size()) - 1)];
                                if (Creature* icicle = NmSummon(NPC_COLLAPSING_ICICLE, target->GetPosition(), true))
                                {
                                    icicle->CastSpell(icicle, SPELL_ICICLE, true);
                                    icicle->DespawnOrUnsummon(4000);
                                }
                            }
                            nightmareEvents.ScheduleEvent(EVENT_NM_ICICLE, 2400);
                            break;
                        }
                        case EVENT_NM_HELIX_AURA:
                            NmStageAura(SPELL_NIGHTMARE_AURA_GLUBTOK);
                            QueueTalk(announcerGUID, SAY_NM_NIGHTMARE_SHIFTS, 200);
                            nightmareEvents.ScheduleEvent(EVENT_NM_HELIX_DESPAWN_FIRE, 400);
                            break;
                        case EVENT_NM_HELIX_DESPAWN_FIRE:
                            DespawnNightmareSet(fireGUIDs);
                            nightmareEvents.ScheduleEvent(EVENT_NM_HELIX_SETUP, 2800);
                            break;
                        case EVENT_NM_HELIX_SETUP:
                            if (Creature* vanessa = NmSummon(NPC_VANESSA_VANCLEEF_NIGHTMARE, vanessaNightmareSpawnPos[1]))
                            {
                                vanessaNmGUID = vanessa->GetGUID();
                                QueueTalk(vanessaNmGUID, SAY_NM_HELIX_1, 1000);
                                QueueTalk(vanessaNmGUID, SAY_NM_HELIX_2, 7000);
                                QueueDespawn(vanessaNmGUID, 15300);
                            }
                            if (Creature* helix = NmSummon(NPC_HELIX_GEARBREAKER_NIGHTMARE, helixNightmareSpawnPos))
                            {
                                // attackable 7.7 seconds in, the spiders come 18.4 seconds in
                                helixNmGUID = helix->GetGUID();
                                QueueTalk(helixNmGUID, SAY_NM_ENTERED_HELIX, 17800);
                                QueueTalk(helixNmGUID, SAY_NM_SPIDERS, 19200);
                                nightmareEvents.ScheduleEvent(EVENT_NM_HELIX_UNFLAG, 7700);
                                nightmareEvents.ScheduleEvent(EVENT_NM_HELIX_SPIDERS, 18400);
                                nightmareEvents.ScheduleEvent(EVENT_NM_HELIX_SPIRIT_STRIKE, 4400);
                            }
                            break;
                        case EVENT_NM_HELIX_UNFLAG:
                            if (Creature* helix = GetNmCreature(helixNmGUID))
                            {
                                helix->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_IMMUNE_TO_PC | UNIT_FLAG_IMMUNE_TO_NPC);
                                helix->SetReactState(REACT_AGGRESSIVE);
                            }
                            break;
                        case EVENT_NM_HELIX_SPIDERS:
                            if (Creature* helix = GetNmCreature(helixNmGUID))
                            {
                                for (Position const& pos : nightmareSkitterlingPos)
                                    if (Creature* spider = helix->SummonCreature(NPC_NIGHTMARE_SKITTERLING, pos, TempSummonType::TEMPSUMMON_CORPSE_TIMED_DESPAWN, 5000))
                                        spiderGUIDs.insert(spider->GetGUID());
                                for (Position const& pos : darkwebDevourerPos)
                                    if (Creature* spider = helix->SummonCreature(NPC_DARKWEB_DEVOURER, pos, TempSummonType::TEMPSUMMON_CORPSE_TIMED_DESPAWN, 5000))
                                        spiderGUIDs.insert(spider->GetGUID());
                                if (Creature* spider = helix->SummonCreature(NPC_CHATTERING_HORROR, chatteringHorrorPos, TempSummonType::TEMPSUMMON_CORPSE_TIMED_DESPAWN, 5000))
                                    spiderGUIDs.insert(spider->GetGUID());
                            }
                            break;
                        case EVENT_NM_HELIX_SPIRIT_STRIKE:
                            if (Creature* helix = GetNmCreature(helixNmGUID))
                            {
                                if (helix->IsAlive() && helix->IsInCombat() && helix->GetVictim() && !helix->HasUnitState(UNIT_STATE_CASTING))
                                    helix->CastSpell(helix->GetVictim(), SPELL_SPIRIT_STRIKE, false);
                                nightmareEvents.ScheduleEvent(EVENT_NM_HELIX_SPIRIT_STRIKE, 3600);
                            }
                            break;
                        case EVENT_NM_REAPER_DESPAWN_SPIDERS:
                            DespawnNightmareSet(spiderGUIDs);
                            QueueTalk(announcerGUID, SAY_NM_NIGHTMARE_SHIFTS, 100);
                            NmStageAura(SPELL_NIGHTMARE_AURA_HELIX);
                            nightmareEvents.ScheduleEvent(EVENT_NM_REAPER_SETUP, 2600);
                            break;
                        case EVENT_NM_REAPER_SETUP:
                        {
                            if (GameObject* door = instance->GetGameObject(FoundryDoorGUID))
                                door->SetGoState(GOState::GO_STATE_ACTIVE);
                            if (Creature* intro = NmSummon(NPC_FOE_REAPER_5000_NIGHTMARE, foeReaperNightmareIntroPos))
                                QueueDespawn(intro->GetGUID(), 10400);
                            if (Creature* vanessa = NmSummon(NPC_VANESSA_VANCLEEF_NIGHTMARE, vanessaNightmareSpawnPos[2]))
                            {
                                vanessaNmGUID = vanessa->GetGUID();
                                QueueTalk(vanessaNmGUID, SAY_NM_MECHANICAL_1, 1000);
                                QueueTalk(vanessaNmGUID, SAY_NM_MECHANICAL_2, 4600);
                                QueueTalk(vanessaNmGUID, SAY_NM_ENTERED_MECHANICAL, 8200);
                                QueueDespawn(vanessaNmGUID, 10400);
                            }
                            SummonSparkChains();
                            nightmareEvents.ScheduleEvent(EVENT_NM_REAPER_SPAWN, 9500);
                            break;
                        }
                        case EVENT_NM_REAPER_SPAWN:
                            if (Creature* reaper = NmFighter(NPC_FOE_REAPER_5000_NIGHTMARE, foeReaperNightmareSpawnPos))
                            {
                                reaper->RemoveAurasDueToSpell(SPELL_OFF_LINE);
                                reaper->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_PACIFIED | UNIT_FLAG_NON_ATTACKABLE);
                                reaperNmGUID = reaper->GetGUID();
                            }
                            break;
                        case EVENT_NM_RIPSNARL_AURA:
                            NmStageAura(SPELL_NIGHTMARE_AURA_MECHANICAL);
                            DespawnNightmareSet(sparkGUIDs);
                            QueueTalk(announcerGUID, SAY_NM_NIGHTMARE_SHIFTS, 150);
                            nightmareEvents.ScheduleEvent(EVENT_NM_RIPSNARL_SETUP, 2600);
                            break;
                        case EVENT_NM_RIPSNARL_SETUP:
                            if (Creature* intro = NmSummon(NPC_RIPSNARL_NIGHTMARE, ripsnarlNightmareIntroPos))
                                QueueDespawn(intro->GetGUID(), 16500);
                            if (Creature* vanessa = NmSummon(NPC_VANESSA_VANCLEEF_NIGHTMARE, vanessaNightmareSpawnPos[3]))
                            {
                                vanessaNmGUID = vanessa->GetGUID();
                                QueueTalk(vanessaNmGUID, SAY_NM_RIPSNARL_1, 2200);
                                QueueTalk(vanessaNmGUID, SAY_NM_RIPSNARL_2, 9350);
                                QueueTalk(vanessaNmGUID, SAY_NM_ENTERED_RIPSNARL, 14150);
                                QueueDespawn(vanessaNmGUID, 16500);
                            }
                            nightmareEvents.ScheduleEvent(EVENT_NM_EMME, 14400);
                            break;
                        case EVENT_NM_EMME:
                            if (GameObject* door = instance->GetGameObject(IronCladDoorGUID))
                                door->SetGoState(GOState::GO_STATE_ACTIVE);
                            if (Creature* emme = NmSummon(NPC_EMME_HARRINGTON, emmeHarringtonSpawnPos))
                            {
                                emmeGUID = emme->GetGUID();
                                SummonWorgen(enragedWorgenEmmeSpawnPos, emme);
                            }
                            deadWorgen = 0;
                            QueueTalk(announcerGUID, SAY_NM_SAVE_EMME, 3500);
                            break;
                        case EVENT_NM_ERIK:
                            if (Creature* erik = NmSummon(NPC_ERIK_HARRINGTON, erikHarringtonSpawnPos))
                            {
                                erikGUID = erik->GetGUID();
                                SummonWorgen(enragedWorgenErikSpawnPos, erik);
                            }
                            deadWorgen = 0;
                            break;
                        case EVENT_NM_JAMES:
                        {
                            Creature* james = NmSummon(NPC_JAMES_HARRINGTON, jamesHarringtonSpawnPos);
                            Creature* calissa = NmSummon(NPC_CALISSA_HARRINGTON, calissaHarringtonSpawnPos);
                            if (james && calissa)
                            {
                                jamesGUID = james->GetGUID();
                                calissaGUID = calissa->GetGUID();
                                james->SetReactState(REACT_PASSIVE);
                                calissa->EnterVehicle(james, 0);
                                calissa->SetInCombatWith(james);
                                calissa->CastSpell(calissa, SPELL_CHOKING, true);
                            }
                            QueueTalk(announcerGUID, SAY_NM_SAVE_CALISSA, 3400);
                            break;
                        }
                        case EVENT_NM_VANESSA_BOSS:
                            if (Creature* vanessa = GetNmCreature(uiVanessaGUID))
                                vanessa->SetVisible(true);
                            DoStartTimedAchievement(ACHIEVEMENT_TIMED_TYPE_EVENT, ACHIEVEMENT_EVENT);
                            break;
                        case EVENT_NM_CHECK_DEAD_PLAYERS:
                        {
                            // Also polled while the group lives: the last one alive may leave after the others died
                            uint32 alive = 0;
                            ForEachAlivePlayer([&alive](Player*) { ++alive; });
                            if (!alive)
                                ResetNightmare();
                            else if (uiVanessaEvent >= NIGHTMARE_STAGE_TRAP)
                                nightmareEvents.ScheduleEvent(EVENT_NM_CHECK_DEAD_PLAYERS, NM_STUCK_CHECK_INTERVAL);
                            break;
                        }
                        case EVENT_NM_RESPAWN_NOTE:
                            if (uiVanessaEvent == NOT_STARTED && GetBossState(DATA_CAPTAIN) == DONE)
                                instance->SummonCreature(NPC_NOTE_FROM_VANESSA, notePos);
                            break;
                        default:
                            break;
                    }
                }
            }

            // the mechanical nightmare: six Lightning Platters and their spark chains
            void SummonSparkChains()
            {
                for (uint8 i = 0; i < 6; ++i)
                {
                    Position const& base = lightningPlatterPos[i];
                    float const height = lightningPlatterHeight[i];
                    bool const clockwise = urand(0, 1) == 1;
                    Position const platterPos(base.GetPositionX(), base.GetPositionY(), height, base.GetOrientation());
                    if (Creature* platter = NmSummon(NPC_VANESSA_LIGHTNING_PLATTER, platterPos, true))
                    {
                        sparkGUIDs.insert(platter->GetGUID());
                        platter->SetDisableGravity(true);
                        platter->GetMotionMaster()->MoveCirclePath(base.GetPositionX(), base.GetPositionY(), height, 1.0f, clockwise, 9, 0.4f);
                    }

                    float const lateral = base.GetOrientation() + float(M_PI) / 2.0f;
                    for (uint32 s = 0; s < LIGHTNING_STALKER_COUNT; ++s)
                    {
                        float const d = lightningStalkerOffset[s];
                        Position const pos(base.GetPositionX() + d * std::cos(lateral), base.GetPositionY() + d * std::sin(lateral), height - 1.0f, 0.0f);
                        Creature* stalker = NmSummon(NPC_VANESSA_LIGHTNING_STALKER, pos, true);
                        if (!stalker)
                            continue;
                        sparkGUIDs.insert(stalker->GetGUID());
                        stalker->SetDisableGravity(true);
                        stalker->GetMotionMaster()->MoveCirclePath(base.GetPositionX(), base.GetPositionY(), height - 1.0f, std::fabs(d), clockwise, 24, std::fabs(d) * LIGHTNING_PLATTER_OMEGA);
                    }
                }
            }

            void NightmareUnitDeath(Unit* unit)
            {
                if (unit->GetTypeId() == TypeID::TYPEID_PLAYER)
                {
                    nightmareEvents.RescheduleEvent(EVENT_NM_CHECK_DEAD_PLAYERS, 200);
                    return;
                }

                Creature* creature = unit->ToCreature();
                if (!creature)
                    return;

                uint64 const guid = creature->GetGUID();
                switch (creature->GetEntry())
                {
                    case NPC_GLUBTOK_NIGHTMARE:
                        if (guid == glubtokNmGUID && uiVanessaEvent == NIGHTMARE_STAGE_GLUBTOK)
                        {
                            creature->DespawnOrUnsummon(4000);
                            SetNightmareStage(NIGHTMARE_STAGE_HELIX);
                        }
                        break;
                    case NPC_HELIX_GEARBREAKER_NIGHTMARE:
                        if (guid == helixNmGUID && uiVanessaEvent == NIGHTMARE_STAGE_HELIX)
                        {
                            creature->DespawnOrUnsummon(5500);
                            SetNightmareStage(NIGHTMARE_STAGE_FOE_REAPER);
                        }
                        break;
                    case NPC_FOE_REAPER_5000_NIGHTMARE:
                        if (guid == reaperNmGUID && uiVanessaEvent == NIGHTMARE_STAGE_FOE_REAPER)
                        {
                            creature->DespawnOrUnsummon(3600);
                            SetNightmareStage(NIGHTMARE_STAGE_RIPSNARL);
                        }
                        break;
                    case NPC_ENRAGED_WORGEN:
                        if (uiVanessaEvent != NIGHTMARE_STAGE_RIPSNARL)
                            break;
                        creature->DespawnOrUnsummon(5000);
                        if (++deadWorgen < 3)
                            break;
                        deadWorgen = 0;
                        if (Creature* emme = GetNmCreature(emmeGUID))
                        {
                            emme->CastSpell(emme, SPELL_ADRENALINE, true);
                            emme->DespawnOrUnsummon();
                            emmeGUID = 0;
                            QueueTalk(announcerGUID, SAY_NM_SAVE_ERIK, 4000);
                            nightmareEvents.ScheduleEvent(EVENT_NM_ERIK, 5800);
                        }
                        else if (Creature* erik = GetNmCreature(erikGUID))
                        {
                            erik->CastSpell(erik, SPELL_ADRENALINE, true);
                            erik->DespawnOrUnsummon();
                            erikGUID = 0;
                            nightmareEvents.ScheduleEvent(EVENT_NM_JAMES, 200);
                        }
                        break;
                    case NPC_JAMES_HARRINGTON:
                        if (guid != jamesGUID || uiVanessaEvent != NIGHTMARE_STAGE_RIPSNARL)
                            break;
                        creature->DespawnOrUnsummon(2300);
                        if (Creature* calissa = GetNmCreature(calissaGUID))
                        {
                            calissa->RemoveAllAuras();
                            calissa->DespawnOrUnsummon(2200);
                        }
                        QueueTalk(announcerGUID, SAY_NM_ELIXIR_WEARS_OFF, 250);
                        CleanupNightmareKeepAnnouncer();
                        uiVanessaEvent = DONE;
                        SaveToDB();
                        nightmareEvents.ScheduleEvent(EVENT_NM_VANESSA_BOSS, 2250);
                        break;
                    case NPC_EMME_HARRINGTON:
                    case NPC_ERIK_HARRINGTON:
                    case NPC_CALISSA_HARRINGTON:
                        if (uiVanessaEvent == NIGHTMARE_STAGE_RIPSNARL)
                        {
                            creature->CastSpell(creature, SPELL_NIGHTMARES_END, true);
                            ResetNightmare();
                        }
                        break;
                    default:
                        break;
                }
            }

            // The end of the Nightmare: every creature goes, except the announcer (it still says that the elixir wears off) and
            // James and Calissa (they have their own despawn timers)
            void CleanupNightmareKeepAnnouncer()
            {
                static uint32 const playerAuras[] = { SPELL_NIGHTMARE_ELIXIR, SPELL_NIGHTMARE_AURA, SPELL_VANESSAS_BLACKOUT,
                    SPELL_NIGHTMARE_AURA_GLUBTOK, SPELL_NIGHTMARE_AURA_HELIX, SPELL_NIGHTMARE_AURA_MECHANICAL, SPELL_NIGHTMARE_AURA_RIPSNARL };
                for (uint32 spell : playerAuras)
                    DoRemoveAurasDueToSpellOnPlayers(spell);
                DespawnNightmareSet(spiderGUIDs);
                fireGUIDs.clear();
                sparkGUIDs.clear();
                for (uint64 guid : nightmareGUIDs)
                {
                    if (guid == announcerGUID || guid == jamesGUID || guid == calissaGUID)
                        continue;
                    if (Creature* creature = GetNmCreature(guid))
                        creature->DespawnOrUnsummon();
                }
                nightmareGUIDs.clear();
                if (Creature* bunny = GetNmCreature(announcerGUID))
                    bunny->DespawnOrUnsummon(3000);
                nightmareEvents.Reset();
            }

            // the four Steam Valves of the trap each send one click here
            void ValveActivated()
            {
                if (uiVanessaEvent != NIGHTMARE_STAGE_TRAP)
                    return;

                ++valvesActivated;
                if (valvesActivated < 4)
                    return;

                for (uint64 guid : nightmareGUIDs)
                    if (Creature* creature = GetNmCreature(guid))
                        if (creature->GetEntry() == NPC_STEAM_VALVE)
                            creature->DespawnOrUnsummon(10000);
                SetNightmareStage(NIGHTMARE_STAGE_GLUBTOK);
            }

            std::string GetSaveData() OVERRIDE
            {
                OUT_SAVE_INST_DATA;

                std::ostringstream saveStream;
                saveStream << "D M " << GetBossSaveData() << State << ' ' << uiVanessaEvent << ' ';

                OUT_SAVE_INST_DATA_COMPLETE;
                return saveStream.str();
            }

            void Load(char const* in) OVERRIDE
            {
                if (!in)
                {
                    OUT_LOAD_INST_DATA_FAIL;
                    return;
                }

                OUT_LOAD_INST_DATA(in);

                char dataHead1, dataHead2;

                std::istringstream loadStream(in);
                loadStream >> dataHead1 >> dataHead2;

                if (dataHead1 == 'D' && dataHead2 == 'M')
                {
                    for (uint8 i = 0; i < MAX_ENCOUNTER; ++i)
                    {
                        uint32 tmpState;
                        loadStream >> tmpState;
                        if (tmpState == IN_PROGRESS || tmpState > SPECIAL)
                            tmpState = NOT_STARTED;
                        SetBossState(i, EncounterState(tmpState));
                    }

                    loadStream >> State;

                    if (State != CANNON_NOT_USED)
                        if (GameObject* pIronCladDoor = instance->GetGameObject(IronCladDoorGUID))
                            pIronCladDoor->SetGoState(GOState::GO_STATE_ACTIVE_ALTERNATIVE);

                    loadStream >> uiVanessaEvent;
                    if (uiVanessaEvent != DONE)
                        uiVanessaEvent = NOT_STARTED;
                }
                else
                    OUT_LOAD_INST_DATA_FAIL;

                OUT_LOAD_INST_DATA_COMPLETE;
            }

        private:
            uint64 uiGlubtokGUID;
            uint64 uiHelixGUID;
            uint64 uiOafGUID;
            uint64 uiFoereaperGUID;
            uint64 uiAdmiralGUID;
            uint64 uiCaptainGUID;

            uint64 GoblinTeleporterGUID;
            uint64 HeavyDoorGUID;
            uint64 HeavyDoor2GUID;
            uint64 IronCladDoorGUID;
            uint64 DefiasCannonGUID;
            uint64 DoorLeverGUID;

            uint32 State;
            uint32 uiVanessaEvent;
            uint32 OverchargeCount;

            // Vanessa's Nightmare
            EventMap nightmareEvents;
            std::vector<NmTalk> nmTalks;
            uint32 nmClock;
            uint8 valvesActivated;
            uint8 deadWorgen;
            bool noteSpawnChecked;
            GuidSet nightmareGUIDs;
            GuidSet fireGUIDs;
            GuidSet spiderGUIDs;
            GuidSet sparkGUIDs;
            uint64 uiVanessaGUID;
            uint64 anchorBunnyGUID;
            uint64 FoundryDoorGUID;
            uint64 trapGUID;
            uint64 announcerGUID;
            uint64 vanessaNmGUID;
            uint64 glubtokNmGUID;
            uint64 helixNmGUID;
            uint64 reaperNmGUID;
            uint64 emmeGUID;
            uint64 erikGUID;
            uint64 calissaGUID;
            uint64 jamesGUID;

            uint32 TeamInInstance;
            std::vector<uint64> FactionNpcGUIDs;
        };

        InstanceScript* GetInstanceScript(InstanceMap* map) const OVERRIDE
        {
            return new instance_deadmines_InstanceMapScript(map);
        }
};

void AddSC_instance_deadmines()
{
    new instance_deadmines();
}
