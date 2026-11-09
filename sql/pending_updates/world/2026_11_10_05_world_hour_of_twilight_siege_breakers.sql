-- Twilight Siege Breakers: the Twilight Portals (aura 110160) summon them for 20 seconds every 6 seconds (spell 110161, and
-- 108013 for the 4.3.4 entry 57259), and the 4.3.4 data scripts them: a new Breaker follows the nearest Siege Breaker
-- Stalker and is killed when the stalker has one within 10 yards, and it stays passive. The 31 standing Siege Breaker
-- spawns of the earlier data are recorded summons of that kind, so they are removed together with their patrol paths.
-- Safe to run twice: the script rows are deleted by entry before they are inserted.
UPDATE `creature_template` SET `AIName`='SmartAI' WHERE `entry` IN (57261, 58234) AND `ScriptName`='';
DELETE FROM `smart_scripts` WHERE `entryorguid` IN (57261, 58234) AND `source_type`=0;
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `event_param5`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES
(58234, 0, 0, 2, 54, 0, 100, 0, 0, 0, 0, 0, 0, 29, 1, 0, 0, 0, 0, 0, 19, 57261, 500, 0, 0, 0, 0, 0, 'Twilight Siege Breaker - On Just Summoned - Start Follow Closest Creature ''Siege Breaker Stalker'''),
(58234, 0, 1, 0, 38, 0, 100, 0, 1, 0, 0, 0, 0, 51, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Twilight Siege Breaker - On Data Set 1 0 - Kill Self'),
(58234, 0, 2, 0, 61, 0, 100, 0, 0, 0, 0, 0, 0, 8, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Twilight Siege Breaker - On Just Summoned - Set Reactstate Passive'),
(57261, 0, 0, 0, 1, 0, 100, 0, 1000, 2000, 1000, 2000, 0, 45, 1, 0, 0, 0, 0, 0, 19, 58234, 10, 0, 0, 0, 0, 0, 'Siege Breaker Stalker - Out Of Combat - Set Data 1 0 On Closest Twilight Siege Breaker');

DELETE FROM `creature` WHERE (`guid`, `id`) IN ((9250742, 58234), (9250743, 58234), (9250744, 58234), (9250747, 58234), (9250748, 58234), (9250749, 58234), (9250750, 58234), (9250753, 58234), (9250754, 58234), (9250755, 58234), (9250756, 58234), (9250757, 58234), (9250758, 58234), (9250763, 58234), (9250764, 58234), (9250765, 58234), (9250766, 58234), (9250768, 58234), (9250769, 58234), (9250851, 58234), (9250854, 58234), (9250855, 58234), (9250856, 58234), (9250858, 58234), (9250859, 58234), (9250864, 58234), (9250865, 58234), (9250867, 58234), (9250868, 58234), (9250872, 58234), (9250873, 58234));
DELETE FROM `creature_addon` WHERE `guid` IN (9250742, 9250743, 9250744, 9250747, 9250748, 9250749, 9250750, 9250753, 9250754, 9250755, 9250756, 9250757, 9250758, 9250763, 9250764, 9250765, 9250766, 9250768, 9250769, 9250851, 9250854, 9250855, 9250856, 9250858, 9250859, 9250864, 9250865, 9250867, 9250868, 9250872, 9250873);
DELETE FROM `waypoint_data` WHERE `id` IN (9250756, 9250757, 9250758, 9250765, 9250766, 9250864, 9250865, 9250867, 9250868, 9250872, 9250873);
