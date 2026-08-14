-- Fix Gaines 25022 crash (no self-SetData re-entrancy) and restore LoS help line.
--
-- Crash: actionlist did INC phase then SetData on self while still inside SetData
-- handling from the pole, which re-enters SmartScript and can corrupt event state.
-- Use CREATE_TIMED_EVENT so the free check runs on the next AI tick instead.
--
-- LoS: event_phase_mask 0 + NOT_REPEATABLE so help works even if phase was lost
-- after .reload; OOC_LOS params are NoHostile, MaxRange, CooldownMin, CooldownMax.

UPDATE `quest_template` SET `SourceSpellId`=0 WHERE `Id`=25022;
DELETE FROM `spell_area` WHERE `spell`=73418 AND `area`=4845 AND `quest_start`=25022;
INSERT INTO `spell_area` (`spell`, `area`, `quest_start`, `quest_end`, `aura_spell`, `racemask`, `gender`, `autocast`, `quest_start_status`, `quest_end_status`) VALUES
(73418, 4845, 25022, 25022, 0, 0, 2, 1, 74, 11);

UPDATE `gameobject_template` SET `AIName`='SmartGameObjectAI', `ScriptName`='', `faction`=1 WHERE `entry` IN (202418, 202419);
DELETE FROM `smart_scripts` WHERE `entryorguid` IN (202418, 202419) AND `source_type`=1;
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES
(202419, 1, 0, 0, 70, 0, 100, 0, 2, 0, 0, 0, 45, 0, 2, 0, 0, 0, 0, 10, 8202171, 38924, 0, 0, 0, 0, 0, 'Quilboar Restraint 202419 - On GO_ACTIVATED - Set Data 0 2 on Rope Bunny 8202171'),
(202418, 1, 0, 0, 70, 0, 100, 0, 2, 0, 0, 0, 45, 0, 2, 0, 0, 0, 0, 10, 8202172, 38924, 0, 0, 0, 0, 0, 'Quilboar Restraint 202418 - On GO_ACTIVATED - Set Data 0 2 on Rope Bunny 8202172');

UPDATE `creature_template` SET
  `modelid1`=11686,
  `modelid2`=27823,
  `minlevel`=60,
  `maxlevel`=60,
  `faction_A`=35,
  `faction_H`=35,
  `unit_flags`=33555200,
  `unit_flags2`=2048,
  `type_flags`=128,
  `InhabitType`=3,
  `AIName`='SmartAI',
  `ScriptName`=''
WHERE `entry`=38924;

DELETE FROM `creature_template_addon` WHERE `entry`=38924;
INSERT INTO `creature_template_addon` (`entry`, `path_id`, `mount`, `bytes1`, `bytes2`, `emote`, `auras`) VALUES
(38924, 0, 0, 0, 1, 0, '');

DELETE FROM `creature` WHERE `guid` IN (8202171, 8202172);
INSERT INTO `creature` (`guid`, `id`, `map`, `spawnMask`, `phaseId`, `phaseGroup`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `MovementType`, `npcflag`, `unit_flags`, `dynamicflags`) VALUES
(8202171, 38924, 1, 1, 0, 0, 0, 0, -2103.505, -2579.979, 96.25291, 5.410521, 300, 0, 0, 1, 0, 0, 0, 0, 0),
(8202172, 38924, 1, 1, 0, 0, 0, 0, -2100.543, -2583.097, 95.56024, 3.001966, 300, 0, 0, 1, 0, 0, 0, 0, 0);

