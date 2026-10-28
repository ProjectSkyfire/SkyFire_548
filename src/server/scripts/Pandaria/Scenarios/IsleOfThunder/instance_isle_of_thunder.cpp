/*
* This file is part of Project SkyFire https://www.projectskyfire.org.
* See LICENSE.md file for Copyright information
*/

#include "ScriptMgr.h"
#include "InstanceScript.h"
#include "isle_of_thunder.h"
#include "ScenarioMgr.h"
#include "LFGMgr.h"
#include "Group.h"

WorldLocation ExitPos[2] =
{
    WorldLocation{ 1064, 7168.998f, 6307.999f, 16.016f, 2.3503f },
    WorldLocation{ 1064, 6142.931f, 5037.585f, 37.008f, 0.0462f },
};

class instance_isle_of_thunder : public InstanceMapScript
{
    public:
        instance_isle_of_thunder() : InstanceMapScript("instance_isle_of_thunder", 1126) { }

        struct instance_isle_of_thunder_InstanceScript : public InstanceScript
        {
            EventMap m_mEvents;

            instance_isle_of_thunder_InstanceScript(Map* map) : InstanceScript(map) { }

            uint64 vereesaGUID;
            uint64 playerGUID;
            uint64 marakahGUID;
            uint64 tulachekGUID;
            uint64 elsiaGUID;
            uint32 step1;
            uint32 step2;
            uint32 step3;
            uint32 step4;
            uint32 step5;
            uint32 step6;
            uint32 pterrorCount;
            uint64 destructableWallGUID;
            uint64 teardownElsiaGUID;
            uint64 alchuklaGUID;
            uint64 additClhucklaGUID;
            uint64 teardownLorthemarGUID;
            uint64 additTeardownElsiaGUID;
            uint64 thunderwingGUID;
            uint64 elsiaToSkiesGUID;
            uint64 horrakiGUID;
            uint64 caveinGUID;
            uint64 moguDoorGUID;
            uint64 taoshiGUID;
            uint64 husengCollisionGUID;
            uint64 thunderwingStormseaGUID;
            uint64 taoshiStormseaGUID;
            uint64 taoshiStormseaShipGUID;
            uint64 captainHalukalGUID;
            uint64 husengGUID;
            uint64 shanbuStormseaGUID;
            uint64 stormseaMoguDoorGUID;
            uint64 hordeSpiritWallGUID;
            uint64 centerSpiritWallGUID;
            uint64 shanbuVereesaGUID;
            uint64 shanbuElsiaGUID;
            uint64 shanbuLorthemarGUID;
            uint64 shanbuJainaGUID;
            uint64 shanbuHordeGatesGUID;
            uint64 shanbuCenterGatesGUID;
            uint64 shanbuGUID;
            uint64 taranZhuGUID;
            uint64 shanbuTaoshiGUID;
            uint64 yaliaGUID;
            uint64 additShanbuJainaGUID;
            uint64 additShanbuLorthemarGUID;
            uint64 additShanbuVereesaGUID;
            uint64 nalakGUID;
            uint64 moguCrucibleGUID;
            uint64 wrathionGUID;
            uint64 thunderForgeGUID;
            uint64 thunderForgeDoorGUID;
            uint64 shadopanDefenderGUID;
            uint64 innerForgeGUID;
            uint64 blacksmithGUID;
            uint64 celestialDefenderGUID;

            uint32 teamInstance;
            uint32 deactivatedSentryCount;
            uint32 zandalariCount;
            uint32 palaceGuardsCount;
            uint32 shanbuGuardsCount;
            uint32 phaseId;
            uint32 roleId;
            uint32 _scenarioId;

            std::list<uint64> covenantHyppoGUIDs;
            std::list<uint64> seekerGUIDs;
            std::list<uint64> bloodhawkGUIDs;
            std::list<uint64> barricadeGUIDs;
            std::list<uint64> portalGUIDs;
            std::list<uint64> teardownAssaultGUIDs;
            std::list<uint64> teardownAssaultEnemyGUIDs;
            std::list<uint64> forgeAssaultGUIDs;
            std::list<uint64> shanbuAssaultGUIDs;
            std::list<uint64> zandalariTrapGUIDs;
            std::list<uint64> palaceGuardGUIDs;
    
            void Initialize() OVERRIDE
            {
                vereesaGUID = 0;
                playerGUID = 0;
                marakahGUID = 0;
                tulachekGUID = 0;
                elsiaGUID = 0;
                step1                    = 0;
                step2                    = 0;
                step3                    = 0;
                step4                    = 0;
                step5                    = 0;
                step6                    = 0;
                destructableWallGUID = 0;
                teardownElsiaGUID = 0;
                alchuklaGUID = 0;
                additClhucklaGUID = 0;
                teardownLorthemarGUID = 0;
                additTeardownElsiaGUID = 0;
                teamInstance             = 0;
                thunderwingGUID = 0;
                elsiaToSkiesGUID = 0;
                horrakiGUID = 0;
                pterrorCount             = 0;
                moguDoorGUID = 0;
                taoshiGUID = 0;
                husengCollisionGUID = 0;
                thunderwingStormseaGUID = 0;
                taoshiStormseaGUID = 0;
                taoshiStormseaShipGUID = 0;
                deactivatedSentryCount   = 0;
                captainHalukalGUID = 0;
                husengGUID = 0;
                shanbuStormseaGUID = 0;
                stormseaMoguDoorGUID = 0;
                hordeSpiritWallGUID = 0;
                centerSpiritWallGUID = 0;
                shanbuVereesaGUID = 0;
                shanbuElsiaGUID = 0;
                shanbuLorthemarGUID = 0;
                shanbuJainaGUID = 0;
                shanbuHordeGatesGUID = 0;
                shanbuCenterGatesGUID = 0;
                shanbuGUID = 0;
                taranZhuGUID = 0;
                shanbuTaoshiGUID = 0;
                yaliaGUID = 0;
                zandalariCount           = 0;
                palaceGuardsCount        = 0;
                additShanbuJainaGUID = 0;
                additShanbuLorthemarGUID = 0;
                additShanbuVereesaGUID = 0;
                shanbuGuardsCount        = 0;
                nalakGUID = 0;
                phaseId                  = 0;
                roleId                   = 0;
                moguCrucibleGUID = 0;
                wrathionGUID = 0;
                thunderForgeGUID = 0;
                thunderForgeDoorGUID = 0;
                shadopanDefenderGUID = 0;
                innerForgeGUID = 0;
                blacksmithGUID = 0;
                celestialDefenderGUID = 0;
                _scenarioId              = 0;

                covenantHyppoGUIDs.clear();
                bloodhawkGUIDs.clear();
                barricadeGUIDs.clear();
                seekerGUIDs.clear();
                portalGUIDs.clear();
                teardownAssaultGUIDs.clear();
                forgeAssaultGUIDs.clear();
                shanbuAssaultGUIDs.clear();
                zandalariTrapGUIDs.clear();
                palaceGuardGUIDs.clear();
            }

