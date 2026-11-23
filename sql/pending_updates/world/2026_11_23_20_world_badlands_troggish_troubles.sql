-- Troggish Troubles (27789): Doctor Blam's turrets (Blam Turret, 46707) could not be used. The turret was a vehicle
-- (VehicleId 1214) with a model, but it had no spell click and no spell click flag, no spells on its bar and no script, so
-- the player could not board it. Once boarded, the turret's script (4670700) summons four waves of Stonevault Ruffians
-- and Goons with spells 87029 and 87031 and gives the four wave credits (46737 to 46740). The two summoned entries were
-- level 1 friendly placeholders without a script, so they stood around instead of attacking.
-- Rows, spell click, bar spells and template values are as in Legends of Azeroth 5.4.8 (the speeds and attack times of
-- the two summons too). The source names the AI SmartVehicleAI, which this core does not have: SmartAI handles the
-- passenger events. The source also has a creature template difference in the immunity mask (16384), taken over as well.

UPDATE `creature_template` SET `npcflag` = 16777216, `AIName` = 'SmartAI', `spell1` = 87034, `spell2` = 87030, `mechanic_immune_mask` = 16384 WHERE `entry` = 46707 AND `ScriptName` = '';

DELETE FROM `npc_spellclick_spells` WHERE `npc_entry` = 46707;
INSERT INTO `npc_spellclick_spells` (`npc_entry`, `spell_id`, `cast_flags`, `user_type`) VALUES
(46707, 46598, 1, 0);

UPDATE `creature_template` SET `faction_A` = 59, `faction_H` = 59, `minlevel` = 45, `maxlevel` = 46, `baseattacktime` = 2000, `rangeattacktime` = 2000, `speed_walk` = 1.6, `speed_run` = 0.571429, `AIName` = 'SmartAI' WHERE `entry` = 46711 AND `ScriptName` = '';
UPDATE `creature_template` SET `faction_A` = 59, `faction_H` = 59, `minlevel` = 45, `maxlevel` = 46, `baseattacktime` = 2000, `rangeattacktime` = 2000, `speed_walk` = 0.6, `speed_run` = 0.214286, `AIName` = 'SmartAI' WHERE `entry` = 46712 AND `ScriptName` = '';

