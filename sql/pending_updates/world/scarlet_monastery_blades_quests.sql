-- Blades of the Anointed (31513) and Unto Dust Thou Shalt Return (31514) used Hooded Crusaders and Blades of the Anointed
-- (64827, 64854, 64841) that are not spawned in the dungeon. Hand them to the NPCs that the heroic versions of the quests
-- (31515 and 31516) already use, which spawn in both modes.

DELETE FROM `creature_queststarter` WHERE `quest` IN (31513, 31514);
INSERT INTO `creature_queststarter` (`id`, `quest`) VALUES
(64838, 31513),
(64855, 31514);

DELETE FROM `creature_questender` WHERE `quest` IN (31513, 31514);
INSERT INTO `creature_questender` (`id`, `quest`) VALUES
(64855, 31513),
(64838, 31514),
(64842, 31514);

-- The first blade is a quest item in the normal chest: always lootable with the quest. The normal chest also held the
-- heroic blade, and the Tears of the Vale chest (not spawned) held both.
UPDATE `gameobject_loot_template` SET `ChanceOrQuestChance` = -100 WHERE `entry` = 214284 AND `item` = 87282;
DELETE FROM `gameobject_loot_template` WHERE `entry` = 214284 AND `item` = 87389;
DELETE FROM `gameobject_loot_template` WHERE `entry` = 221776 AND `item` IN (87282, 87389);

-- Both chests stand on the same spot and were spawned in every mode, so they stacked. One per mode (2 normal, 4 heroic).
UPDATE `gameobject` SET `spawnMask` = 2 WHERE `guid` = 910136 AND `id` = 214284;
UPDATE `gameobject` SET `spawnMask` = 4 WHERE `guid` = 910128 AND `id` = 214296;
