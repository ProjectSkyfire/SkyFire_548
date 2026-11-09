-- Seeking Soil (28220), Singin' in the Sun (28222) and Rejoining the Forest (28228): the Irontree credits had no
-- source. Accepting each quest from its protector summons the matching Protector (spells 89338, 89356 and 89357),
-- which follows the player. The Seedling Protector reaching a Rich Soil credit bunny, the Sapling Protector a Sunbeam
-- credit bunny, and the Flourishing Protector the hill (Irontree Woods Island) credit the owner. The bunnies stand
-- at the Rich Irontree Soil and Shaft of Sunlight positions of the Cataclysm Preservation Project 4.3.4 spawns.
UPDATE `creature_template` SET `AIName` = 'SmartAI' WHERE `entry` IN (48032, 48042, 48044);

DELETE FROM `creature` WHERE `guid` BETWEEN 9256810 AND 9256829;
INSERT INTO `creature` (`guid`, `id`, `map`, `spawnMask`, `phaseId`, `phaseGroup`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `MovementType`, `npcflag`, `unit_flags`, `dynamicflags`) VALUES
(9256810, 48164, 1, 1, 0, 0, 0, 0, 6072.9, -1090.45, 382.295, 0, 60, 0, 0, 0, 0, 0, 0, 33555200, 0),
(9256811, 48164, 1, 1, 0, 0, 0, 0, 6123.02, -1113.98, 376.876, 0, 60, 0, 0, 0, 0, 0, 0, 33555200, 0),
(9256812, 48164, 1, 1, 0, 0, 0, 0, 6099.55, -1155.86, 373.044, 0, 60, 0, 0, 0, 0, 0, 0, 33555200, 0),
(9256813, 48164, 1, 1, 0, 0, 0, 0, 6057.64, -1129.74, 380.679, 0, 60, 0, 0, 0, 0, 0, 0, 33555200, 0),
(9256814, 48164, 1, 1, 0, 0, 0, 0, 6065.7, -1195.44, 376.086, 0, 60, 0, 0, 0, 0, 0, 0, 33555200, 0),
(9256815, 48164, 1, 1, 0, 0, 0, 0, 5976.07, -1118.98, 380.205, 0, 60, 0, 0, 0, 0, 0, 0, 33555200, 0),
(9256816, 48164, 1, 1, 0, 0, 0, 0, 6016.95, -1233.57, 376.616, 0, 60, 0, 0, 0, 0, 0, 0, 33555200, 0),
(9256817, 48164, 1, 1, 0, 0, 0, 0, 6104.25, -1247.72, 375.481, 0, 60, 0, 0, 0, 0, 0, 0, 33555200, 0),
(9256818, 48164, 1, 1, 0, 0, 0, 0, 5997.8, -1194.58, 376.645, 0, 60, 0, 0, 0, 0, 0, 0, 33555200, 0),
(9256819, 48200, 1, 1, 0, 0, 0, 0, 6282.77, -1123.01, 371.131, 0, 60, 0, 0, 0, 0, 0, 0, 33555200, 0),
(9256820, 48200, 1, 1, 0, 0, 0, 0, 6369.22, -1327.92, 375.717, 0, 60, 0, 0, 0, 0, 0, 0, 33555200, 0),
(9256821, 48200, 1, 1, 0, 0, 0, 0, 6342.94, -1256.97, 374.217, 0, 60, 0, 0, 0, 0, 0, 0, 33555200, 0),
(9256822, 48200, 1, 1, 0, 0, 0, 0, 6416.35, -1307.89, 380.111, 0, 60, 0, 0, 0, 0, 0, 0, 33555200, 0),
(9256823, 48200, 1, 1, 0, 0, 0, 0, 6393.17, -1155.77, 387.513, 0, 60, 0, 0, 0, 0, 0, 0, 33555200, 0),
(9256824, 48200, 1, 1, 0, 0, 0, 0, 6456.61, -1182.84, 401.868, 0, 60, 0, 0, 0, 0, 0, 0, 33555200, 0),
(9256825, 48200, 1, 1, 0, 0, 0, 0, 6272.73, -1341.75, 371.245, 0, 60, 0, 0, 0, 0, 0, 0, 33555200, 0),
(9256826, 48200, 1, 1, 0, 0, 0, 0, 6232.03, -1361.31, 376.999, 0, 60, 0, 0, 0, 0, 0, 0, 33555200, 0),
(9256827, 48200, 1, 1, 0, 0, 0, 0, 6287.6, -1369.11, 370.53, 0, 60, 0, 0, 0, 0, 0, 0, 33555200, 0),
(9256828, 48200, 1, 1, 0, 0, 0, 0, 6173.76, -1373.54, 385.671, 0, 60, 0, 0, 0, 0, 0, 0, 33555200, 0),
(9256829, 48200, 1, 1, 0, 0, 0, 0, 6211.91, -1290.37, 374.419, 0, 60, 0, 0, 0, 0, 0, 0, 33555200, 0);