DELETE FROM `smart_scripts` WHERE `entryorguid` IN (38924, -8202171, -8202172) AND `source_type`=0;
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES
(-8202171, 0, 0, 0, 38, 0, 100, 0, 0, 1, 0, 0, 11, 73113, 2, 0, 0, 0, 0, 19, 38872, 20, 0, 0, 0, 0, 0, 'Rope Bunny 8202171 - On Data 0 1 - Cast Rope Beam 73113 on Gaines'),
(-8202171, 0, 1, 2, 38, 0, 100, 1, 0, 2, 0, 0, 92, 1, 73113, 1, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Rope Bunny 8202171 - On Data 0 2 - Interrupt Rope Beam'),
(-8202171, 0, 2, 3, 61, 0, 100, 0, 0, 0, 0, 0, 45, 1, 1, 0, 0, 0, 0, 19, 38872, 20, 0, 0, 0, 0, 0, 'Rope Bunny 8202171 - Linked - Set Data 1 1 on Gaines'),
(-8202171, 0, 3, 4, 61, 0, 100, 0, 0, 0, 0, 0, 28, 73113, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Rope Bunny 8202171 - Linked - Remove Rope Beam from self'),
(-8202171, 0, 4, 5, 61, 0, 100, 0, 0, 0, 0, 0, 28, 73113, 0, 0, 0, 0, 0, 19, 38872, 20, 0, 0, 0, 0, 0, 'Rope Bunny 8202171 - Linked - Remove Rope Beam from Gaines'),
(-8202171, 0, 5, 0, 61, 0, 100, 0, 0, 0, 0, 0, 41, 500, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Rope Bunny 8202171 - Linked - Despawn'),
(-8202172, 0, 0, 0, 38, 0, 100, 0, 0, 1, 0, 0, 11, 72664, 2, 0, 0, 0, 0, 19, 38872, 20, 0, 0, 0, 0, 0, 'Rope Bunny 8202172 - On Data 0 1 - Cast Rope Beam 72664 on Gaines'),
(-8202172, 0, 1, 2, 38, 0, 100, 1, 0, 2, 0, 0, 92, 1, 72664, 1, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Rope Bunny 8202172 - On Data 0 2 - Interrupt Rope Beam'),
(-8202172, 0, 2, 3, 61, 0, 100, 0, 0, 0, 0, 0, 45, 1, 2, 0, 0, 0, 0, 19, 38872, 20, 0, 0, 0, 0, 0, 'Rope Bunny 8202172 - Linked - Set Data 1 2 on Gaines'),
(-8202172, 0, 3, 4, 61, 0, 100, 0, 0, 0, 0, 0, 28, 72664, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Rope Bunny 8202172 - Linked - Remove Rope Beam from self'),
(-8202172, 0, 4, 5, 61, 0, 100, 0, 0, 0, 0, 0, 28, 72664, 0, 0, 0, 0, 0, 19, 38872, 20, 0, 0, 0, 0, 0, 'Rope Bunny 8202172 - Linked - Remove Rope Beam from Gaines'),
(-8202172, 0, 5, 0, 61, 0, 100, 0, 0, 0, 0, 0, 41, 500, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Rope Bunny 8202172 - Linked - Despawn');

UPDATE `creature_template` SET `unit_flags`=768, `AIName`='SmartAI', `ScriptName`='' WHERE `entry`=38872;
DELETE FROM `smart_scripts` WHERE `entryorguid`=38872 AND `source_type`=0;
DELETE FROM `smart_scripts` WHERE `entryorguid` IN (3887200, 3887201, 3887202) AND `source_type`=9;
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES
(38872, 0, 0, 1, 11, 0, 100, 0, 0, 0, 0, 0, 22, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Ambassador Gaines - On Respawn - Set Phase 1'),
(38872, 0, 1, 2, 61, 0, 100, 0, 0, 0, 0, 0, 11, 72866, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Ambassador Gaines - Linked - Cast Cosmetic Stun'),
(38872, 0, 2, 0, 61, 0, 100, 0, 0, 0, 0, 0, 80, 3887201, 2, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Ambassador Gaines - Linked - Run Start Ropes Script'),
(38872, 0, 3, 4, 10, 0, 100, 1, 1, 20, 0, 0, 64, 1, 0, 0, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0, 'Ambassador Gaines - OOC LoS friendly - Store Invoker'),
(38872, 0, 4, 0, 61, 0, 100, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 12, 1, 0, 0, 0, 0, 0, 0, 'Ambassador Gaines - Linked - Say Line 0'),
-- Each rope cut increments phase (1 -> 2 -> 3)
(38872, 0, 5, 0, 38, 0, 100, 0, 1, 1, 0, 0, 23, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Ambassador Gaines - On Data 1 1 - Inc Event Phase'),
(38872, 0, 6, 0, 38, 0, 100, 0, 1, 2, 0, 0, 23, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Ambassador Gaines - On Data 1 2 - Inc Event Phase'),
-- After both cuts phase is 3 (mask 4); OOC update frees him once
(38872, 0, 7, 8, 1, 4, 100, 1, 500, 500, 0, 0, 64, 1, 0, 0, 0, 0, 0, 21, 20, 0, 0, 0, 0, 0, 0, 'Ambassador Gaines - OOC Update (phase 3) - Store Closest Player'),
(38872, 0, 8, 0, 61, 0, 100, 0, 0, 0, 0, 0, 80, 3887200, 2, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Ambassador Gaines - Linked - Run Free Script'),
(38872, 0, 9, 0, 58, 0, 100, 0, 5, 38872, 0, 0, 41, 2000, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Ambassador Gaines - On WP Ended - Despawn'),
(3887201, 9, 0, 0, 0, 0, 100, 0, 1000, 1000, 0, 0, 45, 0, 1, 0, 0, 0, 0, 11, 38924, 30, 0, 0, 0, 0, 0, 'Ambassador Gaines - Script - Set Data 0 1 on Rope Bunnies'),
(3887200, 9, 0, 0, 0, 0, 100, 0, 0, 0, 0, 0, 22, 4, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Ambassador Gaines - Script - Set Phase 4'),
(3887200, 9, 1, 0, 0, 0, 100, 0, 0, 0, 0, 0, 28, 72866, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Ambassador Gaines - Script - Remove Stun'),
(3887200, 9, 2, 0, 0, 0, 100, 0, 0, 0, 0, 0, 28, 73113, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Ambassador Gaines - Script - Remove Rope Beam 73113'),
(3887200, 9, 3, 0, 0, 0, 100, 0, 0, 0, 0, 0, 28, 72664, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Ambassador Gaines - Script - Remove Rope Beam 72664'),
(3887200, 9, 4, 0, 0, 0, 100, 0, 0, 0, 0, 0, 33, 38872, 0, 0, 0, 0, 0, 12, 1, 0, 0, 0, 0, 0, 0, 'Ambassador Gaines - Script - Kill Credit Stored'),
(3887200, 9, 5, 0, 0, 0, 100, 0, 0, 0, 0, 0, 11, 73118, 0, 0, 0, 0, 0, 12, 1, 0, 0, 0, 0, 0, 0, 'Ambassador Gaines - Script - Cast Kill Credit'),
(3887200, 9, 6, 0, 0, 0, 100, 0, 100, 100, 0, 0, 1, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Ambassador Gaines - Script - Say Line 1'),
(3887200, 9, 7, 0, 0, 0, 100, 0, 3200, 3200, 0, 0, 1, 2, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Ambassador Gaines - Script - Say Line 2'),
(3887200, 9, 8, 0, 0, 0, 100, 0, 500, 500, 0, 0, 53, 1, 38872, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Ambassador Gaines - Script - Start Waypoints');

-- Condition SourceGroup = event_id + 1 (LoS store is id 3)
DELETE FROM `conditions` WHERE `SourceTypeOrReferenceId`=22 AND `SourceEntry`=38872 AND `SourceId`=0;
INSERT INTO `conditions` (`SourceTypeOrReferenceId`, `SourceGroup`, `SourceEntry`, `SourceId`, `ElseGroup`, `ConditionTypeOrReference`, `ConditionTarget`, `ConditionValue1`, `ConditionValue2`, `ConditionValue3`, `NegativeCondition`, `ErrorType`, `ErrorTextId`, `ScriptName`, `Comment`) VALUES
(22, 4, 38872, 0, 0, 9, 0, 25022, 0, 0, 0, 0, 0, '', 'Gaines LoS help only if quest 25022 taken');
