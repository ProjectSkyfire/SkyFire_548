-- Searing Flames (77657): Searing Totem's Searing Bolt applies the damage buff to its owner. Based on Legends of Azeroth 5.4.8. Safe to run twice.
DELETE FROM `spell_script_names` WHERE `spell_id` = 3606 AND `ScriptName` = 'spell_sha_searing_bolt';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES
(3606, 'spell_sha_searing_bolt');
