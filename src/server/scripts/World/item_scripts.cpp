/*
* This file is part of Project SkyFire https://www.projectskyfire.org. 
* See LICENSE.md file for Copyright information
*/

 /* ScriptData
 SDName: Item_Scripts
 SD%Complete: 100
 SDComment: Items for a range of different items. See content below (in script)
 SDCategory: Items
 EndScriptData */

 /* ContentData
 item_nether_wraith_beacon(i31742)   Summons creatures for quest Becoming a Spellfire Tailor (q10832)
 item_flying_machine(i34060, i34061)  Engineering crafted flying machines
 item_gor_dreks_ointment(i30175)     Protecting Our Own(q10488)
 item_only_for_flight                Items which should only useable while flying
 EndContentData */

#include "ScriptMgr.h"
#include "ScriptedCreature.h"
#include "Spell.h"
#include "Player.h"

 /*#####
 # item_only_for_flight
 #####*/

enum OnlyForFlight
{
    SPELL_ARCANE_CHARGES = 45072
};

class item_only_for_flight : public ItemScript
{
public:
    item_only_for_flight() : ItemScript("item_only_for_flight") { }

    bool OnUse(Player* player, Item* item, SpellCastTargets const& /*targets*/) OVERRIDE
    {
        uint32 itemId = item->GetEntry();
        bool disabled = false;

        //for special scripts
        switch (itemId)
        {
            case 24538:
                if (player->GetAreaId() != 3628)
                    disabled = true;
                break;
            case 34489:
                if (player->GetZoneId() != 4080)
                    disabled = true;
                break;
            case 34475:
                if (const SpellInfo* spellInfo = sSpellMgr->GetSpellInfo(SPELL_ARCANE_CHARGES))
                    Spell::SendCastResult(player, spellInfo, 1, SpellCastResult::SPELL_FAILED_NOT_ON_GROUND);
                break;
        }

        // allow use in flight only
        if (player->IsInFlight() && !disabled)
            return false;

        // error
        player->SendEquipError(EQUIP_ERR_CLIENT_LOCKED_OUT, item, NULL);
        return true;
    }
};

/*#####
# item_nether_wraith_beacon
#####*/

class item_nether_wraith_beacon : public ItemScript
{
public:
    item_nether_wraith_beacon() : ItemScript("item_nether_wraith_beacon") { }

    bool OnUse(Player* player, Item* /*item*/, SpellCastTargets const& /*targets*/) OVERRIDE
    {
        if (player->GetQuestStatus(10832) == QUEST_STATUS_INCOMPLETE)
        {
            if (Creature* nether = player->SummonCreature(22408, player->GetPositionX(), player->GetPositionY() + 20, player->GetPositionZ(), 0, TempSummonType::TEMPSUMMON_TIMED_DESPAWN, 180000))
                nether->AI()->AttackStart(player);

            if (Creature* nether = player->SummonCreature(22408, player->GetPositionX(), player->GetPositionY() - 20, player->GetPositionZ(), 0, TempSummonType::TEMPSUMMON_TIMED_DESPAWN, 180000))
                nether->AI()->AttackStart(player);
        }
        return false;
    }
};

/*#####
# item_gor_dreks_ointment
#####*/

class item_gor_dreks_ointment : public ItemScript
{
public:
    item_gor_dreks_ointment() : ItemScript("item_gor_dreks_ointment") { }

    bool OnUse(Player* player, Item* item, SpellCastTargets const& targets) OVERRIDE
    {
        if (targets.GetUnitTarget() && targets.GetUnitTarget()->GetTypeId() == TypeID::TYPEID_UNIT &&
            targets.GetUnitTarget()->GetEntry() == 20748 && !targets.GetUnitTarget()->HasAura(32578))
            return false;

        player->SendEquipError(EQUIP_ERR_CLIENT_LOCKED_OUT, item, NULL);
        return true;
    }
};

/*#####
# item_incendiary_explosives
#####*/

class item_incendiary_explosives : public ItemScript
{
public:
    item_incendiary_explosives() : ItemScript("item_incendiary_explosives") { }

