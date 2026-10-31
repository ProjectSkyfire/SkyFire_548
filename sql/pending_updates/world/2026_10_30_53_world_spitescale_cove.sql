-- Spitescale Cove, Echo Isles: the Darkspear Shamans and the naga had no SmartAI, so the battle on the beach never
-- started and the naga never cast. The Darkspear Watchers guarding it were level 5; they are level 90 elite guards.
UPDATE `creature_template` SET `AIName` = 'SmartAI' WHERE `entry` IN (38217, 38300, 38301, 38326);
UPDATE `creature_template` SET `minlevel` = 90, `maxlevel` = 90, `exp` = 3, `mindmg` = 342, `maxdmg` = 485, `attackpower` = 392,
    `minrangedmg` = 295, `maxrangedmg` = 438, `rangedattackpower` = 68 WHERE `entry` = 38217;

DELETE FROM `smart_scripts` WHERE `source_type` = 0 AND `entryorguid` IN (38217, 38300, 38301, 38326);
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `event_param5`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES
(38217, 0, 0, 0, 0, 0, 100, 0, 0, 0, 2300, 3900, 0, 11, 95826, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 'Darkspear Watcher - In combat - Cast Shoot'),
(38300, 0, 0, 0, 11, 0, 100, 0, 0, 0, 0, 0, 0, 8, 2, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Spitescale Wavethrasher - On respawn - Set react state aggressive'),
(38300, 0, 1, 0, 0, 0, 100, 0, 4000, 5000, 17000, 23000, 0, 11, 79810, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 'Spitescale Wavethrasher - In combat - Cast Frost Cleave'),
(38301, 0, 0, 0, 11, 0, 100, 0, 0, 0, 0, 0, 0, 8, 2, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Spitescale Siren - On respawn - Set react state aggressive'),
(38301, 0, 1, 0, 9, 0, 100, 0, 0, 40, 3400, 4700, 0, 11, 32011, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 'Spitescale Siren - Within 0-40 yards - Cast Water Bolt'),
(38301, 0, 2, 0, 0, 0, 100, 0, 5000, 8000, 22000, 24000, 0, 11, 15117, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 'Spitescale Siren - In combat - Cast Chain Lightning'),
(38326, 0, 0, 0, 0, 0, 100, 0, 3400, 4700, 3400, 4700, 0, 11, 73212, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 'Darkspear Shaman - In combat - Cast Lightning Bolt'),
(38326, 0, 1, 0, 2, 0, 100, 0, 0, 40, 14000, 21000, 0, 11, 72014, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Darkspear Shaman - At 40% health - Cast Chain Heal'),
(38326, 0, 2, 0, 1, 0, 100, 0, 1000, 1000, 5000, 5000, 0, 49, 0, 0, 0, 0, 0, 0, 11, 38300, 30, 0, 0, 0, 0, 0, 'Darkspear Shaman - Out of combat - Attack the nearest Spitescale Wavethrasher');
