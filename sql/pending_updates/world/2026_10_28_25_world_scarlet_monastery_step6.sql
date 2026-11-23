-- Entry 3977 may still carry the switched-off pre-MoP script name; map 189 has no spawns or instance.
UPDATE `creature_template` SET `ScriptName` = 'boss_commander_durand' WHERE `entry` = 60040 AND `ScriptName` = '' AND `AIName` = '';
UPDATE `creature_template` SET `ScriptName` = 'boss_high_inqusitior_whitemane' WHERE `entry` = 3977 AND `ScriptName` IN ('', 'boss_high_inquisitor_whitemane') AND `AIName` = '';
UPDATE `creature_template` SET `ScriptName` = 'npc_hooded_crusader_c64842' WHERE `entry` = 64842 AND `ScriptName` = '' AND `AIName` = '';

DELETE FROM `creature_text` WHERE `entry` = 60040 AND `groupid` IN (0, 1);
INSERT INTO `creature_text` (`entry`, `groupid`, `id`, `text`, `type`, `language`, `probability`, `emote`, `duration`, `sound`, `comment`) VALUES
(60040, 0, 0, 'My legend begins NOW!', 12, 0, 100, 0, 0, 27527, 'Durand aggro'),
(60040, 1, 0, 'My blade is peerless!', 12, 0, 100, 0, 0, 27530, 'Durand Dashing Strike');

-- The LoA entry 3977 lines are from the older dungeon. Do not replace them.
-- Outro dialogue is documented in the LoA encounter comments.
DELETE FROM `creature_text` WHERE `entry` = 64842 AND `groupid` IN (0, 1, 2, 3, 4);
INSERT INTO `creature_text` (`entry`, `groupid`, `id`, `text`, `type`, `language`, `probability`, `emote`, `duration`, `sound`, `comment`) VALUES
(64842, 0, 0, 'It is done. You did it!', 12, 0, 100, 0, 0, 0, 'Hooded Crusader outro'),
(64842, 1, 0, '%s gazes down upon the high inquisitor''s blade-skewered corpse.', 16, 0, 100, 0, 0, 0, 'Hooded Crusader outro'),
(64842, 2, 0, 'And now enough of this deception.', 12, 0, 100, 0, 0, 0, 'Hooded Crusader outro'),
(64842, 3, 0, 'Hello, my name''s Lilian. You don''t mind if I take those blades, do you?', 12, 0, 100, 0, 0, 0, 'Lilian outro'),
(64842, 4, 0, 'Yes, these will do quite nicely. They feel almost as if they were meant for me all along.', 12, 0, 100, 0, 0, 0, 'Lilian outro');

DELETE FROM `spell_script_names` WHERE (`spell_id` = 9232 AND `ScriptName` = 'spell_sc_scarlet_ressurection') OR (`spell_id` = 113134 AND `ScriptName` = 'spell_sc_mass_ressurection');
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES
(9232, 'spell_sc_scarlet_ressurection'),
(113134, 'spell_sc_mass_ressurection');
