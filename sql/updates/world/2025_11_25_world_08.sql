-- Fix Creature Gossip Flag Issues
-- Add UNIT_NPC_FLAG_GOSSIP (1) to creatures that have gossip_menu_id but missing the flag
UPDATE `creature_template` SET `npcflag` = `npcflag` | 1 WHERE `entry` IN (
54567, 54943, 66998, 70279, 73591, 73592, 73597, 73598, 73603, 73604, 73605, 73606, 73607, 74216, 74217
);

-- Remove UNIT_NPC_FLAG_GOSSIP (1) from creatures that have the flag but gossip_menu_id is 0
UPDATE `creature_template` SET `npcflag` = `npcflag` & ~1 WHERE `entry` IN (
63596, 63626, 68986, 72488
);

