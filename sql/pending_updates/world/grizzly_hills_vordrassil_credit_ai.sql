-- Grizzly Hills: quest credit given by a spell hit on the Vordrassil creatures, and Mack Fearsen's grog gossip and
-- reward lines. The creatures had no SmartAI here.
UPDATE `creature_template` SET `AIName` = 'SmartAI' WHERE `entry` IN (27263,27264,27265,26604) AND `ScriptName` = '';
DELETE FROM `smart_scripts` WHERE `source_type` = 0 AND `entryorguid` IN (27263,27264,27265,26604);
INSERT INTO `smart_scripts` (`entryorguid`,`source_type`,`id`,`link`,`event_type`,`event_phase_mask`,`event_chance`,`event_flags`,`event_param1`,`event_param2`,`event_param3`,`event_param4`,`event_param5`,`action_type`,`action_param1`,`action_param2`,`action_param3`,`action_param4`,`action_param5`,`action_param6`,`target_type`,`target_param1`,`target_param2`,`target_param3`,`target_x`,`target_y`,`target_z`,`target_o`,`comment`) VALUES
(27263, 0, 0, 0, 8, 0, 100, 0, 48218, 0, 0, 0, 0, 33, 27263, 0, 0, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0, 'Vordrassil''s Heart Credit - On Spellhit ''Sampling Energy'' - Quest Credit ''The Darkness Beneath'' (Phase 1) (No Repeat)'),
(27264, 0, 0, 0, 8, 0, 100, 0, 48218, 0, 0, 0, 0, 33, 27264, 0, 0, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0, 'Vordrassil''s Limb Credit - On Spellhit ''Sampling Energy'' - Quest Credit ''The Darkness Beneath'' (Phase 1) (No Repeat)'),
(27265, 0, 0, 0, 8, 0, 100, 0, 48218, 0, 0, 0, 0, 33, 27265, 0, 0, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0, 'Vordrassil''s Tears Credit - On Spellhit ''Sampling Energy'' - Quest Credit ''The Darkness Beneath'' (Phase 1) (No Repeat)'),
(26604, 0, 0, 1, 62, 0, 100, 0, 9416, 0, 0, 0, 0, 85, 47244, 2, 0, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0, 'Mack Fearsen - On Gossip Option 0 Selected - Invoker cast Create Macks Dark Grog'),
(26604, 0, 1, 0, 61, 0, 100, 0, 0, 0, 0, 0, 0, 72, 0, 0, 0, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0, 'Mack Fearsen - Link - Close Gossip'),
(26604, 0, 2, 0, 20, 0, 100, 0, 12484, 0, 0, 0, 0, 1, 0, 10000, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Mack Fearsen - On Quest Complete (Scourgekabob)  - Say Line 0'),
(26604, 0, 3, 0, 52, 0, 100, 0, 0, 26604, 0, 0, 0, 1, 1, 10000, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Mack Fearsen - On Text Over line 0 - Say Line 1'),
(26604, 0, 4, 0, 52, 0, 100, 0, 1, 26604, 0, 0, 0, 1, 2, 10000, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Mack Fearsen - On Text Over line 1 - Say Line 2');
DELETE FROM `creature_text` WHERE `entry` = 26604 AND `groupid` IN (0,1,2);
INSERT INTO `creature_text` (`entry`, `groupid`, `id`, `text`, `type`, `language`, `probability`, `emote`, `duration`, `sound`, `comment`) VALUES
(26604, 0, 0, 'Shcourged trooolls roashting....', 12, 0, 100, 396, 0, 0, 'Mack Fearsen'),
(26604, 1, 0, '...on an open fire.....', 12, 0, 100, 396, 0, 0, 'Mack Fearsen'),
(26604, 2, 0, 'That''s it! Far as I got... Whatd''ya think?', 12, 0, 100, 396, 0, 0, 'Mack Fearsen');
