-- Vashj'ir, "A Better Vantage" (25955), "Gauging Success" (25965) and "Visions of the Past: Rise from the Deep" (26135): the
-- objectives are creature credits (40963 to 40965, 41304, 41303, 41982) that have no spawn and no spell, so the three quests
-- could not be completed. The client data has an area trigger at each place the quest points at (the quest map blobs of the
-- 5.4.8 data lie in or next to the triggers): the sphere at the tunnel west of the gardens (5882), the sphere at the northern gardens
-- (5884), the box over the southern structures (5883), the box at the tunnel to the north (5923), the sphere on the
-- north-western terrace (5924) and the narrow box at the temple of Quel'Dormir (5971). Entering one credits the objective to the
-- player; the Alliance twins (25582, 25754, 25626) share the same credits. The second objective of 26135 (the vision with the
-- Battlemaiden's blade) is not touched.

DELETE FROM `areatrigger_scripts` WHERE `entry` IN (5882, 5883, 5884, 5923, 5924, 5971);
INSERT INTO `areatrigger_scripts` (`entry`, `ScriptName`) VALUES
(5882, 'SmartTrigger'),
(5883, 'SmartTrigger'),
(5884, 'SmartTrigger'),
(5923, 'SmartTrigger'),
(5924, 'SmartTrigger'),
(5971, 'SmartTrigger');

DELETE FROM `smart_scripts` WHERE `source_type` = 2 AND `entryorguid` IN (5882, 5883, 5884, 5923, 5924, 5971);
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `event_param5`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES
(5882, 2, 0, 0, 46, 0, 100, 0, 5882, 0, 0, 0, 0, 33, 40964, 0, 0, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0, 'Areatrigger 5882 - On Trigger - Quest Credit Tunnel West of the Gardens'),
(5883, 2, 0, 0, 46, 0, 100, 0, 5883, 0, 0, 0, 0, 33, 40965, 0, 0, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0, 'Areatrigger 5883 - On Trigger - Quest Credit Structures South of the Gardens'),
(5884, 2, 0, 0, 46, 0, 100, 0, 5884, 0, 0, 0, 0, 33, 40963, 0, 0, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0, 'Areatrigger 5884 - On Trigger - Quest Credit Northern Gardens'),
(5923, 2, 0, 0, 46, 0, 100, 0, 5923, 0, 0, 0, 0, 33, 41304, 0, 0, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0, 'Areatrigger 5923 - On Trigger - Quest Credit Tunnel to the North'),
(5924, 2, 0, 0, 46, 0, 100, 0, 5924, 0, 0, 0, 0, 33, 41303, 0, 0, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0, 'Areatrigger 5924 - On Trigger - Quest Credit Northwestern Terrace'),
(5971, 2, 0, 0, 46, 0, 100, 0, 5971, 0, 0, 0, 0, 33, 41982, 0, 0, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0, 'Areatrigger 5971 - On Trigger - Quest Credit Temple of Quel''Dormir');