            void OnPlayerEnter(Player* player) OVERRIDE
            {
                if (!playerGUID)
                {
                    playerGUID = player->GetGUID();
                    teamInstance = player->GetTeam();
                    roleId = player->GetRoleForGroup(player->GetTalentSpecialization());

                    if (instance->ToInstanceMap()->IsLFGMap() && player->GetGroup())
                    {
                        // We should set up first stage of scenario type there
                        switch (sLFGMgr->GetDungeon(player->GetGroup()->GetGUID()))
                        {
                            case LFG_DUNGEON_ASSAULT_ON_SHAOLMARA:
                                sScenarioMgr->SendScenarioState(player, 1126, 0, 0, CRITERIA_TREE_ID_SHAOLMARA_1, SCENARIO_ID_SHAOLMARA);
                                break;
                            case LFG_DUNGEON_ASSAULT_ON_ZEB_TULA:
                                sScenarioMgr->SendScenarioState(player, 1126, 0, 0, CRITERIA_TREE_ID_ZEBTULA_1, SCENARIO_ID_ZEBTULA);
                                break;
                            case LFG_DUNGEON_BREAK_THIS_WALL_A:
                                player->CastSpell(player, 137570, true); // intro scene that launch scenario
                                sScenarioMgr->SendScenarioState(player, 1126, 0, 0, CRITERIA_TREE_ID_TEAR_DOWN_A_1, SCENARIO_ID_TEAR_DOWN_ALLIANCE);
                                break;
                            case LFG_DUNGEON_BREAK_THIS_WALL_H:
                                player->CastSpell(player, 137570, true); // intro scene that launch scenario
                                sScenarioMgr->SendScenarioState(player, 1126, 0, 0, CRITERIA_TREE_ID_TEAR_DOWN_H_1, SCENARIO_ID_TEAR_DOWN_HORDE);
                                break;
                            case LFG_DUNGEON_TO_SKIES_A:
                            case LFG_DUNGEON_TO_SKIES_H:
                                sScenarioMgr->SendScenarioState(player, 1126, 0, 0, CRITERIA_TREE_ID_iN_THE_SKY_1, SCENARIO_ID_IN_THE_SKY);

                                if (GetData64(FACTION_DATA))
                                {
                                    instance->SummonCreature(NPC_LORTHEMAR_THRON_IN_SKY, toSkiesHordeSummPos[0]);
                                    instance->SummonCreature(NPC_THUNDERWING, toSkiesHordeSummPos[1]);
                                    instance->SummonCreature(NPC_SCOUT_CAPTAIN_ELSIA_IN_SKY, toSkiesHordeSummPos[2]);
                                }
                                else
                                {
                                    instance->SummonCreature(NPC_LADY_JAINA_IN_SKY, toSkiesAllianceSummPos[0]);
                                    instance->SummonCreature(NPC_VEREESA_WINDRUNNED_IN_SKY, toSkiesAllianceSummPos[1]);
                                    instance->SummonCreature(NPC_THUNDERWING, toSkiesAllianceSummPos[2]);
                                }
                                break;
                            case LFG_THUNDER_FORGE:
                                sScenarioMgr->SendScenarioState(player, 1126, 0, 0, CRITERIA_TREE_ID_THUNDER_FORGE_1, SCENARIO_ID_THUNDER_FORGE);
                                phaseId = SCENARIO_THUNDER_FORGE;
                                instance->SummonCreature(NPC_WRATHION, wrathionSummPos);
                                break;
                            case LFG_DUNGEON_STORMSEA_LANDING:
                                sScenarioMgr->SendScenarioState(player, 1126, 0, 0, CRITERIA_TREE_ID_STORMSEA_LANDING_1, SCENARIO_ID_STORMSEA_LANDING);
                                break;
                            case LFG_DUNGEON_FALL_OF_SHANBU_A:
                            case LFG_DUNGEON_FALL_OF_SHANBU_H:
                                phaseId = SCENARIO_FALL_OF_SHANBU;
                                sScenarioMgr->SendScenarioState(player, 1126, 0, 0, CRITERIA_TREE_ID_FALL_OF_SHANBU_1, SCENARIO_ID_FALL_OF_SHANBU);

                                if (GetData64(FACTION_DATA))
                                {
                                    instance->SummonCreature(NPC_SHANBU_LORTHEMAR, HordeShanbuIntro[1]);
                                    instance->SummonCreature(NPC_SHANBU_ELSIA, HordeShanbuIntro[0]);
                                }
                                else
                                {
                                    instance->SummonCreature(NPC_SHANBU_JAINA, AllianceShanbuIntro[1]);
                                    instance->SummonCreature(NPC_SHANBU_VEREESA, AllianceShanbuIntro[0]);
                                }
                                break;
                        }
                    }
                }
            }
    
            void OnCreatureCreate(Creature* creature) OVERRIDE 
            {
                if (!teamInstance)
                {
                    Map::PlayerList const &players = instance->GetPlayers();
                    if (!players.isEmpty())
                        if (Player* player = players.begin()->GetSource())
                            teamInstance = player->GetTeam();
                }

                switch (creature->GetEntry())
                {
                    case NPC_VEREESA_WINDRUNNER:
                        vereesaGUID = creature->GetGUID();
                        break;
                    case NPC_SILVER_COVENANT_HYPPOGRYPH:
                    case NPC_VEERISA_HIPPOGRYPH:
                        covenantHyppoGUIDs.push_back(creature->GetGUID());
                        break;
                    case NPC_SUNREAVER_PYROMANCER: // who it? why in alliance zone?
                    case NPC_SUNREAVER_CAPTAIN:
                    case NPC_ZEBTULA_GENERIC_BUNNY:
                    case NPC_ZEBTULA_BARRIER_BUNNY:
                    case NPC_LORTEMAR_THERON_UNK:
                    case NPC_CARGO_HAULER:
                    case NPC_BUNNY_ZTO:
                    case NPC_PURPLE_HAWKSTRIDER:
                    case NPC_SCENARIO_TEAR_DOWN_BUNNY:
                    case NPC_TOTALY_GENERIC_BUNNY:
                    case NPC_DEACTIVATED_ACCESS_GEN:
                        creature->SetVisible(false);
                        creature->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_PACIFIED);
                        break;
                    case NPC_ZEBTULA_STONESHIELD:
                    case NPC_ZEBTULA_SPEARANGER:
                    case NPC_ZANDALARI_BARRICADE_A:
                    case NPC_ZANDALARI_BARRICADE_H:
                        creature->SetVisible(false);
                        creature->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_PACIFIED);
                        barricadeGUIDs.push_back(creature->GetGUID());
                        break;
                    case NPC_ARCANITAL_MARAKAH:
                        if (creature->GetDBTableGUIDLow())
                        {
                            creature->SetVisible(false);
                            creature->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_PACIFIED);
                            marakahGUID = creature->GetGUID();
                        }
                        break;
                    case NPC_SCOUT_CAPTAIN_ELSIA:
                        elsiaGUID = creature->GetGUID();
                        break;
                    case NPC_SUNREAVER_BLOODHAWK:
                    case NPC_ELSIA_BLOODHAWK:
                        if (creature->GetPositionY() < 6480.0f)
                            bloodhawkGUIDs.push_back(creature->GetGUID());
                        break;
                    case NPC_DARKSEEKER_GUARDIAN:
                    case NPC_SUNREAVER_MAGUS:
                    case NPC_ELINA_ZARALAE:
                    case NPC_KETHIEL_SUNLANCE:
                    case NPC_HALDURON_BRIGHTWING:
                    case NPC_HIGH_MAGISTER_ROMMAT:
                    case NPC_HIGH_MAGE_ETHAS:
                    case NPC_LANESH_THE_STEALWEAVER:
                    case NPC_AMALIA_PENSHIR:
                    case NPC_DEZKO:
                    case NPC_MAGISTRIX_SAIA:
                    case NPC_RANGER_SHALAN:
                    case NPC_RISHEL:
                    case NPC_CAPTAIN_AERTHAS:
                    case NPC_AMARIEL_SUNSWORN:
                    case NPC_GIRANA_THE_BLOODED:
                    case NPC_MAGISTER_HATHOREL:
                    case NPC_HIGH_ARCANIST_SAVOR:
                    case NPC_SUNREAVER_GOLEM:
                    case NPC_ELYNARA:
                    case NPC_SILVERMOON_RUNESEEKER:
                    case NPC_HARLAN_WHITEDAWN:
                        creature->RemoveFlag(UNIT_FIELD_NPC_FLAGS, UNIT_NPC_FLAG_QUESTGIVER);

