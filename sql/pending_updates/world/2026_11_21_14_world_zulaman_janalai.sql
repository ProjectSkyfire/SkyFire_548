-- Jan'alai's Flame Breath lays a trail of fire on the ground (aura script), the second Amani'shi Hatcher was
-- unscripted, and the hatchers, hatchlings and fire bombs still had their level 70 to 73 values.
DELETE FROM `spell_script_names` WHERE `spell_id` = 43140;
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES
(43140, 'spell_janalai_flame_breath');

UPDATE `creature_template` SET `ScriptName` = 'npc_janalai_hatcher' WHERE `entry` = 24504 AND `ScriptName` = '';
UPDATE `creature_template` SET `minlevel` = 85, `maxlevel` = 85 WHERE `entry` IN (23598, 23818, 24504) AND `minlevel` = 70;
UPDATE `creature_template` SET `minlevel` = 87, `maxlevel` = 87 WHERE `entry` = 23920 AND `minlevel` = 73;