DELETE FROM `smart_scripts` WHERE `source_type` = 0 AND `entryorguid` IN (48032, 48042, 48044);
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `event_param5`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES
(48032, 0, 0, 0, 19, 0, 100, 0, 28220, 0, 0, 0, 0, 85, 89338, 2, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Seedling Protector - On Quest ''Seeking Soil'' Accepted - Invoker Cast Summon Seedling Protector'),
(48032, 0, 1, 0, 19, 0, 100, 0, 28222, 0, 0, 0, 0, 85, 89356, 2, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Seedling Protector - On Quest ''Singin'' in the Sun'' Accepted - Invoker Cast Summon Sapling Protector'),
(48032, 0, 2, 0, 54, 0, 100, 0, 0, 0, 0, 0, 0, 22, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Seedling Protector - On Just Summoned - Set Event Phase 1'),
(48032, 0, 3, 4, 75, 1, 100, 0, 0, 48164, 8, 2000, 0, 33, 48164, 0, 0, 0, 0, 0, 23, 0, 0, 0, 0, 0, 0, 0, 'Seedling Protector - Within 8 Yards of Irontree Soil Credit - Quest Credit Irontree Soil Credit to Owner'),
(48032, 0, 4, 0, 61, 0, 100, 0, 0, 0, 0, 0, 0, 41, 1000, 0, 0, 0, 0, 0, 19, 48164, 10, 0, 0, 0, 0, 0, 'Seedling Protector - Linked - Despawn Irontree Soil Credit'),
(48042, 0, 0, 0, 19, 0, 100, 0, 28222, 0, 0, 0, 0, 85, 89356, 2, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Sapling Protector - On Quest ''Singin'' in the Sun'' Accepted - Invoker Cast Summon Sapling Protector'),
(48042, 0, 1, 0, 54, 0, 100, 0, 0, 0, 0, 0, 0, 22, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Sapling Protector - On Just Summoned - Set Event Phase 1'),
(48042, 0, 2, 3, 75, 1, 100, 0, 0, 48200, 8, 2000, 0, 33, 48200, 0, 0, 0, 0, 0, 23, 0, 0, 0, 0, 0, 0, 0, 'Sapling Protector - Within 8 Yards of Sunbeam Credit - Quest Credit Sunbeam Credit to Owner'),
(48042, 0, 3, 0, 61, 0, 100, 0, 0, 0, 0, 0, 0, 41, 1000, 0, 0, 0, 0, 0, 19, 48200, 10, 0, 0, 0, 0, 0, 'Sapling Protector - Linked - Despawn Sunbeam Credit'),
(48044, 0, 0, 0, 19, 0, 100, 0, 28228, 0, 0, 0, 0, 85, 89357, 2, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Flourishing Protector - On Quest ''Rejoining the Forest'' Accepted - Invoker Cast Summon Flourishing Protector'),
(48044, 0, 1, 0, 54, 0, 100, 0, 0, 0, 0, 0, 0, 22, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Flourishing Protector - On Just Summoned - Set Event Phase 1'),
(48044, 0, 2, 3, 75, 1, 100, 0, 0, 48077, 8, 2000, 0, 15, 28228, 0, 0, 0, 0, 0, 23, 0, 0, 0, 0, 0, 0, 0, 'Flourishing Protector - Within 8 Yards of Irontree Woods Island - Area Explored Rejoining the Forest'),
(48044, 0, 3, 0, 61, 0, 100, 0, 0, 0, 0, 0, 0, 41, 3000, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Flourishing Protector - Linked - Despawn');
