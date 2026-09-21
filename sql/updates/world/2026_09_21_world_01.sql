-- Innkeepers who cannot bind a hearthstone. Two independent faults.
--
-- FAULT 1 -- bind and shop options stored as (OptionType 0, OptionNpcflag 0).
--
-- Player::PrepareGossipMenu drops any option whose OptionNpcflag does not intersect the
-- creature's npcflag, and 0 intersects nothing:
--
--     if (!(itr->second.OptionNpcflag & npcflags))
--         continue;
--
-- The test runs before the switch that would otherwise reject an unknown OptionType, so the rows
-- are discarded in silence. The text, icons and broadcast text ids are already correct; only the
-- two type columns are wrong. GOSSIP_OPTION_INNKEEPER (8) and GOSSIP_OPTION_VENDOR (3) need no
-- script -- Player::OnGossipSelect calls SetBindPoint and SendListInventory itself.
--
-- 17 spawned innkeepers that do carry UNIT_NPC_FLAG_GOSSIP draw no bind line because of this,
-- across the 14 menus below. Menus with no options at all are NOT affected and are not touched:
-- PrepareGossipMenu falls back to menu 0 when the creature's own menu is empty.
--
-- The icon is the reliable signal: icon 5 (GOSSIP_ICON_INTERACT_2) is the bind line and icon 1
-- (GOSSIP_ICON_VENDOR) the shop line on every one of these menus, matching the canonical rows on
-- fallback menu 0 (OptionID 6 -> type 8 / 65536, OptionID 1 -> type 3 / 128).

-- Bind option. Where a menu lists the pair twice (7869), only the lowest OptionID is promoted so
-- no line appears twice.
UPDATE `gossip_menu_option` SET `OptionType` = 8, `OptionNpcflag` = 65536
WHERE `OptionType` = 0 AND `OptionNpcflag` = 0 AND (`MenuID`, `OptionID`) IN (
    (1293,1),   -- Innkeeper Keldamyr 6736
    (6059,0),   -- Lard 14731
    (7290,0),   -- Floyd Pinkus 16602
    (7497,0),   -- Caregivers Breel 16739, Topher Loaal 17553, Ophera Windfury 18906, Inaara 25036
    (7869,0),   -- Innkeeper Grilka 18957
    (8912,0),   -- "Little" Logok 24208
    (11520,0),  -- Erunak Stonespeaker 36915
    (11538,0),  -- Caretaker Movra 39878
    (11568,0),  -- Grimy Greasefingers 42473
    (12003,0),  -- Caretaker Nuunwa 45300
    (12040,0),  -- Tinza Silvermug 45563
    (12101,2),  -- Provisioner Elda 46271
    (12596,0),  -- The Great Pisani 49934
    (12677,1)   -- Gwen Armstead 48735
);

-- Vendor option, for the menus whose innkeeper carries UNIT_NPC_FLAG_VENDOR and has npc_vendor
-- rows. Without it these menus would gain a bind line and lose the shop, which is only reachable
-- today because the menu draws empty.
--
-- 11538 (Caretaker Movra) and 12101 (Provisioner Elda) are deliberately left out: both creatures
-- have the vendor flag and no npc_vendor rows, so PrepareGossipMenu would hide the line again and
-- log "has UNIT_NPC_FLAG_VENDOR set but has an empty trading item list".
UPDATE `gossip_menu_option` SET `OptionType` = 3, `OptionNpcflag` = 128
WHERE `OptionType` = 0 AND `OptionNpcflag` = 0 AND (`MenuID`, `OptionID`) IN (
    (1293,2), (6059,1), (7290,1), (7497,1), (7869,1),
    (8912,1), (11520,1), (12040,1), (12677,2)
);

-- Menu 9868 has the two npcflags transposed: the bind option asks for 128 (VENDOR) and the shop
-- option for 65536 (INNKEEPER). Across the whole table 66 of the 68 OptionType 8 rows use 65536
-- and 321 of the 336 OptionType 3 rows use 128; these two are the outliers. All ten creatures on
-- the menu happen to carry both bits (npcflag 66177), so nothing is visibly broken today, but the
-- rows are wrong and break for any innkeeper on this menu that does not also sell.
UPDATE `gossip_menu_option` SET `OptionNpcflag` = 65536
 WHERE `MenuID` = 9868 AND `OptionID` = 0 AND `OptionType` = 8 AND `OptionNpcflag` = 128;
UPDATE `gossip_menu_option` SET `OptionNpcflag` = 128
 WHERE `MenuID` = 9868 AND `OptionID` = 1 AND `OptionType` = 3 AND `OptionNpcflag` = 65536;

-- FAULT 2 -- innkeeper questgivers without UNIT_NPC_FLAG_GOSSIP. Closes #1354.
--
-- Player::SendPreparedGossip:
--
--     // in case no gossip flag and quest menu not empty, open quest menu
--     if (!source->ToCreature()->HasFlag(UNIT_FIELD_NPC_FLAGS, UNIT_NPC_FLAG_GOSSIP)
--         && !PlayerTalkClass->GetQuestMenu().Empty())
--     {
--         SendPreparedQuest(source->GetGUID());
--         return;
--     }
--
-- These 13 creatures are innkeepers and questgivers with gossip_menu_id 0 and no GOSSIP flag, and
-- each has at least one row in creature_queststarter or creature_questender. Whenever the player
-- has quest business with them the quest window opens in place of the service menu and the
-- hearthstone cannot be rebound; with no quest pending the menu appears and binding works, which
-- is why it reads as intermittent. 39 innkeeper questgivers already carry the flag.
--
-- Lin Windfur (55233) has the questgiver flag and no quest rows, so its quest menu is always
-- empty and it is not affected; it is left alone.

UPDATE `creature_template` SET `npcflag` = `npcflag` | 1
 WHERE `entry` IN (
    1247,  -- Innkeeper Belm
    5111,  -- Innkeeper Firebrew
    6741,  -- Innkeeper Norman
    41618, -- Erunak Stonespeaker
    55180, -- Shademaster Kiryn
    55809, -- Peiji Goldendraft
    59405, -- Li Goldendraft
    62322, -- Graceful Swan
    62871, -- Puli the Even Handed
    62872, -- Cranfur the Noodler
    62996, -- Madam Vee Luo
    64149, -- Matron Vi Vinh
    65528  -- Nan Thunderfoot
   )
   AND (`npcflag` & 1) = 0;