    bool OnUse(Player* player, Item* item, SpellCastTargets const & /*targets*/) OVERRIDE
    {
        if (player->FindNearestCreature(26248, 15) || player->FindNearestCreature(26249, 15))
            return false;
        else
        {
            player->SendEquipError(EQUIP_ERR_OUT_OF_RANGE, item, NULL);
            return true;
        }
    }
};

/*#####
# item_mysterious_egg
#####*/

class item_mysterious_egg : public ItemScript
{
public:
    item_mysterious_egg() : ItemScript("item_mysterious_egg") { }

    bool OnExpire(Player* player, ItemTemplate const* /*pItemProto*/) OVERRIDE
    {
        ItemPosCountVec dest;
        uint8 msg = player->CanStoreNewItem(NULL_BAG, NULL_SLOT, dest, 39883, 1); // Cracked Egg
        if (msg == EQUIP_ERR_OK)
            player->StoreNewItem(dest, 39883, true, Item::GenerateItemRandomPropertyId(39883));

        return true;
    }
};

/*#####
# item_disgusting_jar
#####*/

class item_disgusting_jar : public ItemScript
{
public:
    item_disgusting_jar() : ItemScript("item_disgusting_jar") { }

    bool OnExpire(Player* player, ItemTemplate const* /*pItemProto*/) OVERRIDE
    {
        ItemPosCountVec dest;
        uint8 msg = player->CanStoreNewItem(NULL_BAG, NULL_SLOT, dest, 44718, 1); // Ripe Disgusting Jar
        if (msg == EQUIP_ERR_OK)
            player->StoreNewItem(dest, 44718, true, Item::GenerateItemRandomPropertyId(44718));

        return true;
    }
};

/*#####
# item_pile_fake_furs
#####*/

enum PileFakeFur
{
    GO_CARIBOU_TRAP_1 = 187982,
    GO_CARIBOU_TRAP_2 = 187995,
    GO_CARIBOU_TRAP_3 = 187996,
    GO_CARIBOU_TRAP_4 = 187997,
    GO_CARIBOU_TRAP_5 = 187998,
    GO_CARIBOU_TRAP_6 = 187999,
    GO_CARIBOU_TRAP_7 = 188000,
    GO_CARIBOU_TRAP_8 = 188001,
    GO_CARIBOU_TRAP_9 = 188002,
    GO_CARIBOU_TRAP_10 = 188003,
    GO_CARIBOU_TRAP_11 = 188004,
    GO_CARIBOU_TRAP_12 = 188005,
    GO_CARIBOU_TRAP_13 = 188006,
    GO_CARIBOU_TRAP_14 = 188007,
    GO_CARIBOU_TRAP_15 = 188008,
    GO_HIGH_QUALITY_FUR = 187983,
    NPC_NESINGWARY_TRAPPER = 25835
};

#define CaribouTrapsNum 15
const uint32 CaribouTraps[CaribouTrapsNum] =
{
    GO_CARIBOU_TRAP_1, GO_CARIBOU_TRAP_2, GO_CARIBOU_TRAP_3, GO_CARIBOU_TRAP_4, GO_CARIBOU_TRAP_5,
    GO_CARIBOU_TRAP_6, GO_CARIBOU_TRAP_7, GO_CARIBOU_TRAP_8, GO_CARIBOU_TRAP_9, GO_CARIBOU_TRAP_10,
    GO_CARIBOU_TRAP_11, GO_CARIBOU_TRAP_12, GO_CARIBOU_TRAP_13, GO_CARIBOU_TRAP_14, GO_CARIBOU_TRAP_15,
};

class item_pile_fake_furs : public ItemScript
{
public:
    item_pile_fake_furs() : ItemScript("item_pile_fake_furs") { }

