-- Force of Nature: bind spell scripts and all treant AI, teach role spells.
DELETE FROM `spell_script_names`
WHERE `ScriptName` IN
('spell_dru_force_of_nature_balance',
 'spell_dru_force_of_nature_restoration',
 'spell_dru_force_of_nature_feral',
 'spell_dru_force_of_nature_guardian');

INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES
(33831,  'spell_dru_force_of_nature_balance'),
(102693, 'spell_dru_force_of_nature_restoration'),
(102703, 'spell_dru_force_of_nature_feral'),
(102706, 'spell_dru_force_of_nature_guardian');

UPDATE `creature_template`
SET `AIName` = '', `ScriptName` = 'npc_force_of_nature_balance', `spell1` = 113769
WHERE `entry` = 1964;

UPDATE `creature_template`
SET `AIName` = '', `ScriptName` = 'npc_force_of_nature_restoration', `spell1` = 113828
WHERE `entry` = 54983;

UPDATE `creature_template`
SET `AIName` = '', `ScriptName` = 'npc_force_of_nature_feral', `spell1` = 113770
WHERE `entry` = 54984;

UPDATE `creature_template`
SET `AIName` = '', `ScriptName` = 'npc_force_of_nature_guardian', `spell1` = 113830
WHERE `entry` = 54985;
