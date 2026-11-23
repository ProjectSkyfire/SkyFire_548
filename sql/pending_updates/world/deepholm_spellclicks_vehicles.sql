-- Deepholm: spell clicks and vehicles of the quest creatures. The click rows of the base data use 4.3.4 spells that the 5.4.8
-- client no longer has (82359 and 82456), several clickable creatures lack the spell click npc flag (so nothing could be
-- clicked), "Mor'norokk the Hateful" (the prisoner of "On Second Thought, Take One Prisoner") rides the clicker instead of
-- giving the credit, and six creatures have no vehicle id. Spells, flags and vehicle ids from the 5.4.8 data; clicks that the
-- base data already has and that work (Inspect Fragmented Shell, Inspect Slain Cannoneer and the like) are kept.
-- Based on Legends of Azeroth 5.4.8.

DELETE FROM `npc_spellclick_spells` WHERE `npc_entry` IN (42801, 42964, 43984, 43996, 44049) OR (`npc_entry` = 44126 AND `spell_id` = 82537);
INSERT INTO `npc_spellclick_spells` (`npc_entry`, `spell_id`, `cast_flags`, `user_type`) VALUES
(42801, 80633, 1, 0), -- Mor'norokk the Hateful: Mor'norokk Credit
(42964, 80633, 1, 0), -- Mor'norokk the Hateful: Mor'norokk Credit
(43984, 65109, 1, 0), -- Deactivated War Construct: Kill All Constructs
(43996, 65275, 1, 0), -- Reactivated War Construct: Constructing
(44049, 93848, 1, 0), -- Giant Mushroom: Attack Mushrooms
(44126, 82537, 1, 0); -- War Guardian: War Guardian

-- the spell click npc flag (16777216)
UPDATE `creature_template` SET `npcflag` = `npcflag` | 16777216 WHERE `entry` IN (42964, 43044, 43046, 43952, 43984, 43996, 44049, 45191);

UPDATE `creature_template` SET `VehicleId` = 1012 WHERE `entry` = 44136; -- Zoltrik's Pyremaw
UPDATE `creature_template` SET `VehicleId` = 1020 WHERE `entry` = 44137; -- Crystalspire Stone Drake
UPDATE `creature_template` SET `VehicleId` = 584 WHERE `entry` = 43116; -- Pebble
UPDATE `creature_template` SET `VehicleId` = 1339 WHERE `entry` = 48640; -- Enormous Gyreworm Bunny
UPDATE `creature_template` SET `VehicleId` = 1071 WHERE `entry` = 45004; -- Wyvern (flight to the Maelstrom portal)
UPDATE `creature_template` SET `VehicleId` = 1073 WHERE `entry` = 45024; -- Wyvern (flight into Deepholm)
