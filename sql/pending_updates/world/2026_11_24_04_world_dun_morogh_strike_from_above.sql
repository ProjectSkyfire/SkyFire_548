-- Strike From Above (25841): the Signal Flare (spell 77488) is used at Frostmane Retreat North (41372) and South (41373); the
-- credit was given, but nothing else happened and the spell had no target condition (upstream issue 1412). Legends of Azeroth
-- 5.4.8 summons a Covert Ops Flying Machine (41382) at each retreat; it flies in, fires three Deathblade Rocket Barrages at the
-- Bombardment Bunny (41375) and leaves. The flare conditions, the machine's template, flight and script, and the bunny's script
-- are as in that source. The source gives two rows of the machine's script the id 2; the second chain is renumbered 3 to 5.

DELETE FROM `conditions` WHERE `SourceTypeOrReferenceId` IN (13, 17) AND `SourceEntry` = 77488;
INSERT INTO `conditions` (`SourceTypeOrReferenceId`, `SourceGroup`, `SourceEntry`, `SourceId`, `ElseGroup`, `ConditionTypeOrReference`, `ConditionTarget`, `ConditionValue1`, `ConditionValue2`, `ConditionValue3`, `NegativeCondition`, `ErrorType`, `ErrorTextId`, `ScriptName`, `Comment`) VALUES
(13, 1, 77488, 0, 0, 31, 0, 3, 41372, 0, 0, 0, 0, '', 'Signal Flare - Target is Frostmane Retreat'),
(13, 1, 77488, 0, 1, 31, 0, 3, 41373, 0, 0, 0, 0, '', 'Signal Flare - Target is Frostmane Retreat'),
(17, 0, 77488, 0, 0, 29, 0, 41372, 20, 0, 0, 0, 0, '', 'Signal Flare - Frostmane Retreat nearby'),
(17, 0, 77488, 0, 1, 29, 0, 41373, 20, 0, 0, 0, 0, '', 'Signal Flare - Frostmane Retreat nearby');

UPDATE `creature_template` SET `minlevel` = 10, `maxlevel` = 10, `faction_A` = 1802, `faction_H` = 1802, `unit_flags` = 768, `spell1` = 77477, `AIName` = 'SmartAI' WHERE `entry` = 41382 AND `ScriptName` = '';
UPDATE `creature_template` SET `AIName` = 'SmartAI' WHERE `entry` = 41375 AND `ScriptName` = '';

DELETE FROM `creature_template_addon` WHERE `entry` = 41382;
INSERT INTO `creature_template_addon` (`entry`, `path_id`, `mount`, `bytes1`, `bytes2`, `emote`, `auras`) VALUES
(41382, 0, 22719, 50331648, 1, 0, NULL);

