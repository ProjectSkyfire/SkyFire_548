-- Young and Vicious (24626): the Bloodtalon Lasso on Swiftclaw (37989) gives the capture credit and puts the player in the
-- driving seat of a rideable Swiftclaw (38002), which gives the second credit at the raptor pens near Darkspear Hold.
UPDATE `creature_template` SET `ScriptName` = 'npc_swiftclaw' WHERE `entry` = 37989;
UPDATE `creature_template` SET `ScriptName` = 'npc_swiftclaw_vehicle', `VehicleId` = 617, `npcflag` = 0, `unit_flags` = 512,
    `speed_run` = 1.71429 WHERE `entry` = 38002;

DELETE FROM `creature_text` WHERE `entry` = 38002;
INSERT INTO `creature_text` (`entry`, `groupid`, `id`, `text`, `type`, `language`, `probability`, `emote`, `duration`, `sound`, `comment`) VALUES
(38002, 0, 0, 'Swiftclaw isn''t stopping!  Steer him back to the raptor pens near Darkspear Hold.', 41, 0, 100, 0, 0, 0, 'Swiftclaw - Rider on board');
