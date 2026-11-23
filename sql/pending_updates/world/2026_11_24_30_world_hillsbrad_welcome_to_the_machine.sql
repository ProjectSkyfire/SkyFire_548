-- Welcome to the Machine (28096): the High Executor hands over three orders and the player has to dispense them to the
-- Horde recruits who come by (Dumass, Kingslayer Orkus, Johnny Awesome). The Skeletal Steed next to Darthalia had no
-- click, no vehicle and no script, and the three recruits had no gossip menu, text or credit, so the quest could never be
-- completed. Clicking the steed now summons the riding steed (vehicle 1270), mounts the player, and the three recruits
-- appear one after the other (2, 12 and 22 seconds later) at the spot in front of the player; choosing the one gossip
-- option makes the recruit speak, gives the credit and despawns it. The player dismounts after 32 seconds.
-- Rows, texts and positions are as in Legends of Azeroth 5.4.8. The recruits are summoned for two minutes instead of
-- until clicked, so unused ones do not pile up, and the mounting cast is triggered, as in the other vehicle quests.
-- The core here has no SmartVehicleAI, so the steed uses SmartAI.

UPDATE `creature_template` SET `npcflag` = 1, `AIName` = 'SmartAI' WHERE `entry` = 47445;
UPDATE `creature_template` SET `VehicleId` = 1270, `AIName` = 'SmartAI' WHERE `entry` = 47488;
UPDATE `creature_template` SET `gossip_menu_id` = 12279, `unit_flags` = 32768, `AIName` = 'SmartAI' WHERE `entry` = 47442;
UPDATE `creature_template` SET `gossip_menu_id` = 12276, `unit_flags` = 32768, `flags_extra` = 2, `AIName` = 'SmartAI' WHERE `entry` = 47443;
UPDATE `creature_template` SET `gossip_menu_id` = 12273, `unit_flags` = 32768, `AIName` = 'SmartAI' WHERE `entry` = 47444;

DELETE FROM `creature_text` WHERE `entry` IN (47442, 47443, 47444);
INSERT INTO `creature_text` (`entry`, `groupid`, `id`, `text`, `type`, `language`, `probability`, `emote`, `duration`, `sound`, `comment`) VALUES
(47442, 0, 0, 'I will do this ONE thing that you ask of me, quest giver. Pray I find more menial tasks to accomplish or you will be hearing from me again and I assure you that my commentary on forums of public opinion will be most unkind.', 12, 0, 100, 1, 0, 0, ''),
(47443, 0, 0, 'Yes, cowardly quest giver, sit atop your pale horse while Orkus brings glory to the Horde! I shall return with a thousand skulls!', 12, 0, 100, 25, 0, 0, ''),
(47444, 0, 0, 'NORTH! GOT IT! THANKS! BYE! THANKS!', 12, 0, 100, 3, 0, 0, '');

