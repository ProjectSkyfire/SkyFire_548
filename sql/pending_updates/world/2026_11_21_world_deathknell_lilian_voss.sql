-- Deathknell: "The Truth of the Grave" (24961). Lilian Voss shows her reflection when asked, credits the quest and walks
-- a loop, but her script lacked the path (the Start Waypoint actions pointed at no route), her line of text, and the
-- gossip flag handling of the 5.4.8 script. Replace her script with the 5.4.8 rows (the gossip menu stays this
-- database's 11133). Based on Legends of Azeroth 5.4.8.
DELETE FROM `smart_scripts` WHERE (`entryorguid`=38910 AND `source_type`=0) OR (`entryorguid` IN (3891000, 3891001) AND `source_type`=9);
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `event_param5`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES
(38910, 0, 0, 1, 62, 1, 100, 0, 11133, 0, 0, 0, 0, 80, 3891000, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Lilian Voss - On Gossip Option 0 Selected - Run Script (Phase 1)'),
(38910, 0, 1, 8, 61, 1, 100, 0, 11132, 0, 0, 0, 0, 72, 0, 0, 0, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0, 'Lilian Voss - On Gossip Option 0 Selected - Close Gossip (Phase 1)'),
(38910, 0, 2, 3, 62, 2, 100, 0, 11133, 0, 0, 0, 0, 80, 3891001, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Lilian Voss - On Gossip Option 0 Selected - Run Script (Phase 2)'),
(38910, 0, 3, 9, 61, 2, 100, 0, 11132, 0, 0, 0, 0, 72, 0, 0, 0, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0, 'Lilian Voss - On Gossip Option 0 Selected - Close Gossip (Phase 2)'),
(38910, 0, 4, 0, 25, 0, 100, 0, 0, 0, 0, 0, 0, 17, 431, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Lilian Voss - On Reset - Set Emote State (431)'),
(38910, 0, 5, 0, 25, 0, 100, 0, 0, 0, 0, 0, 0, 22, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Lilian Voss - On Reset - Set Event Phase 1'),
(38910, 0, 6, 10, 40, 0, 100, 0, 11, 3891000, 0, 0, 0, 17, 431, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Lilian Voss - On Waypoint 11 Reached - Set Emote State (431)'),
(38910, 0, 7, 10, 40, 0, 100, 0, 11, 3891001, 0, 0, 0, 17, 431, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Lilian Voss - On Waypoint 11 Reached - Set Emote State (431)'),
(38910, 0, 8, 11, 61, 0, 100, 0, 11132, 0, 0, 0, 0, 85, 73210, 0, 0, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0, 'Lilian Voss - On Gossip Option 0 Selected - Invoker Cast ''Show Mirror'' (Phase 1)'),
(38910, 0, 9, 11, 61, 0, 100, 0, 11132, 0, 0, 0, 0, 85, 73210, 0, 0, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0, 'Lilian Voss - On Gossip Option 0 Selected - Invoker Cast ''Show Mirror'' (Phase 1)'),
(38910, 0, 10, 0, 61, 0, 100, 0, 0, 0, 0, 0, 0, 82, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Lilian Voss - Link - Add Npc Flag Gossip'),
(38910, 0, 11, 0, 61, 0, 100, 0, 0, 0, 0, 0, 0, 83, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Lilian Voss - Link - Remove Npc Flag Gossip'),
(3891000, 9, 0, 0, 0, 0, 100, 0, 500, 500, 0, 0, 0, 66, 0, 0, 0, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0, 'Lilian Voss - On Script - Set Orientation To Invoker'),
(3891000, 9, 1, 0, 0, 0, 100, 0, 0, 0, 0, 0, 0, 17, 26, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Lilian Voss - On Script - Set Emote State (26)'),
(3891000, 9, 2, 0, 0, 0, 100, 0, 1000, 1000, 0, 0, 0, 5, 18, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Lilian Voss - On Script - Play Emote (18)'),
(3891000, 9, 3, 0, 0, 0, 100, 0, 3200, 3200, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Lilian Voss - On Script - Say Text Line 0'),
(3891000, 9, 4, 0, 0, 0, 100, 0, 2000, 2000, 0, 0, 0, 33, 38910, 0, 0, 0, 0, 0, 17, 0, 30, 0, 0, 0, 0, 0, 'Lilian Voss - On Script - Quest Credit ''The Truth of the Grave'''),
(3891000, 9, 5, 0, 0, 0, 100, 0, 0, 0, 0, 0, 0, 53, 1, 3891000, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Lilian Voss - On Script - Start Waypoint'),
(3891000, 9, 6, 0, 0, 0, 100, 0, 0, 0, 0, 0, 0, 22, 2, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Lilian Voss - On Script - Set Event Phase 2'),
(3891001, 9, 0, 0, 0, 0, 100, 0, 500, 500, 0, 0, 0, 66, 0, 0, 0, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0, 'Lilian Voss - On Script - Set Orientation To Invoker'),
(3891001, 9, 1, 0, 0, 0, 100, 0, 0, 0, 0, 0, 0, 17, 26, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Lilian Voss - On Script - Set Emote State (26)'),
(3891001, 9, 2, 0, 0, 0, 100, 0, 1000, 1000, 0, 0, 0, 5, 18, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Lilian Voss - On Script - Play Emote (18)'),
(3891001, 9, 3, 0, 0, 0, 100, 0, 3200, 3200, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Lilian Voss - On Script - Say Text Line 0'),
(3891001, 9, 4, 0, 0, 0, 100, 0, 2000, 2000, 0, 0, 0, 33, 38910, 0, 0, 0, 0, 0, 17, 0, 30, 0, 0, 0, 0, 0, 'Lilian Voss - On Script - Quest Credit ''The Truth of the Grave'''),
(3891001, 9, 5, 0, 0, 0, 100, 0, 0, 0, 0, 0, 0, 53, 1, 3891001, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Lilian Voss - On Script - Start Waypoint'),
(3891001, 9, 6, 0, 0, 0, 100, 0, 0, 0, 0, 0, 0, 22, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Lilian Voss - On Script - Set Event Phase 1');

DELETE FROM `waypoints` WHERE `entry` IN (3891000, 3891001);
INSERT INTO `waypoints` (`entry`, `pointid`, `position_x`, `position_y`, `position_z`, `point_comment`) VALUES
(3891000, 1, 1857.42, 1561.47, 99.079, 'Lilian Voss'),
(3891000, 2, 1864.89, 1574.94, 99.0361, 'Lilian Voss'),
(3891000, 3, 1868.91, 1575.56, 97.5441, 'Lilian Voss'),
(3891000, 4, 1875.28, 1574.12, 94.314, 'Lilian Voss'),
(3891000, 5, 1875.91, 1572.36, 94.314, 'Lilian Voss'),
(3891000, 6, 1873, 1570.42, 94.314, 'Lilian Voss'),
(3891000, 7, 1868.25, 1568.28, 94.314, 'Lilian Voss'),
(3891000, 8, 1864.41, 1567.87, 94.314, 'Lilian Voss'),
(3891000, 9, 1861.8, 1563.51, 94.3124, 'Lilian Voss'),
(3891000, 10, 1860.59, 1558.04, 94.782, 'Lilian Voss'),
(3891000, 11, 1857.21, 1555.61, 94.7926, 'Lilian Voss'),
(3891001, 1, 1860.59, 1558.04, 94.782, 'Lilian Voss'),
(3891001, 2, 1861.8, 1563.51, 94.3124, 'Lilian Voss'),
(3891001, 3, 1864.41, 1567.87, 94.314, 'Lilian Voss'),
(3891001, 4, 1868.25, 1568.28, 94.314, 'Lilian Voss'),
(3891001, 5, 1873, 1570.42, 94.314, 'Lilian Voss'),
(3891001, 6, 1875.91, 1572.36, 94.314, 'Lilian Voss'),
(3891001, 7, 1875.28, 1574.12, 94.314, 'Lilian Voss'),
(3891001, 8, 1868.91, 1575.56, 97.5441, 'Lilian Voss'),
(3891001, 9, 1864.89, 1574.94, 99.0361, 'Lilian Voss'),
(3891001, 10, 1857.42, 1561.47, 99.079, 'Lilian Voss'),
(3891001, 11, 1854.83, 1555.63, 99.07, 'Lilian Voss');

DELETE FROM `creature_text` WHERE `entry`=38910;
INSERT INTO `creature_text` (`entry`, `groupid`, `id`, `text`, `type`, `language`, `probability`, `emote`, `duration`, `sound`, `comment`) VALUES
(38910, 0, 0, 'You don''t understand... I CAN''T be undead! Not me, not now...', 12, 0, 100, 0, 0, 0, 'Lilian Voss - shown her reflection');