                        if (creature->GetPositionX() > 6892.0f)
                        {
                            creature->SetVisible(false);
                            creature->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_PACIFIED);
                            seekerGUIDs.push_back(creature->GetGUID());
                        }
                        break;
                    case NPC_ARCANITAL_TULACHEK:
                        if (creature->GetDBTableGUIDLow())
                        {
                            creature->SetVisible(false);
                            creature->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_PACIFIED);
                            tulachekGUID = creature->GetGUID();
                        }
                        break;
                    case NPC_TEARDOWN_SCOUT_CAPTAIN_ELSIA:
                        if (!GetData64(FACTION_DATA))
                            creature->UpdateEntry(NPC_TEARDOWN_VEREESA_WINDRUNNER, ALLIANCE);

                        if (creature->GetDBTableGUIDLow())
                            teardownElsiaGUID = creature->GetGUID();
                        else
                            additTeardownElsiaGUID = creature->GetGUID();
                        break;
                    case NPC_TEARDOWN_VEREESA_WINDRUNNER:
                        if (!creature->GetDBTableGUIDLow())
                            additTeardownElsiaGUID = creature->GetGUID();
                        break;
                    case NPC_TEARDOWN_SUNREAVER_CAPTAIN:
                    case NPC_TEARDOWN_SUNREAVER_PYRO:
                    case NPC_TEARDOWN_SUNREAVER_SPELLBLADE:
                    case NPC_TEARDOWN_SUNREAVER_ASSASSIN:
                    case NPC_TEARDOWN_SUNREAVER_ASSASSIN_2:
                        if (!GetData64(FACTION_DATA))
                            creature->UpdateEntry(invAssaulterType.find(creature->GetEntry())->second, ALLIANCE);

                        teardownAssaultGUIDs.push_back(creature->GetGUID());
                        creature->SetVisible(false);
                        creature->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_PACIFIED);
                        break;
                    case NPC_WARGOD_ALCHUKLA:
                        if (creature->GetPositionX() > 6864.0f)
                        {
                            alchuklaGUID = creature->GetGUID();
                            creature->SetVisible(false);
                            creature->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_PACIFIED);
                        }
                        else
                        {
                            additClhucklaGUID = creature->GetGUID();
                            teardownAssaultEnemyGUIDs.push_back(creature->GetGUID());
                            creature->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_PACIFIED | UNIT_FLAG_NON_ATTACKABLE);
                        }
                        break;
                    case NPC_TEARDOWN_LORTHEMAR_THERONE:
                        if (!GetData64(FACTION_DATA))
                            creature->UpdateEntry(NPC_TEARDOWN_JAINA_PROUDMOORE, ALLIANCE);

                        teardownAssaultGUIDs.push_back(creature->GetGUID());
                        creature->SetVisible(false);
                        creature->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_PACIFIED);
                        teardownLorthemarGUID = creature->GetGUID();
                        break;
                    case NPC_TEARDOWN_ZANDALARI_STONESHIELD:
                        teardownAssaultEnemyGUIDs.push_back(creature->GetGUID());
                        break;
                    case NPC_THUNDERWING:
                        thunderwingGUID = creature->GetGUID();
                        break;
                    case NPC_SCOUT_CAPTAIN_ELSIA_IN_SKY:
                    case NPC_VEREESA_WINDRUNNED_IN_SKY:
                        if (!creature->GetDBTableGUIDLow())
                            elsiaToSkiesGUID = creature->GetGUID();
                        break;
                    case NPC_BEASTMASTER_HORAKI:
                        creature->SetVisible(false);
                        creature->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_PACIFIED | UNIT_FLAG_NON_ATTACKABLE);
                        horrakiGUID = creature->GetGUID();
                        break;
                    case NPC_TAOSHI:
                        if (creature->GetPositionZ() < 45.0f)
                            taoshiGUID = creature->GetGUID();
                        break;
                    case NPC_LORTHEMAR_THRON_IN_SKY:
                    case NPC_SUNREAVER_MAGISTER_FORGE:
                    case NPC_SUNREAVER_RANGER_FORGE:
                        if (creature->GetDBTableGUIDLow())
                        {
                            if (!GetData64(FACTION_DATA))
                                creature->UpdateEntry(invForgeType.find(creature->GetEntry())->second, ALLIANCE);

                            creature->SetVisible(false);
                            forgeAssaultGUIDs.push_back(creature->GetGUID());
                        }
                        break;
                    case NPC_THUNDERWING_STORMSEA:
                        thunderwingStormseaGUID = creature->GetGUID();
                        break;
                    case NPC_TAOSHI_STORMSEA:
                        taoshiStormseaGUID = creature->GetGUID();
                        break;
                    case NPC_TAOSHI_STORMSEA_SHIP:
                        creature->SetVisible(false);
                        taoshiStormseaShipGUID = creature->GetGUID();
                        break;
                    case NPC_CAPTAIN_HALUKAL:
                        captainHalukalGUID = creature->GetGUID();
                        break;
                    case NPC_HUSENG_THE_GATEKEEPER:
                        creature->SetVisible(false);
                        creature->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_PACIFIED | UNIT_FLAG_NON_ATTACKABLE);
                        husengGUID = creature->GetGUID();
                        break;
                    case NPC_SHAN_BU_STORMSEA:
                        creature->SetVisible(false);
                        creature->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_PACIFIED | UNIT_FLAG_NON_ATTACKABLE);
                        shanbuStormseaGUID = creature->GetGUID();
                        break;
                    case NPC_SILVER_COVENANT_SPELLBLADE_ST:
                        if (GetData64(FACTION_DATA))
                            creature->UpdateEntry(NPC_SUNREAVER_MAGUS, ALLIANCE);
                        break;
                    case NPC_YALIAS_TIGER:
                    case NPC_SHANBU_AETHAS:
                    case NPC_SILVER_COVENANT_SPELLBLOW:
                    case NPC_SUNREAVER_RANGER:
                    case NPC_ZANDALARI_STONE_SHIELD_3:
                        if (creature->GetDBTableGUIDLow())
                        {
                            shanbuAssaultGUIDs.push_back(creature->GetGUID());
                            creature->SetVisible(false);
                            creature->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_PACIFIED | UNIT_FLAG_NON_ATTACKABLE);
                        }
                        break;
                    case NPC_SHANBU_VEREESA:
                        if (creature->GetDBTableGUIDLow())
                        {
                            additShanbuVereesaGUID = creature->GetGUID();
                            shanbuAssaultGUIDs.push_back(creature->GetGUID());
                            creature->SetVisible(false);
                            creature->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_PACIFIED | UNIT_FLAG_NON_ATTACKABLE);
                        }
                        else
                            shanbuVereesaGUID = creature->GetGUID();
                        break;
                    case NPC_SHANBU_ELSIA:
                        if (creature->GetDBTableGUIDLow())
                        {
                            shanbuAssaultGUIDs.push_back(creature->GetGUID());
                            creature->SetVisible(false);
                            creature->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_PACIFIED | UNIT_FLAG_NON_ATTACKABLE);
                        }
                        else
                            shanbuElsiaGUID = creature->GetGUID();
                        break;
                    case NPC_SHANBU_JAINA:
                        if (creature->GetDBTableGUIDLow())
                        {
                            additShanbuJainaGUID = creature->GetGUID();
                            shanbuAssaultGUIDs.push_back(creature->GetGUID());
                            creature->SetVisible(false);
                            creature->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_PACIFIED | UNIT_FLAG_NON_ATTACKABLE);
                        }
                        else
                            shanbuJainaGUID = creature->GetGUID();
                        break;
                    case NPC_SHANBU_LORTHEMAR:
                        if (creature->GetDBTableGUIDLow())
                        {
                            additShanbuLorthemarGUID = creature->GetGUID();
                            shanbuAssaultGUIDs.push_back(creature->GetGUID());
                            creature->SetVisible(false);
                            creature->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_PACIFIED | UNIT_FLAG_NON_ATTACKABLE);
                        }
                        else
                            shanbuLorthemarGUID = creature->GetGUID();
                        break;
                    case NPC_SHAN_BU:
                        shanbuGUID = creature->GetGUID();
                        shanbuAssaultGUIDs.push_back(creature->GetGUID());
                        creature->SetVisible(false);
                        creature->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_PACIFIED | UNIT_FLAG_NON_ATTACKABLE);
                        break;
                    case NPC_SHANBU_TARAN_ZHU:
                        taranZhuGUID = creature->GetGUID();
                        shanbuAssaultGUIDs.push_back(creature->GetGUID());
                        creature->SetVisible(false);
                        creature->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_PACIFIED | UNIT_FLAG_NON_ATTACKABLE);
                        break;
                    case NPC_SHANBU_TAOSHI:
                        shanbuTaoshiGUID = creature->GetGUID();
                        shanbuAssaultGUIDs.push_back(creature->GetGUID());
                        creature->SetVisible(false);
                        creature->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_PACIFIED | UNIT_FLAG_NON_ATTACKABLE);
                        break;
                    case NPC_YALIAS_SAGEWHISPER:
                        yaliaGUID = creature->GetGUID();
                        shanbuAssaultGUIDs.push_back(creature->GetGUID());
                        creature->SetVisible(false);
                        creature->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_PACIFIED | UNIT_FLAG_NON_ATTACKABLE);
                        break;
                    case NPC_PALACE_GATEKEEPER:
                        palaceGuardGUIDs.push_back(creature->GetGUID());
                        break;
                    case NPC_NALAK:
                        nalakGUID = creature->GetGUID();
                        break;
                    case NPC_WRATHION:
                        wrathionGUID = creature->GetGUID();
                        break;
                    case NPC_THUNDER_FORGE:
                        thunderForgeGUID = creature->GetGUID();
                        break;
                    case NPC_SHADO_PAN_DEFENDER:
                        shadopanDefenderGUID = creature->GetGUID();
                        break;
                    case NPC_THUNDER_FORGE_2:
                        innerForgeGUID = creature->GetGUID();
                        break;
                    case NPC_CELESTIAL_BLACKSMITH:
                        blacksmithGUID = creature->GetGUID();
                        break;
                    case NPC_CELESTIAL_DEFENDER:
                        celestialDefenderGUID = creature->GetGUID();
                        break;
                    case NPC_TAOSHI_CAVE:
                        creature->AddUnitState(UNIT_STATE_IGNORE_PATHFINDING);
                        break;
                }
            }

            void OnGameObjectCreate(GameObject* go) OVERRIDE
            {
                switch (go->GetEntry())
                {
                    case GO_SUNREAVER_PORTAL:
                    case GO_SUNREAVER_LOW_PORTAL:
                    case GO_PORTAL_TO_ISLE_OF_THUNDER:
                        portalGUIDs.push_back(go->GetGUID());
                        break;
                    case GO_TEAR_DOWN_DESTRUCT_WALL:
                        destructableWallGUID = go->GetGUID();
                        go->SetDestructibleState(GO_DESTRUCTIBLE_INTACT);
                        break;
                    case GO_CAVEIN:
                        caveinGUID = go->GetGUID();
                        break;
                    case GO_MOGU_DOOR:
                        if (go->GetPositionX() < 7300.0f)
                            moguDoorGUID = go->GetGUID();
                        break;
                    case GO_HUSENG_COLLISION:
                        if (go->GetDBTableGUIDLow() == 537499) // so much, couldn`t make filter anyway
                            husengCollisionGUID = go->GetGUID();
                        break;
                    case GO_MOGU_GATES_STORMSEA:
                        stormseaMoguDoorGUID = go->GetGUID();
                        break;
                    case GO_SPIRIT_WALL:
                        if (go->GetDBTableGUIDLow() == 537485) // same
                            hordeSpiritWallGUID = go->GetGUID();
                        else if (go->GetDBTableGUIDLow() == 537524)
                            centerSpiritWallGUID = go->GetGUID();
                        break;
                    case GO_MOGU_GATES_CENTER:
                        shanbuCenterGatesGUID = go->GetGUID();
                        break;
                    case GO_MOGU_GATES_HORDE:
                        shanbuHordeGatesGUID = go->GetGUID();
                        break;
                    case GO_MOGU_CRUCIBLE:
                        moguCrucibleGUID = go->GetGUID();
                        break;
                    case GO_THUNDER_FORGE_DOOR:
                        thunderForgeDoorGUID = go->GetGUID();
                        break;
                }
            }

            void OnUnitDeath(Unit* unit) OVERRIDE
            {
                if (unit && unit->ToCreature() && (unit->ToCreature()->GetEntry() == NPC_SHANZE_ELECTROCUTIONER || unit->ToCreature()->GetEntry() == NPC_SHANZE_SLAVER))
                    if (Creature* additTaoshi = unit->FindNearestCreature(NPC_TAOSHI_CAVE, 200.0f, true))
                        additTaoshi->AI()->Talk(urand(0, 2));
            }

            bool SetBossState(uint32 type, EncounterState state) OVERRIDE
            {
                if (!InstanceScript::SetBossState(type, state))
                    return false;

                return true;
            }

            void SetData(uint32 type, uint32 data) OVERRIDE
            {
                switch (type)
                {
                    case STEP_MEET_WITH_RANGER:
                        if (data == IN_PROGRESS)
                        {
                            for (auto&& itr : GetData64(FACTION_DATA) ? bloodhawkGUIDs : covenantHyppoGUIDs)
                                if (Creature* hippogryph = instance->GetCreature(itr))
                                    hippogryph->AI()->DoAction(ACTION_START_INTRO);

                            if (Creature* vereesa = instance->GetCreature(GetData64(GetData64(FACTION_DATA) ? NPC_SCOUT_CAPTAIN_ELSIA : NPC_VEREESA_WINDRUNNER)))
                                vereesa->AI()->DoAction(ACTION_START_INTRO);
                        }
                        else // on finish
                        {
                            for (auto&& itr : GetData64(FACTION_DATA) ? bloodhawkGUIDs : covenantHyppoGUIDs)
                                if (Creature* hippogryph = instance->GetCreature(itr))
                                    hippogryph->DisappearAndDie();

                            for (auto&& itr : instance->GetPlayers())
                                if (Player* player = itr.GetSource())
                                    sScenarioMgr->SendScenarioState(player, 1126, 1, 0, GetData64(FACTION_DATA) ? CRITERIA_TREE_ID_ZEBTULA_2 : CRITERIA_TREE_ID_SHAOLMARA_2, GetData64(FACTION_DATA) ? SCENARIO_ID_ZEBTULA : SCENARIO_ID_SHAOLMARA);
                        }
                    
                        step1 = data;
                        break;
                    case STEP_SURVIVE_THE_AMBUSH:
                        if (data == IN_PROGRESS)
                        {
                            if (Creature* vereesa = instance->GetCreature(GetData64(GetData64(FACTION_DATA) ? NPC_SCOUT_CAPTAIN_ELSIA : NPC_VEREESA_WINDRUNNER)))
                            {
                                for (auto&& itr : GetData64(FACTION_DATA) ? hordeAmbusherSpawnPos : ambusherSpawnPos)
                                    vereesa->SummonCreature(NPC_SHAOL_MARA_BEASTCALLER, itr, TempSummonType::TEMPSUMMON_MANUAL_DESPAWN);

                                // Summon Mara`kah or Tulachek
                                vereesa->SummonCreature(GetData64(FACTION_DATA) ? NPC_ARCANITAL_TULACHEK : NPC_ARCANITAL_MARAKAH, GetData64(FACTION_DATA) ? hordeAmbusherSpawnPos[0] : ambusherSpawnPos[0], TempSummonType::TEMPSUMMON_MANUAL_DESPAWN);
                            }
                        }
                        else if (data == DONE)
                        {
                            for (auto&& itr : instance->GetPlayers())
                                if (Player* player = itr.GetSource())
                                    sScenarioMgr->SendScenarioState(player, 1126, 2, 0, GetData64(FACTION_DATA) ? CRITERIA_TREE_ID_ZEBTULA_3 : CRITERIA_TREE_ID_SHAOLMARA_3, GetData64(FACTION_DATA) ? SCENARIO_ID_ZEBTULA : SCENARIO_ID_SHAOLMARA);
                        }

                        step2 = data;
                        break;
                    case STEP_DESTROY_THE_BARRICADES:
                        if (data == IN_PROGRESS)
                        {
                            for (auto&& itr : barricadeGUIDs)
                                if (Creature* barricadeDefender = instance->GetCreature(itr))
                                    barricadeDefender->AI()->DoAction(ACTION_BARRICADE_ASSAULT);
                        }
                        else // on finish
                        {
                            if (Creature* vereesa = instance->GetCreature(GetData64(GetData64(FACTION_DATA) ? NPC_SCOUT_CAPTAIN_ELSIA : NPC_VEREESA_WINDRUNNER)))
                                vereesa->AI()->DoAction(ACTION_BEHIND_ANKI);

                            for (auto&& itr : instance->GetPlayers())
                                if (Player* player = itr.GetSource())
                                    sScenarioMgr->SendScenarioState(player, 1126, 3, 0, GetData64(FACTION_DATA) ? CRITERIA_TREE_ID_ZEBTULA_4 : CRITERIA_TREE_ID_SHAOLMARA_4, GetData64(FACTION_DATA) ? SCENARIO_ID_ZEBTULA : SCENARIO_ID_SHAOLMARA);
                        }

                        step3 = data;
                        break;
                    case STEP_ANKI_AND_PAKUTESH:
                        step4 = data;

                        for (auto&& itr : instance->GetPlayers())
                            if (Player* player = itr.GetSource())
                                sScenarioMgr->SendScenarioState(player, 1126, 4, 0, GetData64(FACTION_DATA) ? CRITERIA_TREE_ID_ZEBTULA_5 : CRITERIA_TREE_ID_SHAOLMARA_5, GetData64(FACTION_DATA) ? SCENARIO_ID_ZEBTULA : SCENARIO_ID_SHAOLMARA);
                        break;
                    case STEP_KILL_ARCANITAL:
                        step5 = data;

                        if (data == DONE && GetData64(FACTION_DATA))
                        {
                            for (auto&& itr : seekerGUIDs)
                            {
                                if (Creature* seeker = instance->GetCreature(itr))
                                {
                                    seeker->SetVisible(true);
                                    seeker->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_PACIFIED);
                                }
                            }

                            for (auto&& itr : portalGUIDs)
                                DoRespawnGameObject(itr, 7 * DAY);
                        }

                        DoFinishLFGDungeon(GetData64(FACTION_DATA) ? 604 : 617);
                        for (auto&& itr : instance->GetPlayers())
                            if (Player* player = itr.GetSource())
                            {
                                player->SetJoinPos(ExitPos[player->GetTeam() == HORDE ? 0 : 1]);
                                player->KilledMonsterCredit(player->GetTeam() == HORDE ? 69775 : 70345);
                            }
                        break;
                    case STEP_FIND_MISSED_EXPLOSIVE:
                        if (step1 == data)
                            break;

                        step1 = data;

                        if (data == DONE)
                        {
                            for (auto&& itr : instance->GetPlayers())
                                if (Player* player = itr.GetSource())
                                    sScenarioMgr->SendScenarioState(player, 1126, 1, 0, GetData64(FACTION_DATA) ? CRITERIA_TREE_ID_TEAR_DOWN_H_2 : CRITERIA_TREE_ID_TEAR_DOWN_A_2, GetData64(FACTION_DATA) ? SCENARIO_ID_TEAR_DOWN_HORDE : SCENARIO_ID_TEAR_DOWN_ALLIANCE);

                            for (auto&& itr : teardownAssaultGUIDs)
                            {
                                if (Creature* assaultUnit = instance->GetCreature(itr))
                                {
                                    assaultUnit->SetVisible(true);
                                    assaultUnit->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_PACIFIED);
                                }
                            }

                            instance->SummonCreature(GetData64(FACTION_DATA) ? NPC_TEARDOWN_SCOUT_CAPTAIN_ELSIA : NPC_TEARDOWN_VEREESA_WINDRUNNER, elsiaSummPos);
                        }
                        break;
                    case STEP_SEND_EXPLOSIVE:
                        for (auto&& itr : instance->GetPlayers())
                            if (Player* player = itr.GetSource())
                                sScenarioMgr->SendScenarioState(player, 1126, 2, 0, GetData64(FACTION_DATA) ? CRITERIA_TREE_ID_TEAR_DOWN_H_3 : CRITERIA_TREE_ID_TEAR_DOWN_A_3, GetData64(FACTION_DATA) ? SCENARIO_ID_TEAR_DOWN_HORDE : SCENARIO_ID_TEAR_DOWN_ALLIANCE);

                        step2 = data;
                        break;
                    case STEP_EXPLOSIVE:
                        if (data == DONE)
                        {
                            for (auto&& itr : teardownAssaultEnemyGUIDs)
                                if (Creature* zandalari = instance->GetCreature(itr))
                                    zandalari->DespawnOrUnsummon();

                            for (auto&& itr : instance->GetPlayers())
                                if (Player* player = itr.GetSource())
                                    sScenarioMgr->SendScenarioState(player, 1126, 3, 0, GetData64(FACTION_DATA) ? CRITERIA_TREE_ID_TEAR_DOWN_H_4 : CRITERIA_TREE_ID_TEAR_DOWN_A_4, GetData64(FACTION_DATA) ? SCENARIO_ID_TEAR_DOWN_HORDE : SCENARIO_ID_TEAR_DOWN_ALLIANCE);
                        }

                        step3 = data;
                        break;
                    case STEP_KILL_WARGOD_ALCHUKLA:
                        step4 = data;

                        if (data == IN_PROGRESS)
                        {
                            for (auto&& itr : teardownAssaultGUIDs)
                                if (Creature* pUnit = instance->GetCreature(itr))
                                    pUnit->GetMotionMaster()->MovePoint(0, assaultBreakPos.GetPositionX() + frand(-15.0f, 15.0f), assaultBreakPos.GetPositionY() + frand(-10.0f, 10.0f), assaultBreakPos.GetPositionZ());

                            if (Creature* lorthemar = instance->GetCreature(GetData64(NPC_TEARDOWN_LORTHEMAR_THERONE)))
                                lorthemar->AI()->Talk(1);

                            if (Creature* elsia = instance->GetCreature(GetData64(NPC_TEARDOWN_SCOUT_CAPTAIN_ELSIA + 1)))
                                elsia->AI()->DoAction(ACTION_ALCHUKLA_FIND);
                        }
                        else if (data == DONE)
                        {
                            /*for (auto&& itr : portalGUIDs)
                                DoRespawnGameObject(itr, 7 * DAY);*/

                            instance->SummonCreature(GetData64(FACTION_DATA) ? NPC_TEARDOWN_LORTHEMAR_THERONE : NPC_TEARDOWN_JAINA_PROUDMOORE, elsiaAlchuklaPath[5]);
                            DoFinishLFGDungeon(GetData64(FACTION_DATA) ? 608 : 602);
                        }
                        break;
                    case STEP_IN_SKY:
                        step1 = data;

                        for (auto&& itr : instance->GetPlayers())
                            if (Player* player = itr.GetSource())
                                sScenarioMgr->SendScenarioState(player, 1126, 1, 0, CRITERIA_TREE_ID_iN_THE_SKY_2, SCENARIO_ID_IN_THE_SKY);
                        break;
                    case STEP_INTERCEPTION:
                        if (++pterrorCount >= 8 && step2 != DONE)
                        {
                            step2 = DONE;

                            if (Creature* thunderwing = instance->GetCreature(GetData64(NPC_THUNDERWING)))
                                thunderwing->AI()->DoAction(ACTION_FLY_FALL);

                            SaveToDB();

                            for (auto&& itr : instance->GetPlayers())
                                if (Player* player = itr.GetSource())
                                    sScenarioMgr->SendScenarioState(player, 1126, 2, 0, CRITERIA_TREE_ID_iN_THE_SKY_3, SCENARIO_ID_IN_THE_SKY);
                        }
                        break;
                    case STEP_CURB_BEASTMASTER:
                        step3 = data;
                        instance->SummonCreature(NPC_TAOSHI, taoshiSummPos);

                        for (auto&& itr : instance->GetPlayers())
                            if (Player* player = itr.GetSource())
                                sScenarioMgr->SendScenarioState(player, 1126, 3, 0, CRITERIA_TREE_ID_iN_THE_SKY_4, SCENARIO_ID_IN_THE_SKY);
                        break;
                    case STEP_IN_THE_MINE:
                        step4 = data;

                        for (auto&& itr : instance->GetPlayers())
                            if (Player* player = itr.GetSource())
                                sScenarioMgr->SendScenarioState(player, 1126, 4, 0, CRITERIA_TREE_ID_iN_THE_SKY_5, SCENARIO_ID_IN_THE_SKY);
                        break;
                    case STEP_WAS_YOURS_BECOME_MINE:
                        step5 = data;
                        instance->SummonCreature(NPC_TAOSHI, taoshiOpenDoorPath[0]);

                        for (auto&& itr : instance->GetPlayers())
                            if (Player* player = itr.GetSource())
                                sScenarioMgr->SendScenarioState(player, 1126, 5, 0, CRITERIA_TREE_ID_iN_THE_SKY_6, SCENARIO_ID_IN_THE_SKY);
                        break;
                    case STEP_COVER_ME:
                        step6 = data;

                        if (Creature* taoshi = instance->GetCreature(GetData64(NPC_TAOSHI)))
                            taoshi->AI()->Talk(5);

                        if (GameObject* door = instance->GetGameObject(GetData64(GO_MOGU_DOOR)))
                            door->UseDoorOrButton();

                        for (auto&& itr : forgeAssaultGUIDs)
                        {
                            if (Creature* forgeAssaulter = instance->GetCreature(itr))
                            {
                                forgeAssaulter->SetVisible(true);

                                if (forgeAssaulter->GetEntry() == NPC_LORTHEMAR_THRON_IN_SKY || forgeAssaulter->GetEntry() == NPC_LADY_JAINA_IN_SKY)
                                    forgeAssaulter->AI()->DoAction(ACTION_START_INTRO);
                                else
                                    forgeAssaulter->GetMotionMaster()->MovePoint(0, forgeAssaultEntrancePos.GetPositionX() + frand(-8.0f, 8.0f), forgeAssaultEntrancePos.GetPositionY() + frand(-5.0f, 5.0f), forgeAssaultEntrancePos.GetPositionZ());

                            }
                        }

                        DoFinishLFGDungeon(GetData64(FACTION_DATA) ? 606 : 618);
                        break;
                    case STEP_BLENDING_IN:
                        step1 = data;

                        for (auto&& itr : instance->GetPlayers())
                            if (Player* player = itr.GetSource())
                                sScenarioMgr->SendScenarioState(player, 1126, 1, 0, CRITERIA_TREE_ID_STORMSEA_LANDING_2, SCENARIO_ID_STORMSEA_LANDING);
                        break;
                    case STEP_PATH_OF_SMOKE:
                        if (++deactivatedSentryCount >= 10 && GetData(STEP_PATH_OF_SMOKE) != DONE)
                        {
                            step2 = DONE;

                            if (Creature* thunderwing = instance->GetCreature(GetData64(NPC_THUNDERWING_STORMSEA)))
                                thunderwing->AI()->DoAction(ACTION_START_INTRO);

                            if (Creature* shanbu = instance->GetCreature(GetData64(NPC_SHAN_BU_STORMSEA)))
                            {
                                shanbu->SetVisible(false);
                                shanbu->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_PACIFIED);
                            }

                            for (auto&& itr : instance->GetPlayers())
                                if (Player* player = itr.GetSource())
                                    sScenarioMgr->SendScenarioState(player, 1126, 2, 0, CRITERIA_TREE_ID_STORMSEA_LANDING_3, SCENARIO_ID_STORMSEA_LANDING);
                        }
                        break;
                    case STEP_SHADOWS_IN_THE_STORM:
                        step3 = data;

                        for (auto&& itr : instance->GetPlayers())
                            if (Player* player = itr.GetSource())
                                sScenarioMgr->SendScenarioState(player, 1126, 3, 0, CRITERIA_TREE_ID_STORMSEA_LANDING_4, SCENARIO_ID_STORMSEA_LANDING);
                        break;
                    case STEP_WETWORK:
                        step4 = data;

                        if (Creature* taoshi = instance->GetCreature(GetData64(NPC_TAOSHI_STORMSEA_SHIP)))
                            taoshi->AI()->DoAction(ACTION_LEAVE_SHIP);

                        HandleGameObject(GetData64(GO_HUSENG_COLLISION), true, NULL);

                        for (auto&& itr : instance->GetPlayers())
                            if (Player* player = itr.GetSource())
                                sScenarioMgr->SendScenarioState(player, 1126, 4, 0, CRITERIA_TREE_ID_STORMSEA_LANDING_5, SCENARIO_ID_STORMSEA_LANDING);
                        break;
                    case STEP_KEEP_QUIET:
                        step5 = data;

                        for (auto&& itr : instance->GetPlayers())
                            if (Player* player = itr.GetSource())
                                sScenarioMgr->SendScenarioState(player, 1126, 5, 0, CRITERIA_TREE_ID_STORMSEA_LANDING_6, SCENARIO_ID_STORMSEA_LANDING);
                        break;
                    case STEP_LIGHT_UP_THE_NIGHT:
                        for (auto&& itr : instance->GetPlayers())
                            if (Player* player = itr.GetSource())
                                sScenarioMgr->SendScenarioState(player, 1126, 6, 0, CRITERIA_TREE_ID_STORMSEA_LANDING_7, SCENARIO_ID_STORMSEA_LANDING);

                        step6 = data;
                        DoFinishLFGDungeon(603);
                        break;
                    case STEP_BLOODY_CROSSROAD:
                        if (data < DONE)
                            step1 = data;

                        if (data == IN_PROGRESS)
                        {
                            for (auto&& itr : shanbuAssaultGUIDs)
                                if (Creature* assaulter = instance->GetCreature(itr))
                                    assaulter->AI()->DoAction(ACTION_START_INTRO);

                            // Despawn Temp Creatures
                            for (auto&& itr : tempShanbuCreatures)
                                if (Creature* temp = instance->GetCreature(GetData64(itr)))
                                    temp->DespawnOrUnsummon();

                            // Open Center Door and Start Event
                            HandleGameObject(GetData64(GO_MOGU_GATES_CENTER), true, NULL);
                        }
                        else if (++zandalariCount >= 8 && GetData(STEP_BLOODY_CROSSROAD) != DONE)
                        {
                            step1 = DONE;

                            for (auto&& itr : palaceGuardGUIDs)
                                if (Creature* palaceGuardian = instance->GetCreature(itr))
                                    palaceGuardian->AI()->DoAction(ACTION_START_INTRO);

                            if (Creature* taranZhu = instance->GetCreature(GetData64(NPC_SHANBU_TARAN_ZHU)))
                                taranZhu->AI()->Talk(1);

                            for (auto&& itr : instance->GetPlayers())
                                if (Player* player = itr.GetSource())
                                    sScenarioMgr->SendScenarioState(player, 1126, 1, 0, CRITERIA_TREE_ID_FALL_OF_SHANBU_2, SCENARIO_ID_FALL_OF_SHANBU);
                        }
                        break;
                    case STEP_KILL_THE_GUARDS:
                        if (++palaceGuardsCount >= 2 && GetData(STEP_KILL_THE_GUARDS) != DONE)
                        {
                            step2 = DONE;

                            if (Creature* taranZhu = instance->GetCreature(GetData64(NPC_SHANBU_TARAN_ZHU)))
                                taranZhu->AI()->DoAction(ACTION_KEEP_TO_SHANBU);

                            for (auto&& itr : instance->GetPlayers())
                                if (Player* player = itr.GetSource())
                                    sScenarioMgr->SendScenarioState(player, 1126, 2, 0, CRITERIA_TREE_ID_FALL_OF_SHANBU_3, SCENARIO_ID_FALL_OF_SHANBU);
                        }
                        break;
                    case STEP_TO_LEI_SHEN_PALACE:
                        if (++shanbuGuardsCount >= 4 && GetData(STEP_TO_LEI_SHEN_PALACE) != DONE)
                        {
                            step3 = DONE;

                            if (Creature* shanbu = instance->GetCreature(GetData64(NPC_SHAN_BU)))
                                shanbu->AI()->DoAction(ACTION_ATTACK_SHANBU);

                            for (auto&& itr : instance->GetPlayers())
                                if (Player* player = itr.GetSource())
                                    sScenarioMgr->SendScenarioState(player, 1126, 3, 0, CRITERIA_TREE_ID_FALL_OF_SHANBU_4, SCENARIO_ID_FALL_OF_SHANBU);
                        }
                        break;
                    case STEP_DEFEAT_SHAN_BU:
                        step4 = data;
                        DoFinishLFGDungeon(GetData64(FACTION_DATA) ? 621 : 607);

                        if (Creature* taranZhu = instance->GetCreature(GetData64(NPC_SHANBU_TARAN_ZHU)))
                            taranZhu->AI()->DoAction(ACTION_SHANBU_DEFEATED);
                        break;
                    case STEP_TALK_WITH_WRATHION:
                        step1 = data;

                        for (auto&& itr : instance->GetPlayers())
                            if (Player* player = itr.GetSource())
                                sScenarioMgr->SendScenarioState(player, 1126, 1, 0, CRITERIA_TREE_ID_THUNDER_FORGE_2, SCENARIO_ID_THUNDER_FORGE);
                        break;
                    case STEP_FORGED_FROM_TRILLIUM:
                        if (step2 == data)
                            break;

                        step2 = data;

                        if (Creature* wrathion = instance->GetCreature(GetData64(NPC_WRATHION)))
                            wrathion->AI()->DoAction(ACTION_FORGE_INFLUSION);

                        for (auto&& itr : instance->GetPlayers())
                            if (Player* player = itr.GetSource())
                                sScenarioMgr->SendScenarioState(player, 1126, 2, 0, CRITERIA_TREE_ID_THUNDER_FORGE_3, SCENARIO_ID_THUNDER_FORGE);
                        break;
                    case STEP_CHARGING:
                        step3 = data;
                        if (step3 == SPECIAL)
                        {
                            // Summon Forgemaster
                            instance->SummonCreature(NPC_FORGEMASTER_VULKON, forgeMasterSummonPos);
                        }
                        else if (step3 == DONE)
                        {
                            // Move to inner of forge
                            if (Creature* wrathion = instance->GetCreature(GetData64(NPC_WRATHION)))
                                wrathion->AI()->DoAction(ACTION_MOVE_INNER_FORGE);

                            for (auto&& itr : instance->GetPlayers())
                                if (Player* player = itr.GetSource())
                                    sScenarioMgr->SendScenarioState(player, 1126, 3, 0, CRITERIA_TREE_ID_THUNDER_FORGE_4, SCENARIO_ID_THUNDER_FORGE);
                        }
                        break;
                    case STEP_IN_FORGE:
                        step4 = data;

                        if (Creature* wrathion = instance->GetCreature(GetData64(NPC_WRATHION)))
                            wrathion->AI()->DoAction(ACTION_INNER_FORGE);

                        for (auto&& itr : instance->GetPlayers())
                            if (Player* player = itr.GetSource())
                                sScenarioMgr->SendScenarioState(player, 1126, 4, 0, CRITERIA_TREE_ID_THUNDER_FORGE_5, SCENARIO_ID_THUNDER_FORGE);
                        break;
                    case STEP_FILTH_FOUND:
                        step5 = data;

                        if (Creature* wrathion = instance->GetCreature(GetData64(NPC_WRATHION)))
                            wrathion->AI()->DoAction(ACTION_ALMAGMATION_INCOMING);

                        for (auto&& itr : instance->GetPlayers())
                            if (Player* player = itr.GetSource())
                                sScenarioMgr->SendScenarioState(player, 1126, 5, 0, CRITERIA_TREE_ID_THUNDER_FORGE_6, SCENARIO_ID_THUNDER_FORGE);
                        break;
                    case STEP_MANIFESTATION_OF_FILTH:
                        step6 = data;
                        DoFinishLFGDungeon(615);

                        if (Creature* wrathion = instance->GetCreature(GetData64(NPC_WRATHION)))
                            wrathion->AI()->DoAction(ACTION_SHA_DEFEATED);

                        if (Creature* defender = instance->GetCreature(GetData64(NPC_CELESTIAL_DEFENDER)))
                            defender->DespawnOrUnsummon();

                        if (Creature* blacksmith = instance->GetCreature(GetData64(NPC_CELESTIAL_BLACKSMITH)))
                            blacksmith->DespawnOrUnsummon();
                        break;
                }

                if (data == DONE || data == SPECIAL)
                    SaveToDB();
            }

            uint32 GetData(uint32 type) const OVERRIDE
            {
                switch (type)
                {
                    case STEP_MEET_WITH_RANGER:
                    case STEP_FIND_MISSED_EXPLOSIVE:
                    case STEP_IN_SKY:
                    case STEP_BLENDING_IN:
                    case STEP_BLOODY_CROSSROAD:
                    case STEP_TALK_WITH_WRATHION:
                        return step1;
                    case STEP_SURVIVE_THE_AMBUSH:
                    case STEP_SEND_EXPLOSIVE:
                    case STEP_INTERCEPTION:
                    case STEP_PATH_OF_SMOKE:
                    case STEP_KILL_THE_GUARDS:
                    case STEP_FORGED_FROM_TRILLIUM:
                        return step2;
                    case STEP_DESTROY_THE_BARRICADES:
                    case STEP_EXPLOSIVE:
                    case STEP_CURB_BEASTMASTER:
                    case STEP_SHADOWS_IN_THE_STORM:
                    case STEP_TO_LEI_SHEN_PALACE:
                    case STEP_CHARGING:
                        return step3;
                    case STEP_ANKI_AND_PAKUTESH:
                    case STEP_KILL_WARGOD_ALCHUKLA:
                    case STEP_IN_THE_MINE:
                    case STEP_WETWORK:
                    case STEP_DEFEAT_SHAN_BU:
                    case STEP_IN_FORGE:
                        return step4;
                    case STEP_KILL_ARCANITAL:
                    case STEP_WAS_YOURS_BECOME_MINE:
                    case STEP_KEEP_QUIET:
                    case STEP_FILTH_FOUND:
                        return step5;
                    case STEP_COVER_ME:
                    case STEP_LIGHT_UP_THE_NIGHT:
                    case STEP_MANIFESTATION_OF_FILTH:
                        return step6;
                    case ROLE_DATA:
                        return roleId == ROLES_HEALER ? 1 : 0;
                }

                return 0;
            }

            uint64 GetData64(uint32 type) const OVERRIDE
            {
                switch (type)
                {
                    case NPC_VEREESA_WINDRUNNER:
                        return vereesaGUID;
                    case PLAYER_DATA:
                        return playerGUID;
                    case NPC_ARCANITAL_MARAKAH:
                        return marakahGUID;
                    case NPC_SCOUT_CAPTAIN_ELSIA:
                        return elsiaGUID;
                    case FACTION_DATA:
                        return teamInstance == HORDE ? uint64(uint64(1)) : 0;
                    case NPC_ARCANITAL_TULACHEK:
                        return tulachekGUID;
                    case GO_TEAR_DOWN_DESTRUCT_WALL:
                        return destructableWallGUID;
                    case NPC_TEARDOWN_SCOUT_CAPTAIN_ELSIA:
                        return teardownElsiaGUID;
                    case NPC_WARGOD_ALCHUKLA:
                        return alchuklaGUID;
                    case NPC_WARGOD_ALCHUKLA + 1:
                        return additClhucklaGUID;
                    case NPC_TEARDOWN_LORTHEMAR_THERONE:
                        return teardownLorthemarGUID;
                    case NPC_TEARDOWN_SCOUT_CAPTAIN_ELSIA + 1:
                        return additTeardownElsiaGUID;
                    case NPC_THUNDERWING:
                        return thunderwingGUID;
                    case NPC_SCOUT_CAPTAIN_ELSIA_IN_SKY:
                        return elsiaToSkiesGUID;
                    case NPC_BEASTMASTER_HORAKI:
                        return horrakiGUID;
                    case GO_CAVEIN:
                        return caveinGUID;
                    case GO_MOGU_DOOR:
                        return moguDoorGUID;
                    case NPC_TAOSHI:
                        return taoshiGUID;
                    case GO_HUSENG_COLLISION:
                        return husengCollisionGUID;
                    case NPC_THUNDERWING_STORMSEA:
                        return thunderwingStormseaGUID;
                    case NPC_TAOSHI_STORMSEA:
                        return taoshiStormseaGUID;
                    case NPC_TAOSHI_STORMSEA_SHIP:
                        return taoshiStormseaShipGUID;
                    case NPC_CAPTAIN_HALUKAL:
                        return captainHalukalGUID;
                    case NPC_HUSENG_THE_GATEKEEPER:
                        return husengGUID;
                    case NPC_SHAN_BU_STORMSEA:
                        return shanbuStormseaGUID;
                    case GO_MOGU_GATES_STORMSEA:
                        return stormseaMoguDoorGUID;
                    case GO_SPIRIT_WALL:
                        return centerSpiritWallGUID;
                    case GO_SPIRIT_WALL + 1:
                        return hordeSpiritWallGUID;
                    case NPC_SHANBU_ELSIA:
                        return shanbuElsiaGUID;
                    case NPC_SHANBU_VEREESA:
                        return shanbuVereesaGUID;
                    case NPC_SHANBU_JAINA:
                        return shanbuJainaGUID;
                    case NPC_SHANBU_LORTHEMAR:
                        return shanbuLorthemarGUID;
                    case GO_MOGU_GATES_CENTER:
                        return shanbuCenterGatesGUID;
                    case GO_MOGU_GATES_HORDE:
                        return shanbuHordeGatesGUID;
                    case NPC_SHAN_BU:
                        return shanbuGUID;
                    case NPC_SHANBU_TARAN_ZHU:
                        return taranZhuGUID;
                    case NPC_SHANBU_TAOSHI:
                        return shanbuTaoshiGUID;
                    case NPC_YALIAS_SAGEWHISPER:
                        return yaliaGUID;
                    case NPC_SHANBU_JAINA + 5000:
                        return additShanbuJainaGUID;
                    case NPC_SHANBU_LORTHEMAR + 5000:
                        return additShanbuLorthemarGUID;
                    case NPC_SHANBU_VEREESA + 5000:
                        return additShanbuVereesaGUID;
                    case NPC_NALAK:
                        return nalakGUID;
                    case PHASE_DATA:
                        return uint64(uint64(phaseId));
                    case GO_MOGU_CRUCIBLE:
                        return moguCrucibleGUID;
                    case NPC_WRATHION:
                        return wrathionGUID;
                    case NPC_THUNDER_FORGE:
                        return thunderForgeGUID;
                    case GO_THUNDER_FORGE_DOOR:
                        return thunderForgeDoorGUID;
                    case NPC_SHADO_PAN_DEFENDER:
                        return shadopanDefenderGUID;
                    case NPC_THUNDER_FORGE_2:
                        return innerForgeGUID;
                    case NPC_CELESTIAL_BLACKSMITH:
                        return blacksmithGUID;
                    case NPC_CELESTIAL_DEFENDER:
                        return celestialDefenderGUID;
                }

                return 0;
            }

            std::string GetSaveData() OVERRIDE
            {
                OUT_SAVE_INST_DATA;

                std::ostringstream saveStream;
                saveStream << "I T " << step1 << ' ' << step2 << ' ' << step3 << ' ' << step4 << ' ' << step5 << ' ' << step6;

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

                if (dataHead1 == 'I' && dataHead2 == 'T')
                {
                    for (uint8 i = 0; i < 3; ++i)
                    {
                        uint32 tmpState;
                        loadStream >> tmpState;
                        if (tmpState == IN_PROGRESS || tmpState > SPECIAL)
                            tmpState = NOT_STARTED;

                        SetBossState(i, EncounterState(tmpState));
                    }
                }
                else OUT_LOAD_INST_DATA_FAIL;

                OUT_LOAD_INST_DATA_COMPLETE;
            }

            void Update(uint32 diff) OVERRIDE
            {
                m_mEvents.Update(diff);
            }
        };

        InstanceScript* GetInstanceScript(InstanceMap* map) const OVERRIDE
        {
            return new instance_isle_of_thunder_InstanceScript(map);
        }
};

void AddSC_instance_isle_of_thunder()
{
    new instance_isle_of_thunder();
}