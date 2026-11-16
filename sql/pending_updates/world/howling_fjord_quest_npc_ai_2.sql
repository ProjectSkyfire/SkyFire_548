-- Sage Mistwalker (24186, quests 11286, 11317, 11323 and the worg disguise gossip): the template uses gossip menu 8920 where 3.3.5 and 5.4.8
-- use 8908, which holds the "I have misplaced my worg disguise" option the SmartAI listens to. Menu 8908 leads to 8920.
-- Dragonflayer Vrykul Prisoner (24284, quest 11330): Peppy's Special Mix (43386) gives the credit and runs the transformation script.
-- The 5.4.8 row listens to any spell hit, so the spell of the 3.3.5 row is used.
UPDATE `creature_template` SET `gossip_menu_id` = 8908 WHERE `entry` = 24186 AND `gossip_menu_id` = 8920;
-- SmartAI of 24186, 24284
UPDATE `creature_template` SET `AIName` = 'SmartAI' WHERE `entry` IN (24186,24284) AND `ScriptName` = '';
DELETE FROM `smart_scripts` WHERE `source_type` = 0 AND `entryorguid` IN (24186,24284);
DELETE FROM `smart_scripts` WHERE `source_type` = 9 AND `entryorguid` IN (2428400);
INSERT INTO `smart_scripts` (`entryorguid`,`source_type`,`id`,`link`,`event_type`,`event_phase_mask`,`event_chance`,`event_flags`,`event_param1`,`event_param2`,`event_param3`,`event_param4`,`event_param5`,`action_type`,`action_param1`,`action_param2`,`action_param3`,`action_param4`,`action_param5`,`action_param6`,`target_type`,`target_param1`,`target_param2`,`target_param3`,`target_x`,`target_y`,`target_z`,`target_o`,`comment`) VALUES
(24186, 0, 0, 1, 20, 0, 100, 0, 11324, 0, 0, 0, 0, 28, 50102, 0, 0, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0, 'Sage Mistwalker - On Quest Reward Alpha Worg - Remove Aura Ulfang: Force Cast Player Eyes of the Eagle'),
(24186, 0, 1, 2, 61, 0, 100, 0, 0, 0, 0, 0, 0, 28, 43060, 0, 0, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0, 'Sage Mistwalker - Linked with Previous Event - Remove Aura Eyes of the Eagle'),
(24186, 0, 2, 0, 61, 0, 100, 0, 0, 0, 0, 0, 0, 28, 43369, 0, 0, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0, 'Sage Mistwalker - Linked with Previous Event - Remove Aura Worg Disguise'),
(24186, 0, 3, 4, 62, 0, 100, 0, 8908, 0, 0, 0, 0, 85, 43379, 0, 0, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0, 'Sage Mistwalker - On Gossip Option Select - Invoker Cast The Cleansing: Create Worg Disguise'),
(24186, 0, 4, 0, 61, 0, 100, 0, 0, 0, 0, 0, 0, 72, 0, 0, 0, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0, 'Sage Mistwalker - Linked with Previous Event - Close Gossip'),
(24284, 0, 0, 1, 8, 0, 100, 1, 43386, 0, 0, 0, 0, 33, 24284, 0, 0, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0, 'Dragonflayer Vrykul Prisoner - On SpellHit Peppy''s Special Mix - Quest Credit'),
(24284, 0, 1, 0, 61, 0, 100, 0, 0, 0, 0, 0, 0, 80, 2428400, 2, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Dragonflayer Vrykul Prisoner - Linked To Id 0 - Run Script'),
(2428400, 9, 0, 0, 0, 0, 100, 0, 0, 0, 0, 0, 0, 5, 7, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Dragonflayer Vrykul Prisoner - On Script - Play Emote 7'),
(2428400, 9, 1, 0, 0, 0, 100, 0, 3000, 3000, 0, 0, 0, 11, 21862, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Dragonflayer Vrykul Prisoner - On Script - Cast Radiation'),
(2428400, 9, 2, 0, 0, 0, 100, 0, 0, 0, 0, 0, 0, 91, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Dragonflayer Vrykul Prisoner - On Script - Remove Flag Sit'),
(2428400, 9, 3, 0, 0, 0, 100, 0, 2000, 2000, 0, 0, 0, 4, 9110, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Dragonflayer Vrykul Prisoner - On Script - Play Sound'),
(2428400, 9, 4, 0, 0, 0, 100, 0, 0, 0, 0, 0, 0, 5, 53, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Dragonflayer Vrykul Prisoner - On Script - Play Emote 53'),
(2428400, 9, 5, 0, 0, 0, 100, 0, 0, 0, 0, 0, 0, 59, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Dragonflayer Vrykul Prisoner - On Script - Set Run 1'),
(2428400, 9, 6, 0, 0, 0, 100, 0, 1500, 1500, 0, 0, 0, 89, 5, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Dragonflayer Vrykul Prisoner - On Script - Random Move'),
(2428400, 9, 7, 0, 0, 0, 100, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Dragonflayer Vrykul Prisoner - On Script - Say Line 0'),
(2428400, 9, 8, 0, 0, 0, 100, 0, 0, 0, 0, 0, 0, 45, 0, 1, 0, 0, 0, 0, 11, 23842, 30, 0, 0, 0, 0, 0, 'Dragonflayer Vrykul Prisoner - On Script - Set Data 0 1'),
(2428400, 9, 9, 0, 0, 0, 100, 0, 10000, 10000, 0, 0, 0, 3, 0, 1145, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Dragonflayer Vrykul Prisoner - On Script - Morph To Model'),
(2428400, 9, 10, 0, 0, 0, 100, 0, 0, 0, 0, 0, 0, 11, 43401, 2, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Dragonflayer Vrykul Prisoner - On Script - Cast Cosmetic - Blood Explosion Green - Large'),
(2428400, 9, 11, 0, 0, 0, 100, 0, 0, 0, 0, 0, 0, 89, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Dragonflayer Vrykul Prisoner - On Script - Random Move'),
(2428400, 9, 12, 0, 0, 0, 100, 0, 5000, 5000, 0, 0, 0, 45, 0, 2, 0, 0, 0, 0, 11, 23842, 30, 0, 0, 0, 0, 0, 'Dragonflayer Vrykul Prisoner - On Script - Set Data 0 2'),
(2428400, 9, 13, 0, 0, 0, 100, 0, 0, 0, 0, 0, 0, 37, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Dragonflayer Vrykul Prisoner - On Script - Die');

DELETE FROM `creature_text` WHERE `entry` = 24284 AND `groupid` = 0;
INSERT INTO `creature_text` (`entry`, `groupid`, `id`, `text`, `type`, `language`, `probability`, `emote`, `duration`, `sound`, `comment`) VALUES
(24284, 0, 0, 'The vrykul clutches at his throat as he begins to gag and thrash about.', 16, 0, 100, 53, 0, 0, 'Dragonflayer Vrykul Prisoner to Player');
