-- Spared from Madness (26736): the objectives are the rescue of Osborn Obnoticus, Emerine Junis and Berrin Burnquill from
-- the cage in Kurzen's compound. The three prisoners (43919, 43920, 43921) had no spawn, the Kurzen Cage (204828) had
-- no script and the prisoners had no AI, so the cage held nothing and gave no credit.
-- Opening the cage gives the invoker the three credits and tells the prisoners (data 1), who walk a short path out of the
-- cage, speak, and Emerine casts Teleport to Rebel Camp (82223) and leaves. Rows, paths, speech and spawns are as in
-- Legends of Azeroth 5.4.8, with these differences: the paths carry no quest id (the credit comes from the cage, and the
-- quest id would make the escort code check for the player), and Emerine tells the other two prisoners (data 2) to leave
-- with her, because the spell has no area to hit in this data (its radius is empty), so they would stay behind.
-- The cage has three stacked spawns here (guids 1162, 3235 and 5422); this file does not remove them.

UPDATE `creature_template` SET `AIName` = 'SmartAI' WHERE `entry` IN (43919, 43920, 43921) AND `ScriptName` = '';
UPDATE `gameobject_template` SET `AIName` = 'SmartGameObjectAI' WHERE `entry` = 204828 AND `ScriptName` = '';

DELETE FROM `creature` WHERE `guid` BETWEEN 9310511 AND 9310513;
INSERT INTO `creature` (`guid`, `id`, `map`, `spawnMask`, `phaseId`, `phaseGroup`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `MovementType`, `npcflag`, `unit_flags`, `dynamicflags`) VALUES
(9310511, 43919, 0, 1, 0, 0, 0, 0, -11571.1, -566.815, 33.6846, 3.2821, 60, 0, 0, 0, 0, 0, 0, 0, 0),
(9310512, 43920, 0, 1, 0, 0, 0, 0, -11571, -568.511, 33.6846, 3.27033, 60, 0, 0, 0, 0, 0, 0, 0, 0),
(9310513, 43921, 0, 1, 0, 0, 0, 0, -11570.9, -570.906, 33.6831, 2.98365, 60, 0, 0, 0, 0, 0, 0, 0, 0);

DELETE FROM `waypoints` WHERE `entry` IN (43919, 43920, 43921);
INSERT INTO `waypoints` (`entry`, `pointid`, `position_x`, `position_y`, `position_z`, `point_comment`) VALUES
(43919, 1, -11571.2, -568.362, 33.8483, 'Osborn Obnoticus'),
(43919, 2, -11571.1, -570.538, 33.7655, 'Osborn Obnoticus'),
(43919, 3, -11571, -572.714, 33.6828, 'Osborn Obnoticus'),
(43920, 1, -11571.1, -570.538, 33.7655, 'Emerine Junis'),
(43920, 2, -11569.3, -573.601, 33.6826, 'Emerine Junis'),
(43921, 1, -11573, -573.91, 33.6818, 'Berrin Burnquill');

DELETE FROM `smart_scripts` WHERE `source_type` = 0 AND `entryorguid` = 43919;
DELETE FROM `smart_scripts` WHERE `source_type` = 0 AND `entryorguid` = 43920;
DELETE FROM `smart_scripts` WHERE `source_type` = 0 AND `entryorguid` = 43921;
DELETE FROM `smart_scripts` WHERE `source_type` = 1 AND `entryorguid` = 204828;

INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `event_param5`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES
(43919, 0, 0, 0, 38, 0, 100, 0, 0, 1, 0, 0, 0, 53, 0, 43919, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Osborn Obnoticus - On Data Set 0 1 - Waypoint Start'),
(43919, 0, 1, 0, 58, 0, 100, 0, 3, 43919, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Osborn Obnoticus - On Waypoint Ended - Say Line 0'),
(43919, 0, 2, 0, 8, 0, 100, 0, 82223, 0, 0, 0, 0, 41, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Osborn Obnoticus - On SpellHit Teleport to Rebel Camp - Despawn'),
(43919, 0, 3, 0, 38, 0, 100, 0, 0, 2, 0, 0, 0, 41, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Osborn Obnoticus - On Data Set 0 2 - Despawn'),
(43920, 0, 0, 0, 38, 0, 100, 0, 0, 1, 0, 0, 0, 53, 0, 43920, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Emerine Junis - On Data Set 0 1 - Waypoint Start'),
(43920, 0, 1, 2, 58, 0, 100, 0, 2, 43920, 0, 0, 0, 1, 0, 5000, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Emerine Junis - On Waypoint Ended - Say Line 0'),
(43920, 0, 2, 0, 61, 0, 100, 0, 0, 0, 0, 0, 0, 66, 3, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Emerine Junis - Linked To Id 1 - Set Orientation'),
(43920, 0, 3, 0, 52, 0, 100, 0, 0, 43920, 0, 0, 0, 1, 1, 3000, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Emerine Junis - On Text Over 0 - Say Line 1'),
(43920, 0, 4, 0, 52, 0, 100, 0, 1, 43920, 0, 0, 0, 1, 2, 2000, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Emerine Junis - On Text Over 1 - Say Line 2'),
(43920, 0, 5, 6, 52, 0, 100, 0, 2, 43920, 0, 0, 0, 11, 82223, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Emerine Junis - On Text Over 2 - Cast Teleport to Rebel Camp'),
(43920, 0, 6, 7, 61, 0, 100, 0, 0, 0, 0, 0, 0, 45, 0, 2, 0, 0, 0, 0, 19, 43919, 30, 0, 0, 0, 0, 0, 'Emerine Junis - Linked To Id 5 - Set Data 0 2 On Osborn Obnoticus'),
(43920, 0, 7, 8, 61, 0, 100, 0, 0, 0, 0, 0, 0, 45, 0, 2, 0, 0, 0, 0, 19, 43921, 30, 0, 0, 0, 0, 0, 'Emerine Junis - Linked To Id 6 - Set Data 0 2 On Berrin Burnquill'),
(43920, 0, 8, 0, 61, 0, 100, 0, 0, 0, 0, 0, 0, 41, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Emerine Junis - Linked To Id 7 - Despawn'),
(43921, 0, 0, 0, 38, 0, 100, 0, 0, 1, 0, 0, 0, 53, 0, 43921, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Berrin Burnquill - On Data Set 0 1 - Waypoint Start'),
(43921, 0, 1, 2, 58, 0, 100, 0, 1, 43921, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Berrin Burnquill - On Waypoint Ended - Say Line 0'),
(43921, 0, 2, 3, 61, 0, 100, 0, 0, 0, 0, 0, 0, 66, 0, 0, 0, 0, 0, 0, 19, 43920, 5, 0, 0, 0, 0, 0, 'Berrin Burnquill - Linked To Id 1 - Set Orientation'),
(43921, 0, 3, 0, 61, 0, 100, 0, 0, 0, 0, 0, 0, 17, 10, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Berrin Burnquill - Linked To Id 2 - Set Emote State 17'),
(43921, 0, 4, 5, 8, 0, 100, 0, 82223, 0, 0, 0, 0, 17, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Berrin Burnquill - On SpellHit Teleport to Rebel Camp - Set Emote State 0'),
(43921, 0, 5, 0, 61, 0, 100, 0, 0, 0, 0, 0, 0, 41, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Berrin Burnquill - Linked To Id 4 - Despawn'),
(43921, 0, 6, 0, 38, 0, 100, 0, 0, 2, 0, 0, 0, 41, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Berrin Burnquill - On Data Set 0 2 - Despawn'),
(204828, 1, 0, 1, 70, 0, 100, 0, 2, 0, 0, 0, 0, 33, 43919, 0, 0, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0, 'Kurzen Cage - On GO State Change - Quest Credit'),
(204828, 1, 1, 2, 61, 0, 100, 0, 0, 0, 0, 0, 0, 33, 43920, 0, 0, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0, 'Kurzen Cage - Linked To Id 0 - Quest Credit'),
(204828, 1, 2, 3, 61, 0, 100, 0, 0, 0, 0, 0, 0, 33, 43921, 0, 0, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0, 'Kurzen Cage - Linked To Id 1 - Quest Credit'),
(204828, 1, 3, 4, 61, 0, 100, 0, 0, 0, 0, 0, 0, 45, 0, 1, 0, 0, 0, 0, 19, 43919, 10, 0, 0, 0, 0, 0, 'Kurzen Cage - Linked To Id 2 - Set Data 0 1'),
(204828, 1, 4, 5, 61, 0, 100, 0, 0, 0, 0, 0, 0, 45, 0, 1, 0, 0, 0, 0, 19, 43920, 10, 0, 0, 0, 0, 0, 'Kurzen Cage - Linked To Id 3 - Set Data 0 1'),
(204828, 1, 5, 0, 61, 0, 100, 0, 0, 0, 0, 0, 0, 45, 0, 1, 0, 0, 0, 0, 19, 43921, 10, 0, 0, 0, 0, 0, 'Kurzen Cage - Linked To Id 4 - Set Data 0 1');

DELETE FROM `creature_text` WHERE `entry` IN (43919, 43920, 43921);
INSERT INTO `creature_text` (`entry`, `groupid`, `id`, `text`, `type`, `language`, `probability`, `emote`, `duration`, `sound`, `comment`) VALUES
(43919, 0, 0, 'Woo hoo.', 12, 0, 100, 0, 0, 0, 'Osborn Obnoticus'),
(43920, 0, 0, 'Thank you!  I don''t know how much longer we would have survived.', 12, 0, 100, 0, 0, 0, 'Emerine Junis'),
(43920, 1, 0, 'That cage was stifling my magic.', 12, 0, 100, 0, 0, 0, 'Emerine Junis'),
(43920, 2, 0, 'If you don''t mind, I''m going to teleport us back to the Rebel Camp.  We''ll thank you in person later.', 12, 0, 100, 0, 0, 0, 'Emerine Junis'),
(43921, 0, 0, 'Freedom!  Sweet, sweet freedom!', 12, 0, 100, 0, 0, 0, 'Berrin Burnquill');
