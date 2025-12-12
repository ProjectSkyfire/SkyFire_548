-- Fix npc_vendor errors

-- 1. Remove vendor entries for creatures without vendor flag
-- These creatures exist but don't have UNIT_NPC_FLAG_VENDOR (0x80) set in npcflag
-- Since they don't have vendor flags, the vendor entries are invalid

-- Entry 63367: Has npcflag=1 (GOSSIP only), no vendor flag
DELETE FROM `npc_vendor` WHERE `entry` = 63367;

-- Entry 72007: Has npcflag=1 (GOSSIP only), no vendor flag
DELETE FROM `npc_vendor` WHERE `entry` = 72007;

-- Entry 72993: Has npcflag=1 (GOSSIP only), no vendor flag
DELETE FROM `npc_vendor` WHERE `entry` = 72993;

-- Entry 73004: Has npcflag=1 (GOSSIP only), no vendor flag
DELETE FROM `npc_vendor` WHERE `entry` = 73004;

-- Entry 73819: Has npcflag=1 (GOSSIP only), no vendor flag
DELETE FROM `npc_vendor` WHERE `entry` = 73819;

-- 2. Remove vendor entries with non-existent items
-- These items don't exist in MoP item_template

-- Vendor 70436: Item 102457 does not exist
DELETE FROM `npc_vendor` WHERE `entry` = 70436 AND `item` = 102457;

-- Vendor 73307: Item 102463 does not exist
DELETE FROM `npc_vendor` WHERE `entry` = 73307 AND `item` = 102463;

