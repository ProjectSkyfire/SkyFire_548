-- Bind only templates without an existing AI or script.
UPDATE `creature_template` SET `ScriptName` = 'boss_brother_korloff' WHERE `entry` = 59223 AND `ScriptName` = '' AND `AIName` = '';
UPDATE `creature_template` SET `ScriptName` = 'npc_trigger_scorched_flame' WHERE `entry` = 59507 AND `ScriptName` = '' AND `AIName` = '';

DELETE FROM `creature_text` WHERE `entry` = 59223 AND `groupid` BETWEEN 0 AND 3;
INSERT INTO `creature_text` (`entry`, `groupid`, `id`, `text`, `type`, `language`, `probability`, `emote`, `duration`, `sound`, `comment`) VALUES
(59223, 0, 0, 'I will break you.', 14, 0, 100, 0, 0, 29496, 'Korloff aggro'),
(59223, 1, 0, 'You are like... piece of iron...', 14, 0, 100, 0, 0, 29497, 'Korloff death'),
(59223, 2, 0, 'If you die, you die.', 14, 0, 100, 0, 0, 29500, 'Korloff Firestorm'),
(59223, 3, 0, 'My fists are scarlet... with your blood!', 14, 0, 100, 0, 0, 29499, 'Korloff Blazing Fists');

DELETE FROM `spell_script_names` WHERE (`spell_id` = 113764 AND `ScriptName` = 'spell_firestorm_kick') OR (`spell_id` = 114460 AND `ScriptName` = 'spell_scorched_earth') OR (`spell_id` = 125852 AND `ScriptName` = 'spell_korloff_burning_man');
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES
(113764, 'spell_firestorm_kick'),
(114460, 'spell_scorched_earth'),
(125852, 'spell_korloff_burning_man');
