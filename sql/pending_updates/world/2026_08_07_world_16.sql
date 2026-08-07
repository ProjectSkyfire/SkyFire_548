DELETE FROM `spell_script_names` WHERE `spell_id`=72925;
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES
(72925, 'spell_barrens_bandage');

UPDATE `creature_template` SET `ScriptName`='npc_barrent_wounded_defender' WHERE `entry`=38805;

DELETE FROM `creature_text` WHERE `entry`=38805;
INSERT INTO `creature_text` (`entry`, `groupid`, `id`, `text`, `type`, `language`, `probability`, `emote`, `duration`, `sound`, `comment`) VALUES
(38805, 0, 0, 'I think I can limp back to the fortress...', 12, 0, 100, 0, 0, 0, 'Wounded Defender'),
(38805, 0, 1, 'These bandages reek! I guess I shouldn''t complain.', 12, 0, 100, 0, 0, 0, 'Wounded Defender'),
(38805, 0, 2, 'Thank you - this was turning into a massacre...', 12, 0, 100, 0, 0, 0, 'Wounded Defender');
