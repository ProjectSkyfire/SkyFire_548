-- The Slave Pens: bind the three boss scripts (Mennu the Betrayer, Rokmar the Crackler, Quagmirran),
-- which had none, add Mennu's texts and the Corrupted Nova Totem explosion, and fix the levels of the
-- normal templates (the heroic level was their maximum). Rows from TrinityCore 3.3.5.
UPDATE `creature_template` SET `ScriptName` = 'boss_mennu_the_betrayer' WHERE `entry` = 17941 AND `ScriptName` = '';
UPDATE `creature_template` SET `ScriptName` = 'boss_rokmar_the_crackler' WHERE `entry` = 17991 AND `ScriptName` = '';
UPDATE `creature_template` SET `ScriptName` = 'boss_quagmirran' WHERE `entry` = 17942 AND `ScriptName` = '';

-- Mennu's lines, 3.3.5 creature_text.
DELETE FROM `creature_text` WHERE `entry` = 17941;
INSERT INTO `creature_text` (`entry`, `groupid`, `id`, `text`, `type`, `language`, `probability`, `emote`, `duration`, `sound`, `comment`) VALUES
(17941, 0, 0, 'The work must continue!', 14, 0, 100, 0, 0, 10376, 'Mennu the Betrayer - Aggro'),
(17941, 0, 1, 'You brought this on yourselves!', 14, 0, 100, 0, 0, 10378, 'Mennu the Betrayer - Aggro'),
(17941, 0, 2, 'Don''t make me kill you!', 14, 0, 100, 0, 0, 10379, 'Mennu the Betrayer - Aggro'),
(17941, 1, 0, 'It had to be done....', 14, 0, 100, 0, 0, 10380, 'Mennu the Betrayer - Kill'),
(17941, 1, 1, 'You should not have come....', 14, 0, 100, 0, 0, 10381, 'Mennu the Betrayer - Kill'),
(17941, 2, 0, 'I... deserve this....', 14, 0, 100, 0, 0, 10382, 'Mennu the Betrayer - Death');

-- Corrupted Nova Totem explodes (Fire Nova) when it dies.
UPDATE `creature_template` SET `AIName` = 'SmartAI' WHERE `entry` = 18179 AND `AIName` = '' AND `ScriptName` = '';
DELETE FROM `smart_scripts` WHERE `entryorguid` = 18179 AND `source_type` = 0;
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `event_param5`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES
(18179, 0, 0, 0, 6, 0, 100, 0, 0, 0, 0, 0, 0, 11, 33132, 2, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Corrupted Nova Totem - On Death - Cast Fire Nova');

-- Normal-mode levels (TrinityCore 3.3.5 ranges).
UPDATE `creature_template` SET `maxlevel` = 64 WHERE `entry` = 17893 AND `minlevel` = 64 AND `maxlevel` = 70;
UPDATE `creature_template` SET `maxlevel` = 62 WHERE `entry` = 21126 AND `minlevel` = 62 AND `maxlevel` = 70;
UPDATE `creature_template` SET `maxlevel` = 63 WHERE `entry` = 21127 AND `minlevel` = 63 AND `maxlevel` = 70;
UPDATE `creature_template` SET `maxlevel` = 64 WHERE `entry` = 21128 AND `minlevel` = 64 AND `maxlevel` = 70;
UPDATE `creature_template` SET `maxlevel` = 64 WHERE `entry` = 18179 AND `minlevel` = 64 AND `maxlevel` = 74;
