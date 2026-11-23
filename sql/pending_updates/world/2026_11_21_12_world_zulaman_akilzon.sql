-- Akil'zon's Electrical Storm lifts and shields its target (aura script) and hurts everyone else (spell script on
-- the damage tick). The Amani Kidnapper that carries off a player is a level 85 hostile summon, not a level 1 friend.
DELETE FROM `spell_script_names` WHERE `spell_id` IN (43648, 43657);
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES
(43648, 'spell_akilzon_electrical_storm'),
(43657, 'spell_akilzon_electrical_storm_dmg');

UPDATE `creature_template` SET `minlevel` = 85, `maxlevel` = 85, `faction_A` = 1890, `faction_H` = 1890 WHERE `entry` = 52638 AND `minlevel` = 1;
