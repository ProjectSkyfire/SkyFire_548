-- Heroic fixes for Mana-Tombs. Yor, the optional heroic boss, could not be summoned: Shaffar's Stasis Chamber was not
-- spawned and had no AI, and Yor himself was friendly. The chamber is spawned for heroic only at the position of the
-- 5.4.8 data, with the TrinityCore row that summons Yor when the chamber is opened. Two heroic templates had levels
-- that disagree with the retail data.

UPDATE `gameobject_template` SET `AIName` = 'SmartGameObjectAI' WHERE `entry` = 185522 AND `AIName` = '';

DELETE FROM `gameobject` WHERE `guid` = 1062290;
INSERT INTO `gameobject` (`guid`, `id`, `map`, `spawnMask`, `phaseId`, `phaseGroup`, `position_x`, `position_y`, `position_z`, `orientation`, `rotation0`, `rotation1`, `rotation2`, `rotation3`, `spawntimesecs`, `animprogress`, `state`) VALUES
(1062290, 185522, 557, 4, 0, 0, -224.035, -9.92103, 16.6866, 0.130538, 0, 0, 0.0652229, 0.997871, 7200, 100, 1);

DELETE FROM `smart_scripts` WHERE `entryorguid` = 185522 AND `source_type` = 1;
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `event_param5`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES
(185522, 1, 0, 0, 70, 0, 100, 0, 2, 0, 0, 0, 0, 12, 22930, 8, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Shaffar''s Stasis Chamber - On Gameobject State Changed - Summon Creature ''Yor''');

UPDATE `creature_template` SET `faction_A` = 14, `faction_H` = 14, `flags_extra` = 1 WHERE `entry` = 22930;

UPDATE `creature_template` SET `minlevel` = 70, `maxlevel` = 70 WHERE `entry` = 20252; -- Arcane Fiend heroic, was 70-71
UPDATE `creature_template` SET `minlevel` = 70, `maxlevel` = 70 WHERE `entry` = 20254; -- Ethereal Beacon heroic, was 70-71