    bool OnUse(Player* player, Item* /*item*/, SpellCastTargets const & /*targets*/) OVERRIDE
    {
        GameObject* go = NULL;
        for (uint8 i = 0; i < CaribouTrapsNum; ++i)
        {
            go = player->FindNearestGameObject(CaribouTraps[i], 5.0f);
            if (go)
                break;
        }

        if (!go)
            return false;

        if (go->FindNearestCreature(NPC_NESINGWARY_TRAPPER, 10.0f, true) || go->FindNearestCreature(NPC_NESINGWARY_TRAPPER, 10.0f, false) || go->FindNearestGameObject(GO_HIGH_QUALITY_FUR, 2.0f))
            return true;

        float x, y, z;
        go->GetClosePoint(x, y, z, go->GetObjectSize() / 3, 7.0f);
        go->SummonGameObject(GO_HIGH_QUALITY_FUR, go->GetPositionX(), go->GetPositionY(), go->GetPositionZ(), 0, 0, 0, 0, 0, 1000);
        if (TempSummon* summon = player->SummonCreature(NPC_NESINGWARY_TRAPPER, x, y, z, go->GetOrientation(), TempSummonType::TEMPSUMMON_DEAD_DESPAWN, 1000))
        {
            summon->SetVisible(false);
            summon->SetReactState(REACT_PASSIVE);
            summon->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_IMMUNE_TO_PC);
        }
        return false;
    }
};

/*#####
# item_petrov_cluster_bombs
#####*/

enum PetrovClusterBombs
{
    SPELL_PETROV_BOMB = 42406,
    AREA_ID_SHATTERED_STRAITS = 4064,
    ZONE_ID_HOWLING = 495
};

class item_petrov_cluster_bombs : public ItemScript
{
public:
    item_petrov_cluster_bombs() : ItemScript("item_petrov_cluster_bombs") { }

    bool OnUse(Player* player, Item* /*item*/, const SpellCastTargets & /*targets*/) OVERRIDE
    {
        if (player->GetZoneId() != ZONE_ID_HOWLING)
            return false;

        if (!player->GetTransport() || player->GetAreaId() != AREA_ID_SHATTERED_STRAITS)
        {
            if (const SpellInfo* spellInfo = sSpellMgr->GetSpellInfo(SPELL_PETROV_BOMB))
                Spell::SendCastResult(player, spellInfo, 1, SpellCastResult::SPELL_FAILED_NOT_HERE);

            return true;
        }

        return false;
    }
};

/*######
# item_dehta_trap_smasher
# For quest 11876, Help Those That Cannot Help Themselves
######*/
enum HelpThemselves
{
    QUEST_CANNOT_HELP_THEMSELVES = 11876,
    NPC_TRAPPED_MAMMOTH_CALF = 25850,
    GO_MAMMOTH_TRAP_1 = 188022,
    GO_MAMMOTH_TRAP_2 = 188024,
    GO_MAMMOTH_TRAP_3 = 188025,
    GO_MAMMOTH_TRAP_4 = 188026,
    GO_MAMMOTH_TRAP_5 = 188027,
    GO_MAMMOTH_TRAP_6 = 188028,
    GO_MAMMOTH_TRAP_7 = 188029,
    GO_MAMMOTH_TRAP_8 = 188030,
    GO_MAMMOTH_TRAP_9 = 188031,
    GO_MAMMOTH_TRAP_10 = 188032,
    GO_MAMMOTH_TRAP_11 = 188033,
    GO_MAMMOTH_TRAP_12 = 188034,
    GO_MAMMOTH_TRAP_13 = 188035,
    GO_MAMMOTH_TRAP_14 = 188036,
    GO_MAMMOTH_TRAP_15 = 188037,
    GO_MAMMOTH_TRAP_16 = 188038,
    GO_MAMMOTH_TRAP_17 = 188039,
    GO_MAMMOTH_TRAP_18 = 188040,
    GO_MAMMOTH_TRAP_19 = 188041,
    GO_MAMMOTH_TRAP_20 = 188042,
    GO_MAMMOTH_TRAP_21 = 188043,
    GO_MAMMOTH_TRAP_22 = 188044,
};

#define MammothTrapsNum 22
const uint32 MammothTraps[MammothTrapsNum] =
{
    GO_MAMMOTH_TRAP_1, GO_MAMMOTH_TRAP_2, GO_MAMMOTH_TRAP_3, GO_MAMMOTH_TRAP_4, GO_MAMMOTH_TRAP_5,
    GO_MAMMOTH_TRAP_6, GO_MAMMOTH_TRAP_7, GO_MAMMOTH_TRAP_8, GO_MAMMOTH_TRAP_9, GO_MAMMOTH_TRAP_10,
    GO_MAMMOTH_TRAP_11, GO_MAMMOTH_TRAP_12, GO_MAMMOTH_TRAP_13, GO_MAMMOTH_TRAP_14, GO_MAMMOTH_TRAP_15,
    GO_MAMMOTH_TRAP_16, GO_MAMMOTH_TRAP_17, GO_MAMMOTH_TRAP_18, GO_MAMMOTH_TRAP_19, GO_MAMMOTH_TRAP_20,
    GO_MAMMOTH_TRAP_21, GO_MAMMOTH_TRAP_22
};

