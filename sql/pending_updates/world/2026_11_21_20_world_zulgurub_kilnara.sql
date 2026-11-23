-- High Priestess Kilnara: the Pride of Bethekk had no script and the Wave of Agony and Cave In stalkers were
-- level 1 friends or level 85 mobs of the wrong level. The two Wave of Agony spells need their scripts. The
-- Here, Kitty Kitty rat spells (98178, 98238) are not scripted.
UPDATE `creature_template` SET `minlevel` = 84, `maxlevel` = 84, `ScriptName` = 'npc_kilnara_pride_of_bethekk' WHERE `entry` = 52061 AND `ScriptName` = '';
UPDATE `creature_template` SET `minlevel` = 87, `maxlevel` = 87, `faction_A` = 14, `faction_H` = 14 WHERE `entry` IN (52147, 52160) AND `minlevel` = 1;
UPDATE `creature_template` SET `minlevel` = 87, `maxlevel` = 87 WHERE `entry` = 52387 AND `minlevel` = 85;

DELETE FROM `spell_script_names` WHERE `spell_id` IN (96457, 98269);
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES
(96457, 'spell_kilnara_wave_of_agony_start'),
(98269, 'spell_kilnara_wave_of_agony_target');