DELETE FROM `smart_scripts` WHERE `entryorguid` = 47442 AND `source_type` = 0 AND `id` IN (0, 1, 2, 3);
DELETE FROM `smart_scripts` WHERE `entryorguid` = 47443 AND `source_type` = 0 AND `id` IN (0, 1, 2, 3);
DELETE FROM `smart_scripts` WHERE `entryorguid` = 47444 AND `source_type` = 0 AND `id` IN (0, 1, 2, 3);
DELETE FROM `smart_scripts` WHERE `entryorguid` = 47445 AND `source_type` = 0 AND `id` IN (0, 1);
DELETE FROM `smart_scripts` WHERE `entryorguid` = 47488 AND `source_type` = 0 AND `id` IN (0, 1, 2);
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `event_param5`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES
(47442, 0, 0, 1, 62, 0, 100, 0, 12279, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Johnny Awesome - On Gossip Select - Say Text'),
(47442, 0, 1, 0, 61, 0, 100, 0, 0, 0, 0, 0, 0, 72, 0, 0, 0, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0, 'Johnny Awesome - Gossip Closed'),
(47442, 0, 2, 3, 52, 0, 100, 0, 0, 0, 0, 0, 0, 33, 47442, 0, 0, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0, 'Johnny Awesome - On Text Over - Give Quest Credit'),
(47442, 0, 3, 0, 61, 0, 100, 0, 0, 0, 0, 0, 0, 41, 2000, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Force Despawn'),
(47443, 0, 0, 1, 62, 0, 100, 0, 12276, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Orkus - On Gossip Select - Say Text'),
(47443, 0, 1, 0, 61, 0, 100, 0, 0, 0, 0, 0, 0, 72, 0, 0, 0, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0, 'Orkus - Gossip Closed'),
(47443, 0, 2, 3, 52, 0, 100, 0, 0, 0, 0, 0, 0, 33, 47443, 0, 0, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0, 'Orkus - On Text Over - Give Quest Credit'),
(47443, 0, 3, 0, 61, 0, 100, 0, 0, 0, 0, 0, 0, 41, 2000, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Force Despawn'),
(47444, 0, 0, 1, 62, 0, 100, 0, 12273, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Dumass - On Gossip Select - Say Text'),
(47444, 0, 1, 0, 61, 0, 100, 0, 0, 0, 0, 0, 0, 72, 0, 0, 0, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0, 'Dumass - Gossip Closed'),
(47444, 0, 2, 3, 52, 0, 100, 0, 0, 0, 0, 0, 0, 33, 47444, 0, 0, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0, 'Dumass - On Text Over - Give Quest Credit'),
(47444, 0, 3, 0, 61, 0, 100, 0, 0, 0, 0, 0, 0, 41, 2000, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Force Despawn'),
(47445, 0, 0, 1, 64, 0, 100, 0, 0, 0, 0, 0, 0, 72, 0, 0, 0, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0, 'Skeletal Steed'),
(47445, 0, 1, 0, 61, 0, 100, 0, 0, 0, 0, 0, 0, 12, 47488, 8, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Skeletal Steed'),
(47488, 0, 0, 1, 54, 0, 100, 0, 0, 0, 0, 0, 0, 64, 1, 0, 0, 0, 0, 0, 21, 5, 0, 0, 0, 0, 0, 0, 'Skeletal Steed'),
(47488, 0, 1, 0, 61, 0, 100, 0, 0, 0, 0, 0, 0, 80, 4748800, 2, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Skeletal Steed'),
(47488, 0, 2, 0, 28, 0, 100, 0, 0, 0, 0, 0, 0, 41, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Skeletal Steed');

DELETE FROM `smart_scripts` WHERE `entryorguid` = 4748800 AND `source_type` = 9 AND `id` IN (0, 1, 2, 3, 4);
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `event_param5`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES
(4748800, 9, 0, 0, 0, 0, 100, 0, 1, 1, 0, 0, 0, 86, 46598, 2, 12, 1, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Skeletal Steed'),
(4748800, 9, 1, 0, 0, 0, 100, 0, 2000, 2000, 0, 0, 0, 12, 47444, 3, 120000, 0, 0, 0, 8, 0, 0, 0, -578.039, 425.369, 79.8909, 5.60079, 'Skeletal Steed'),
(4748800, 9, 2, 0, 0, 0, 100, 0, 10000, 10000, 0, 0, 0, 12, 47443, 3, 120000, 0, 0, 0, 8, 0, 0, 0, -578.039, 425.369, 79.8909, 5.60079, 'Skeletal Steed'),
(4748800, 9, 3, 0, 0, 0, 100, 0, 10000, 10000, 0, 0, 0, 12, 47442, 3, 120000, 0, 0, 0, 8, 0, 0, 0, -578.039, 425.369, 79.8909, 5.60079, 'Skeletal Steed'),
(4748800, 9, 4, 0, 0, 0, 100, 0, 10000, 10000, 0, 0, 0, 28, 46598, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Skeletal Steed');