class item_dehta_trap_smasher : public ItemScript
{
public:
    item_dehta_trap_smasher() : ItemScript("item_dehta_trap_smasher") { }

    bool OnUse(Player* player, Item* /*item*/, const SpellCastTargets & /*targets*/) OVERRIDE
    {
        if (player->GetQuestStatus(QUEST_CANNOT_HELP_THEMSELVES) != QUEST_STATUS_INCOMPLETE)
            return false;

        Creature* pMammoth = player->FindNearestCreature(NPC_TRAPPED_MAMMOTH_CALF, 5.0f);
        if (!pMammoth)
            return false;

        GameObject* pTrap = NULL;
        for (uint8 i = 0; i < MammothTrapsNum; ++i)
        {
            pTrap = player->FindNearestGameObject(MammothTraps[i], 11.0f);
            if (pTrap)
            {
                pMammoth->AI()->DoAction(1);
                pTrap->SetGoState(GOState::GO_STATE_READY);
                player->KilledMonsterCredit(NPC_TRAPPED_MAMMOTH_CALF, 0);
                return true;
            }
        }
        return false;
    }
};

enum TheEmissary
{
    QUEST_THE_EMISSARY = 11626,
    NPC_LEVIROTH = 26452
};

class item_trident_of_nazjan : public ItemScript
{
public:
    item_trident_of_nazjan() : ItemScript("item_Trident_of_Nazjan") { }

    bool OnUse(Player* player, Item* item, const SpellCastTargets & /*targets*/) OVERRIDE
    {
        if (player->GetQuestStatus(QUEST_THE_EMISSARY) == QUEST_STATUS_INCOMPLETE)
        {
            if (Creature* pLeviroth = player->FindNearestCreature(NPC_LEVIROTH, 10.0f)) // spell range
            {
                pLeviroth->AI()->AttackStart(player);
                return false;
            }
            else
                player->SendEquipError(EQUIP_ERR_OUT_OF_RANGE, item, NULL);
        }
        else
            player->SendEquipError(EQUIP_ERR_CLIENT_LOCKED_OUT, item, NULL);
        return true;
    }
};

enum CapturedFrog
{
    QUEST_THE_PERFECT_SPIES = 25444,
    NPC_VANIRAS_SENTRY_TOTEM = 40187
};

class item_captured_frog : public ItemScript
{
public:
    item_captured_frog() : ItemScript("item_captured_frog") { }

    bool OnUse(Player* player, Item* item, SpellCastTargets const& /*targets*/) OVERRIDE
    {
        if (player->GetQuestStatus(QUEST_THE_PERFECT_SPIES) == QUEST_STATUS_INCOMPLETE)
        {
            if (player->FindNearestCreature(NPC_VANIRAS_SENTRY_TOTEM, 10.0f))
                return false;
            else
                player->SendEquipError(EQUIP_ERR_OUT_OF_RANGE, item, NULL);
        }
        else
            player->SendEquipError(EQUIP_ERR_CLIENT_LOCKED_OUT, item, NULL);
        return true;
    }
};

class item_eye_of_black_prince : public ItemScript
{
public:
    item_eye_of_black_prince() : ItemScript("item_eye_of_black_prince") { }

