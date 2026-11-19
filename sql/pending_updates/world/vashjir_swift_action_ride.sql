-- Vashj'ir, "Swift Action" (25963): talk to Fiasco Sizzlegrin and ride the Tamed Seahorse over the terrace, dropping improvised
-- explosives on 100 Azsh'ir Soldiers, 20 Wave Screamers and 12 Archaeans. The option of menu 11534 had no script. In the client
-- data it casts "Forcecast Ruins Assault Horde" (78053), which credits Fiasco and force-casts 78051: that summons the Tamed
-- Seahorse (41868, vehicle 816) and puts the player in phase 181. The phase is left out on purpose: a phase here replaces the
-- default one, so every Azsh'ir on the terrace (800 spawns, all in the default phase) would vanish. The seahorse has no spell
-- bar here; the Alliance twin (41247) carries Improvised Explosives (77330) in the 4.3.4 and 5.4.8 data, so the Horde one gets
-- the same. The player boards by casting "Ride Vehicle Hardcoded" (46598) on the seahorse, as a click would.

UPDATE `creature_template` SET `VehicleId` = 816, `spell1` = 77330, `AIName` = 'SmartAI' WHERE `entry` = 41868;
UPDATE `creature_template` SET `AIName` = 'SmartAI' WHERE `entry` = 40918;

DELETE FROM `smart_scripts` WHERE `source_type` = 0 AND `entryorguid` IN (40918, 41868);
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `event_param5`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES
(40918, 0, 0, 1, 62, 0, 100, 0, 11534, 0, 0, 0, 0, 33, 40918, 0, 0, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0, 'On Gossip Option 0 Selected - Quest Credit Fiasco Sizzlegrin'),
(40918, 0, 1, 2, 61, 0, 100, 0, 0, 0, 0, 0, 0, 72, 0, 0, 0, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0, 'On Link - Close Gossip'),
(40918, 0, 2, 0, 61, 0, 100, 0, 0, 0, 0, 0, 0, 12, 41868, 1, 900000, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0, 'On Link - Summon Tamed Seahorse at the Player'),
(41868, 0, 0, 0, 54, 0, 100, 0, 0, 0, 0, 0, 0, 85, 46598, 2, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'On Just Summoned - Summoner Rides This Vehicle');
