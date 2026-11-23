-- High Priest Venoxis: the adds of his fight were level 1 friends with no script, and three of his spells need
-- spell scripts (the toxic link, its aura, the growing pool of acid tears).
UPDATE `creature_template` SET `minlevel` = 87, `maxlevel` = 87, `faction_A` = 14, `faction_H` = 14, `ScriptName` = 'npc_venoxis_venoxis_pool_of_acid_tears' WHERE `entry` = 52197 AND `ScriptName` = '';
UPDATE `creature_template` SET `minlevel` = 87, `maxlevel` = 87, `faction_A` = 14, `faction_H` = 14, `ScriptName` = 'npc_venoxis_venomous_effusion' WHERE `entry` = 52288 AND `ScriptName` = '';
UPDATE `creature_template` SET `ScriptName` = 'npc_venoxis_venomous_effusion_stalker' WHERE `entry` = 52302 AND `ScriptName` = '';
UPDATE `creature_template` SET `minlevel` = 87, `maxlevel` = 87, `faction_A` = 14, `faction_H` = 14, `ScriptName` = 'npc_venoxis_bloodvenom' WHERE `entry` = 52525 AND `ScriptName` = '';

DELETE FROM `spell_script_names` WHERE `spell_id` IN (96475, 96477, 96521);
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES
(96475, 'spell_venoxis_toxic_link'),
(96477, 'spell_venoxis_toxic_link_aura'),
(96521, 'spell_venoxis_pool_of_acid_tears_dmg');
