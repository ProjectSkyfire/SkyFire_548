-- Zanzil: the Zanzili Berserker had no script, and the Pursuit and Zanzil's Fire spells need their scripts (the
-- fire trail grows in front of him, the pursuit makes a berserker chase one player).
UPDATE `creature_template` SET `ScriptName` = 'npc_zanzil_berserker' WHERE `entry` = 52054 AND `ScriptName` = '';

DELETE FROM `spell_script_names` WHERE `spell_id` IN (96342, 96914);
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES
(96342, 'spell_zanzil_pursuit'),
(96914, 'spell_zanzil_fire');
