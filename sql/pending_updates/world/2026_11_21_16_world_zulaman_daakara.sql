-- Daakara's template was bound to a script name no code registers (boss_zuljin) and his Feather Vortex, Column of
-- Fire and Amani Lynx were level 1 friends with the names of the old Zul'jin scripts or none. The two spell scripts
-- and the Ring Out criterion belong to the fight.
UPDATE `creature_template` SET `ScriptName` = 'boss_daakara' WHERE `entry` = 23863 AND `ScriptName` = 'boss_zuljin';
UPDATE `creature_template` SET `minlevel` = 73, `maxlevel` = 73, `faction_A` = 14, `faction_H` = 14, `ScriptName` = 'npc_daakara_vortex' WHERE `entry` = 24136;
UPDATE `creature_template` SET `minlevel` = 73, `maxlevel` = 73, `faction_A` = 14, `faction_H` = 14, `ScriptName` = 'npc_daakara_pillar_of_flame' WHERE `entry` = 24187;
UPDATE `creature_template` SET `minlevel` = 85, `maxlevel` = 85, `faction_A` = 16, `faction_H` = 16, `ScriptName` = 'npc_daakara_amani_lynx' WHERE `entry` = 52839;

DELETE FROM `spell_script_names` WHERE `spell_id` IN (42583, 97647);
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES
(42583, 'spell_daakara_claw_rage_charge'),
(97647, 'spell_daakara_sweeping_winds');

DELETE FROM `achievement_criteria_data` WHERE `criteria_id` = 16835 AND `type` = 11;
INSERT INTO `achievement_criteria_data` (`criteria_id`, `type`, `value1`, `value2`, `ScriptName`) VALUES
(16835, 11, 0, 0, 'achievement_ring_out');