    bool OnUse(Player* /*player*/, Item* item, SpellCastTargets const& /*targets*/) OVERRIDE {
        switch (item->GetEntry())
        {
            ///// - Sha Touched Weapons - /////
            // Heart of Fear
            // Shek'zeer
            case 86865: // Kri'tak, Imperial Scepter of the Swarm - Raid Finder
            case 86227: // Kri'tak, Imperial Scepter of the Swarm - Normal
            case 86990: // Kri'tak, Imperial Scepter of the Swarm - Heroic
                        // Shek'zeer
            case 86864: // Claws of Shek'zeer - Raid Finder
            case 86226: // Claws of Shek'zeer - Normal
            case 86988: // Claws of Shek'zeer - Heroic

                        // Terrace of Endless Spring
                        // Tsulong
            case 86886: // Loshan, Terror Incarnate - Raid Finder
            case 86328: // Loshan, Terror Incarnate - Normal
            case 87164: // Loshan, Terror Incarnate - Heroic
                        // Tsulong
            case 86879: // Gao-Rei, Staff of the Legendary Protector - Raid Finder
            case 86321: // Gao-Rei, Staff of the Legendary Protector - Normal
            case 87156: // Gao-Rei, Staff of the Legendary Protector - Heroic
                        // Lei Shi
            case 86889: // Taoren, the Soul Burner - Raid Finder
            case 86331: // Taoren, the Soul Burner - Normal
            case 87168: // Taoren, the Soul Burner - Heroic
                        // Lei Shi
            case 86910: // Spiritsever - Raid Finder
            case 86391: // Spiritsever - Normal
            case 87166: // Spiritsever - Heroic
                        // Lei Shi
            case 86893: // Jin'ya, Orb of the Waterspeaker - Raid Finder
            case 86335: // Jin'ya, Orb of the Waterspeaker - Normal
            case 87170: // Jin'ya, Orb of the Waterspeaker - Heroic
                        // Sha of Fear
            case 86906: // Kilrak, Jaws of Terror - Raid Finder
            case 86387: // Kilrak, Jaws of Terror - Normal
            case 87173: // Kilrak, Jaws of Terror - Heroic
                        // Sha of Fear
            case 86905: // Shin'ka, Execution of Dominion - Raid Finder
            case 86386: // Shin'ka, Execution of Dominion - Normal
            case 87176: // Shin'ka, Execution of Dominion - Heroic
                        ///// - Throne of Thunder - /////
                        // Last Stand of the Zandalari
                        // Jin'Rokh
            case 95640: // Soulblade of the Breaking Storm - Raid Finder
            case 94730: // Soulblade of the Breaking Storm - Normal
            case 96012: // Soulblade of the Breaking Storm - Thunderforged
            case 96384: // Soulblade of the Breaking Storm - Heroic
            case 96756: // Soulblade of the Breaking Storm - Heroic Thunderforged
                        // Jin'Rokh
            case 95632: // Worldbreaker's Stormscythe - Raid Finder
            case 94722: // Worldbreaker's Stormscythe - Normal
            case 96004: // Worldbreaker's Stormscythe - Thunderforged
            case 96376: // Worldbreaker's Stormscythe - Heroic
            case 96748: // Worldbreaker's Stormscythe - Heroic Thunderforged
                        // Horridon
            case 95657: // Dinomancer's Spiritbinding Spire - Raid Finder
            case 94749: // Dinomancer's Spiritbinding Spire - Normal
            case 96029: // Dinomancer's Spiritbinding Spire - Thunderforged
            case 96401: // Dinomancer's Spiritbinding Spire - Heroic
            case 96773: // Dinomancer's Spiritbinding Spire - Heroic Thunderforged
                        // Horridon
            case 95647: // Jalak's Maelstrom Staff - Raid Finder
            case 94740: // Jalak's Maelstrom Staff - Normal
            case 96019: // Jalak's Maelstrom Staff - Thunderforged
            case 96391: // Jalak's Maelstrom Staff - Heroic
            case 96763: // Jalak's Maelstrom Staff - Heroic Thunderforged
                        // Horridon
            case 95660: // Venomlord's Totemic Wand - Raid Finder
            case 94755: // Venomlord's Totemic Wand - Normal
            case 96032: // Venomlord's Totemic Wand - Thunderforged
            case 96404: // Venomlord's Totemic Wand - Heroic
            case 96776: // Venomlord's Totemic Wand - Heroic Thunderforged
                        // Council of Elders
            case 95666: // Kura-Kura, Kazra'jin's Skullcleaver - Raid Finder
            case 94759: // Kura-Kura, Kazra'jin's Skullcleaver - Normal
            case 96038: // Kura-Kura, Kazra'jin's Skullcleaver - Thunderforged
            case 96410: // Kura-Kura, Kazra'jin's Skullcleaver - Heroic
            case 96782: // Kura-Kura, Kazra'jin's Skullcleaver - Heroic Thunderforged
                        // Council of Elders
            case 95670: // Amun-Thoth, Sul's Spiritrending Talons - Raid Finder
            case 94760: // Amun-Thoth, Sul's Spiritrending Talons - Normal
            case 96042: // Amun-Thoth, Sul's Spiritrending Talons - Thunderforged
            case 96414: // Amun-Thoth, Sul's Spiritrending Talons - Heroic
            case 96786: // Amun-Thoth, Sul's Spiritrending Talons - Heroic Thunderforged
                        // Council of Elders
            case 95675: // Zerat, Malakk's Soulburning Greatsword - Raid Finder
            case 94758: // Zerat, Malakk's Soulburning Greatsword - Normal
            case 96047: // Zerat, Malakk's Soulburning Greatsword - Thunderforged
            case 96419: // Zerat, Malakk's Soulburning Greatsword - Heroic
            case 96791: // Zerat, Malakk's Soulburning Greatsword - Heroic Thunderforged

                        // Forgotten Depths
                        // Tortos
            case 95686: // Shellsplitter Greataxe - Raid Finder
            case 94768: // Shellsplitter Greataxe - Normal
            case 96058: // Shellsplitter Greataxe - Thunderforged
            case 96430: // Shellsplitter Greataxe - Heroic
            case 96802: // Shellsplitter Greataxe - Heroic Thunderforged
                        // Tortos
            case 95678: // Shattered Tortoiseshell Longbow - Raid Finder
            case 94769: // Shattered Tortoiseshell Longbow - Normal
            case 96050: // Shattered Tortoiseshell Longbow - Thunderforged
            case 96422: // Shattered Tortoiseshell Longbow - Heroic
            case 96794: // Shattered Tortoiseshell Longbow - Heroic Thunderforged
                        // Megaera
            case 95698: // Megaera's Poisoned Fang - Raid Finder
            case 94788: // Megaera's Poisoned Fang - Normal
            case 96070: // Megaera's Poisoned Fang - Thunderforged
            case 96442: // Megaera's Poisoned Fang - Heroic
            case 96814: // Megaera's Poisoned Fang - Heroic Thunderforged
                        // Ji'kun
            case 95720: // Giorgio's Caduceus of Pure Moods - Raid Finder
            case 94805: // Giorgio's Caduceus of Pure Moods - Normal
            case 96092: // Giorgio's Caduceus of Pure Moods - Thunderforged
            case 96464: // Giorgio's Caduceus of Pure Moods - Heroic
            case 96836: // Giorgio's Caduceus of Pure Moods - Heroic Thunderforged

                        // Halls of Flesh-Shapping
                        // Durumu
            case 95743: // Ritual Dagger of the Mind's Eye - Raid Finder
            case 94922: // Ritual Dagger of the Mind's Eye - Normal
            case 96115: // Ritual Dagger of the Mind's Eye - Thunderforged
            case 96487: // Ritual Dagger of the Mind's Eye - Heroic
            case 96859: // Ritual Dagger of the Mind's Eye - Heroic Thunderforged
                        // Durumu
            case 95728: // Durumu's Baleful Gaze - Raid Finder
            case 94814: // Durumu's Baleful Gaze - Normal
            case 96100: // Durumu's Baleful Gaze - Thunderforged
            case 96472: // Durumu's Baleful Gaze - Heroic
            case 96844: // Durumu's Baleful Gaze - Heroic Thunderforged
                        // Primordius
            case 95758: // Acid-Spine Bonemace - Raid Finder
            case 94937: // Acid-Spine Bonemace - Normal
            case 96130: // Acid-Spine Bonemace - Thunderforged
            case 96502: // Acid-Spine Bonemace - Heroic
            case 96874: // Acid-Spine Bonemace - Heroic Thunderforged
                        // Dark Animus
            case 95774: // Athame of the Sanguine Ritual - Raid Finder
            case 94955: // Athame of the Sanguine Ritual - Normal
            case 96146: // Athame of the Sanguine Ritual - Thunderforged
            case 96518: // Athame of the Sanguine Ritual - Heroic
            case 96890: // Athame of the Sanguine Ritual - Heroic Thunderforged
                        // Dark Animus
            case 95770: // Hand of the Dark Animus - Raid Finder
            case 94954: // Hand of the Dark Animus - Normal
            case 96142: // Hand of the Dark Animus - Thunderforged
            case 96514: // Hand of the Dark Animus - Heroic
            case 96886: // Hand of the Dark Animus - Heroic Thunderforged

                        // Pinnacle of Storms
                        // Iron Qon
            case 95780: // Iron Qon's Boot Knife - Raid Finder
            case 94971: // Iron Qon's Boot Knife - Normal
            case 96152: // Iron Qon's Boot Knife - Thunderforged
            case 96524: // Iron Qon's Boot Knife - Heroic
            case 96896: // Iron Qon's Boot Knife - Heroic Thunderforged
                        // Iron Qon
            case 95790: // Qon's Flaming Scimitar - Raid Finder
            case 94964: // Qon's Flaming Scimitar - Normal
            case 96162: // Qon's Flaming Scimitar - Thunderforged
            case 96534: // Qon's Flaming Scimitar - Heroic
            case 96906: // Qon's Flaming Scimitar - Heroic Thunderforged
                        // Iron Qon
            case 95781: // Voice of the Quilen - Raid Finder
            case 94963: // Voice of the Quilen - Normal
            case 96153: // Voice of the Quilen - Thunderforged
            case 96525: // Voice of the Quilen - Heroic
            case 96897: // Voice of the Quilen - Heroic Thunderforged
                        // Twin Consorts
            case 95791: // Wu-Lai, Bladed Fan of the Consorts - Raid Finder
            case 94973: // Wu-Lai, Bladed Fan of the Consorts - Normal
            case 96163: // Wu-Lai, Bladed Fan of the Consorts - Thunderforged
            case 96535: // Wu-Lai, Bladed Fan of the Consorts - Heroic
            case 96907: // Wu-Lai, Bladed Fan of the Consorts - Heroic Thunderforged
                        // Twin Consorts
            case 95795: // Suen-Wo, Spire of the Falling Sun - Raid Finder
            case 94974: // Suen-Wo, Spire of the Falling Sun - Normal
            case 96167: // Suen-Wo, Spire of the Falling Sun - Thunderforged
            case 96539: // Suen-Wo, Spire of the Falling Sun - Heroic
            case 96911: // Suen-Wo, Spire of the Falling Sun - Heroic Thunderforged
                        // Lei Shen
            case 95803: // Shan-Dun, Breaker of Hope - Raid Finder
            case 94983: // Shan-Dun, Breaker of Hope - Normal
            case 96175: // Shan-Dun, Breaker of Hope - Thunderforged
            case 96547: // Shan-Dun, Breaker of Hope - Heroic
            case 96919: // Shan-Dun, Breaker of Hope - Heroic Thunderforged
                        // Lei Shen
            case 95809: // Uroe, Harbinger of Terror - Raid Finder
            case 94982: // Uroe, Harbinger of Terror - Normal
            case 96181: // Uroe, Harbinger of Terror - Thunderforged
            case 96553: // Uroe, Harbinger of Terror - Heroic
            case 96925: // Uroe, Harbinger of Terror - Heroic Thunderforged
                        // Lei Shen
            case 95815: // Torall, Rod of the Shattered Throne - Raid Finder
            case 94988: // Torall, Rod of the Shattered Throne - Normal
            case 96187: // Torall, Rod of the Shattered Throne - Thunderforged
            case 96559: // Torall, Rod of the Shattered Throne - Heroic
            case 96931: // Torall, Rod of the Shattered Throne - Heroic Thunderforged

                        // Shared Boss Loot
            case 95877: // Bo-Ris, Horror in the Night - Raid Finder
            case 95498: // Bo-Ris, Horror in the Night - Normal
            case 96249: // Bo-Ris, Horror in the Night - Thunderforged
            case 96621: // Bo-Ris, Horror in the Night - Heroic
            case 96993: // Bo-Ris, Horror in the Night - Heroic Thunderforged
                        // Shared Boss Loot
            case 95862: // Darkwood Spiritstaff - Raid Finder
            case 95507: // Darkwood Spiritstaff - Normal
            case 96234: // Darkwood Spiritstaff - Thunderforged
            case 96606: // Darkwood Spiritstaff - Heroic
            case 96978: // Darkwood Spiritstaff - Heroic Thunderforged
                        // Shared Boss Loot
            case 95876: // Do-tharak, the Swordbreaker - Raid Finder
            case 95502: // Do-tharak, the Swordbreaker - Normal
            case 96248: // Do-tharak, the Swordbreaker - Thunderforged
            case 96620: // Do-tharak, the Swordbreaker - Heroic
            case 96992: // Do-tharak, the Swordbreaker - Heroic Thunderforged
                        // Shared Boss Loot
            case 95860: // Fyn's Flickering Dagger - Raid Finder
            case 95501: // Fyn's Flickering Dagger - Normal
            case 96232: // Fyn's Flickering Dagger - Thunderforged
            case 96604: // Fyn's Flickering Dagger - Heroic
            case 96976: // Fyn's Flickering Dagger - Heroic Thunderforged
                        // Shared Boss Loot
            case 95875: // Greatsword of Frozen Hells - Raid Finder
            case 95505: // Greatsword of Frozen Hells - Normal
            case 96247: // Greatsword of Frozen Hells - Thunderforged
            case 96619: // Greatsword of Frozen Hells - Heroic
            case 96991: // Greatsword of Frozen Hells - Heroic Thunderforged
                        // Shared Boss Loot
            case 95858: // Invocation of the Dawn - Raid Finder
            case 95499: // Invocation of the Dawn - Normal
            case 96230: // Invocation of the Dawn - Thunderforged
            case 96602: // Invocation of the Dawn - Heroic
            case 96974: // Invocation of the Dawn - Heroic Thunderforged
                        // Shared Boss Loot
            case 95867: // Jerthud, Graceful Hand of the Savior - Raid Finder
            case 95500: // Jerthud, Graceful Hand of the Savior - Normal
            case 96239: // Jerthud, Graceful Hand of the Savior - Thunderforged
            case 96611: // Jerthud, Graceful Hand of the Savior - Heroic
            case 96983: // Jerthud, Graceful Hand of the Savior - Heroic Thunderforged
                        // Shared Boss Loot
            case 95859: // Miracoran, the Vehement Chord - Raid Finder
            case 95503: // Miracoran, the Vehement Chord - Normal
            case 96231: // Miracoran, the Vehement Chord - Thunderforged
            case 96603: // Miracoran, the Vehement Chord - Heroic
            case 96975: // Miracoran, the Vehement Chord - Heroic Thunderforged
                        // Shared Boss Loot
            case 95866: // Nadagast's Exsanguinator - Raid Finder
            case 95506: // Nadagast's Exsanguinator - Normal
            case 96238: // Nadagast's Exsanguinator - Thunderforged
            case 96610: // Nadagast's Exsanguinator - Heroic
            case 96982: // Nadagast's Exsanguinator - Heroic Thunderforged
                        // Shared Boss Loot
            case 97129: // Tia-Tia, the Scything Star - Raid Finder
            case 97126: // Tia-Tia, the Scything Star - Normal
            case 97128: // Tia-Tia, the Scything Star - Thunderforged
            case 97127: // Tia-Tia, the Scything Star - Heroic
            case 97130: // Tia-Tia, the Scything Star - Heroic Thunderforged
                        // Shared Boss Loot
            case 95861: // Zeeg's Ancient Kegsmasher - Raid Finder
            case 95504: // Zeeg's Ancient Kegsmasher - Normal
            case 96233: // Zeeg's Ancient Kegsmasher - Thunderforged
            case 96605: // Zeeg's Ancient Kegsmasher - Heroic
            case 96977: // Zeeg's Ancient Kegsmasher - Heroic Thunderforged
                return true;
                break;

            default:
                return false;
                break;
        }
    }
};

void AddSC_item_scripts()
{
    new item_only_for_flight();
    new item_nether_wraith_beacon();
    new item_gor_dreks_ointment();
    new item_incendiary_explosives();
    new item_mysterious_egg();
    new item_disgusting_jar();
    new item_pile_fake_furs();
    new item_petrov_cluster_bombs();
    new item_dehta_trap_smasher();
    new item_trident_of_nazjan();
    new item_captured_frog();
    new item_eye_of_black_prince();
}