DELETE FROM `smart_scripts` WHERE `source_type` = 0 AND `entryorguid` IN (41372, 41373, 41375);
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `event_param5`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES
(41372, 0, 0, 1, 8, 0, 100, 0, 77488, 0, 0, 0, 0, 33, 41372, 0, 0, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0, 'Frostmane Retreat North - On SpellHit Signal Flare - Quest Credit'),
(41372, 0, 1, 2, 61, 0, 100, 0, 0, 0, 0, 0, 0, 12, 41382, 8, 0, 0, 0, 0, 8, 0, 0, 0, -5705.89, -997.903, 416.821, 0, 'Frostmane Retreat North - Linked To Id 0 - Summon Creature Covert Ops Flying Machine'),
(41372, 0, 2, 0, 61, 0, 100, 0, 0, 0, 0, 0, 0, 80, 4137200, 2, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Frostmane Retreat North - Linked To Id 1 - Run Script'),
(41373, 0, 0, 1, 8, 0, 100, 0, 77488, 0, 0, 0, 0, 33, 41373, 0, 0, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0, 'Frostmane Retreat South - On SpellHit Signal Flare - Quest Credit'),
(41373, 0, 1, 2, 61, 0, 100, 0, 0, 0, 0, 0, 0, 12, 41382, 8, 0, 0, 0, 0, 8, 0, 0, 0, -5777.12, -956.198, 412.096, 0, 'Frostmane Retreat South - Linked To Id 0 - Summon Creature Covert Ops Flying Machine'),
(41373, 0, 2, 0, 61, 0, 100, 0, 0, 0, 0, 0, 0, 80, 4137300, 2, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Frostmane Retreat South - Linked To Id 1 - Run Script'),
(41375, 0, 0, 0, 11, 0, 100, 0, 0, 0, 0, 0, 0, 42, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Bombardment Bunny - On Respawn - Set Invincibility Hp 1'),
(41375, 0, 1, 2, 8, 0, 100, 0, 85915, 0, 0, 0, 0, 11, 77477, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Bombardment Bunny - On SpellHit Deathblade Rocket Barrage - Cast Missile Barrage'),
(41375, 0, 2, 3, 61, 0, 100, 0, 0, 0, 0, 0, 0, 11, 84788, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Bombardment Bunny - Linked To Id 1 - Cast Fleshburner Cosmetic Fire Aura'),
(41375, 0, 3, 0, 61, 0, 100, 0, 0, 0, 0, 0, 0, 45, 0, 1, 0, 0, 0, 0, 11, 41375, 50, 0, 0, 0, 0, 0, 'Bombardment Bunny - Linked To Id 2 - Set Data 0 1'),
(41375, 0, 5, 0, 38, 0, 100, 0, 0, 1, 0, 0, 0, 11, 84788, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Bombardment Bunny - On Data Set 0 1 - Cast Fleshburner Cosmetic Fire Aura');

DELETE FROM `smart_scripts` WHERE `source_type` = 0 AND `entryorguid` = 41382;
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `event_param5`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES
(41382, 0, 0, 1, 38, 0, 100, 0, 0, 1, 0, 0, 0, 59, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Covert Ops Flying Machine - On Data Set 0 1 - Set Run 1'),
(41382, 0, 1, 2, 61, 0, 100, 0, 0, 0, 0, 0, 0, 69, 0, 0, 0, 0, 0, 0, 8, 0, 0, 0, -5706.56, -1015.85, 410.394, 0, 'Covert Ops Flying Machine - Linked To Id 0 - Move To Pos'),
(41382, 0, 2, 0, 61, 0, 100, 0, 0, 0, 0, 0, 0, 80, 4138200, 2, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Covert Ops Flying Machine - Linked To Id 1 - Run Script'),
(41382, 0, 3, 4, 38, 0, 100, 0, 0, 2, 0, 0, 0, 59, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Covert Ops Flying Machine - On Data Set 0 2 - Set Run 1'),
(41382, 0, 4, 5, 61, 0, 100, 0, 0, 0, 0, 0, 0, 69, 0, 0, 0, 0, 0, 0, 8, 0, 0, 0, -5814.01, -957.724, 404.772, 0, 'Covert Ops Flying Machine - Linked To Id 2 - Move To Pos'),
(41382, 0, 5, 0, 61, 0, 100, 0, 0, 0, 0, 0, 0, 80, 4138201, 2, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Covert Ops Flying Machine - Linked To Id 3 - Run Script');

DELETE FROM `smart_scripts` WHERE `source_type` = 9 AND `entryorguid` IN (4137200, 4137300, 4138200, 4138201);
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `event_param5`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES
(4137200, 9, 0, 0, 0, 0, 100, 0, 500, 500, 0, 0, 0, 45, 0, 1, 0, 0, 0, 0, 11, 41382, 50, 0, 0, 0, 0, 0, 'Frostmane Retreat North - On Script - Set Data 0 1'),
(4137300, 9, 0, 0, 0, 0, 100, 0, 500, 500, 0, 0, 0, 45, 0, 2, 0, 0, 0, 0, 11, 41382, 50, 0, 0, 0, 0, 0, 'Frostmane Retreat South - On Script - Set Data 0 2'),
(4138200, 9, 0, 0, 0, 0, 100, 0, 5000, 5000, 0, 0, 0, 66, 0, 0, 0, 0, 0, 0, 19, 41375, 20, 0, 0, 0, 0, 0, 'Covert Ops Flying Machine - On Script - Set Orientation'),
(4138200, 9, 1, 0, 0, 0, 100, 0, 0, 0, 0, 0, 0, 11, 85915, 0, 0, 0, 0, 0, 19, 41375, 20, 0, 0, 0, 0, 0, 'Covert Ops Flying Machine - On Script - Cast Deathblade Rocket Barrage'),
(4138200, 9, 2, 0, 0, 0, 100, 0, 2000, 2000, 0, 0, 0, 11, 85915, 0, 0, 0, 0, 0, 19, 41375, 20, 0, 0, 0, 0, 0, 'Covert Ops Flying Machine - On Script - Cast Deathblade Rocket Barrage'),
(4138200, 9, 3, 0, 0, 0, 100, 0, 2000, 2000, 0, 0, 0, 11, 85915, 0, 0, 0, 0, 0, 19, 41375, 20, 0, 0, 0, 0, 0, 'Covert Ops Flying Machine - On Script - Cast Deathblade Rocket Barrage'),
(4138200, 9, 4, 0, 0, 0, 100, 0, 2000, 2000, 0, 0, 0, 69, 0, 0, 0, 0, 0, 0, 8, 0, 0, 0, -5738.15, -1052.52, 416.584, 0, 'Covert Ops Flying Machine - On Script - Move To Pos'),
(4138200, 9, 5, 0, 0, 0, 100, 0, 7000, 7000, 0, 0, 0, 41, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Covert Ops Flying Machine - On Script - Despawn'),
(4138201, 9, 0, 0, 0, 0, 100, 0, 5000, 5000, 0, 0, 0, 66, 0, 0, 0, 0, 0, 0, 19, 41375, 20, 0, 0, 0, 0, 0, 'Covert Ops Flying Machine - On Script - Set Orientation'),
(4138201, 9, 1, 0, 0, 0, 100, 0, 0, 0, 0, 0, 0, 11, 85915, 0, 0, 0, 0, 0, 19, 41375, 20, 0, 0, 0, 0, 0, 'Covert Ops Flying Machine - On Script - Cast Deathblade Rocket Barrage'),
(4138201, 9, 2, 0, 0, 0, 100, 0, 2000, 2000, 0, 0, 0, 11, 85915, 0, 0, 0, 0, 0, 19, 41375, 20, 0, 0, 0, 0, 0, 'Covert Ops Flying Machine - On Script - Cast Deathblade Rocket Barrage'),
(4138201, 9, 3, 0, 0, 0, 100, 0, 2000, 2000, 0, 0, 0, 11, 85915, 0, 0, 0, 0, 0, 19, 41375, 20, 0, 0, 0, 0, 0, 'Covert Ops Flying Machine - On Script - Cast Deathblade Rocket Barrage'),
(4138201, 9, 4, 0, 0, 0, 100, 0, 2000, 2000, 0, 0, 0, 69, 0, 0, 0, 0, 0, 0, 8, 0, 0, 0, -5857.79, -978.587, 414.416, 0, 'Covert Ops Flying Machine - On Script - Move To Pos'),
(4138201, 9, 5, 0, 0, 0, 100, 0, 7000, 7000, 0, 0, 0, 41, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Covert Ops Flying Machine - On Script - Despawn');