DELETE FROM `smart_scripts` WHERE `source_type` = 0 AND `entryorguid` IN (46707, 46711, 46712);
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `event_param5`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES
(46707, 0, 0, 1, 27, 0, 100, 0, 0, 0, 0, 0, 0, 64, 1, 0, 0, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0, 'Blam Turret - On Passenger Boarded - Store Target'),
(46707, 0, 1, 2, 61, 0, 100, 0, 0, 0, 0, 0, 0, 103, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Blam Turret - On Passenger Boarded - Set Root'),
(46707, 0, 2, 0, 61, 0, 100, 0, 0, 0, 0, 0, 0, 80, 4670700, 2, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Blam Turret - On Passenger Boarded - Run Script'),
(46707, 0, 3, 4, 28, 0, 100, 0, 0, 0, 0, 0, 0, 80, 0, 2, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Blam Turret - On Passenger Removed - Stop Script'),
(46707, 0, 4, 5, 61, 0, 100, 0, 0, 0, 0, 0, 0, 103, 0, 2, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Blam Turret - On Passenger Removed - Reset Root'),
(46707, 0, 5, 0, 61, 0, 100, 0, 0, 0, 0, 0, 0, 24, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Blam Turret - On Passenger Removed - Evade'),
(46707, 0, 6, 7, 38, 0, 100, 0, 1, 0, 0, 0, 0, 11, 87029, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Blam Turret - On Data Set 1 - Cast Troggish Troubles: Summon Ruffian (Random)'),
(46707, 0, 7, 8, 61, 0, 100, 0, 0, 0, 0, 0, 0, 11, 87031, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Blam Turret - On Data Set 1 - Cast Troggish Troubles: Summon Goon (Random)'),
(46707, 0, 8, 9, 61, 0, 100, 0, 0, 0, 0, 0, 0, 11, 87029, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Blam Turret - On Data Set 1 - Cast Troggish Troubles: Summon Ruffian (Random)'),
(46707, 0, 9, 10, 61, 0, 100, 0, 0, 0, 0, 0, 0, 11, 87031, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Blam Turret - On Data Set 1 - Cast Troggish Troubles: Summon Goon (Random)'),
(46707, 0, 10, 11, 61, 0, 100, 0, 0, 0, 0, 0, 0, 11, 87029, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Blam Turret - On Data Set 1 - Cast Troggish Troubles: Summon Ruffian (Random)'),
(46707, 0, 11, 12, 61, 0, 100, 0, 0, 0, 0, 0, 0, 11, 87031, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Blam Turret - On Data Set 1 - Cast Troggish Troubles: Summon Goon (Random)'),
(46707, 0, 12, 13, 61, 0, 100, 0, 0, 0, 0, 0, 0, 11, 87029, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Blam Turret - On Data Set 1 - Cast Troggish Troubles: Summon Ruffian (Random)'),
(46707, 0, 13, 0, 61, 0, 100, 0, 0, 0, 0, 0, 0, 11, 87031, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Blam Turret - On Data Set 1 - Cast Troggish Troubles: Summon Goon (Random)'),
(46711, 0, 0, 1, 54, 0, 100, 0, 0, 0, 0, 0, 0, 49, 0, 0, 0, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0, 'Stonevault Ruffian - On Respawn - Aggro'),
(46711, 0, 1, 0, 21, 0, 100, 0, 0, 0, 0, 0, 0, 41, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Stonevault Ruffian - On Reached Home - Despawn'),
(46712, 0, 0, 1, 54, 0, 100, 0, 0, 0, 0, 0, 0, 49, 0, 0, 0, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0, 'Stonevault Goon - On Respawn - Aggro'),
(46712, 0, 1, 0, 21, 0, 100, 0, 0, 0, 0, 0, 0, 41, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Stonevault Goon - On Reached Home - Despawn');

DELETE FROM `smart_scripts` WHERE `source_type` = 9 AND `entryorguid` = 4670700;
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `event_param5`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES
(4670700, 9, 0, 0, 0, 0, 100, 0, 5000, 5000, 0, 0, 0, 45, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Blam Turret - Script - Set Data 1'),
(4670700, 9, 1, 0, 0, 0, 100, 0, 10000, 10000, 0, 0, 0, 45, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Blam Turret - Script - Set Data 1'),
(4670700, 9, 2, 0, 0, 0, 100, 0, 10000, 10000, 0, 0, 0, 45, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Blam Turret - Script - Set Data 1'),
(4670700, 9, 3, 0, 0, 0, 100, 0, 10000, 10000, 0, 0, 0, 45, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Blam Turret - Script - Set Data 1'),
(4670700, 9, 4, 0, 0, 0, 100, 0, 5000, 5000, 0, 0, 0, 33, 46737, 0, 0, 0, 0, 0, 12, 1, 0, 0, 0, 0, 0, 0, 'Blam Turret - Script - Killcredit'),
(4670700, 9, 5, 0, 0, 0, 100, 0, 5000, 5000, 0, 0, 0, 45, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Blam Turret - Script - Set Data 1'),
(4670700, 9, 6, 0, 0, 0, 100, 0, 10000, 10000, 0, 0, 0, 45, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Blam Turret - Script - Set Data 1'),
(4670700, 9, 7, 0, 0, 0, 100, 0, 10000, 10000, 0, 0, 0, 45, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Blam Turret - Script - Set Data 1'),
(4670700, 9, 8, 0, 0, 0, 100, 0, 10000, 10000, 0, 0, 0, 45, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Blam Turret - Script - Set Data 1'),
(4670700, 9, 9, 0, 0, 0, 100, 0, 5000, 5000, 0, 0, 0, 33, 46738, 0, 0, 0, 0, 0, 12, 1, 0, 0, 0, 0, 0, 0, 'Blam Turret - Script - Killcredit'),
(4670700, 9, 10, 0, 0, 0, 100, 0, 5000, 5000, 0, 0, 0, 45, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Blam Turret - Script - Set Data 1'),
(4670700, 9, 11, 0, 0, 0, 100, 0, 10000, 10000, 0, 0, 0, 45, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Blam Turret - Script - Set Data 1'),
(4670700, 9, 12, 0, 0, 0, 100, 0, 10000, 10000, 0, 0, 0, 45, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Blam Turret - Script - Set Data 1'),
(4670700, 9, 13, 0, 0, 0, 100, 0, 10000, 10000, 0, 0, 0, 45, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Blam Turret - Script - Set Data 1'),
(4670700, 9, 14, 0, 0, 0, 100, 0, 5000, 5000, 0, 0, 0, 33, 46739, 0, 0, 0, 0, 0, 12, 1, 0, 0, 0, 0, 0, 0, 'Blam Turret - Script - Killcredit'),
(4670700, 9, 15, 0, 0, 0, 100, 0, 5000, 5000, 0, 0, 0, 45, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Blam Turret - Script - Set Data 1'),
(4670700, 9, 16, 0, 0, 0, 100, 0, 10000, 10000, 0, 0, 0, 45, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Blam Turret - Script - Set Data 1'),
(4670700, 9, 17, 0, 0, 0, 100, 0, 10000, 10000, 0, 0, 0, 45, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Blam Turret - Script - Set Data 1'),
(4670700, 9, 18, 0, 0, 0, 100, 0, 10000, 10000, 0, 0, 0, 45, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Blam Turret - Script - Set Data 1'),
(4670700, 9, 19, 0, 0, 0, 100, 0, 5000, 5000, 0, 0, 0, 33, 46740, 0, 0, 0, 0, 0, 12, 1, 0, 0, 0, 0, 0, 0, 'Blam Turret - Script - Killcredit');
