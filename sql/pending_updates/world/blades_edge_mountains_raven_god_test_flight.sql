-- Blade's Edge Mountains: Whispers of the Raven God (the four Prophecy objects, the Voice of the Raven God and the
-- Ravenspeech aura the Grishna ogres give on death) and Tally Zapnabber's waiver of the Test Flight quests.

-- Whispers of the Raven God and Test Flight
DELETE FROM `smart_scripts` WHERE `source_type` = 0 AND `entryorguid` IN (19988,19989,19990,21460,21851);
INSERT INTO `smart_scripts` (`entryorguid`,`source_type`,`id`,`link`,`event_type`,`event_phase_mask`,`event_chance`,`event_flags`,`event_param1`,`event_param2`,`event_param3`,`event_param4`,`event_param5`,`action_type`,`action_param1`,`action_param2`,`action_param3`,`action_param4`,`action_param5`,`action_param6`,`target_type`,`target_param1`,`target_param2`,`target_param3`,`target_x`,`target_y`,`target_z`,`target_o`,`comment`) VALUES
(19988,0,0,0,6,0,100,0,0,0,0,0,0,85,37466,0,0,0,0,0,7,0,0,0,0,0,0,0,'Grishna Falconwing - On Death - Invoker Cast Understanding Ravenspeech'),
(19989,0,0,0,6,0,100,0,0,0,0,0,0,85,37466,0,0,0,0,0,7,0,0,0,0,0,0,0,'Grishna Harbinger - On Death - Invoker Cast Understanding Ravenspeech'),
(19990,0,0,0,6,0,100,0,0,0,0,0,0,85,37466,0,0,0,0,0,7,0,0,0,0,0,0,0,'Grishna Scorncrow - On Death - Invoker Cast Understanding Ravenspeech'),
(21460,0,0,1,62,0,100,0,8303,0,0,0,0,56,30540,1,0,0,0,0,7,0,0,0,0,0,0,0,'Tally Zapnabber - On Gossip Option 0 Selected - Add Item ''Tally''s Waiver (Unsigned)'' 1 Time'),
(21460,0,1,0,61,0,100,0,0,0,0,0,0,72,0,0,0,0,0,0,7,0,0,0,0,0,0,0,'Tally Zapnabber - On Gossip Option 0 Selected - Close Gossip'),
(21851,0,0,0,38,0,100,0,0,1,0,0,0,1,0,0,0,0,0,0,12,1,0,0,0,0,0,0,'The Voice of the Raven God - On Data Set 0 1 - Say Line 0'),
(21851,0,1,0,38,0,100,0,0,2,0,0,0,1,1,0,0,0,0,0,12,1,0,0,0,0,0,0,'The Voice of the Raven God - On Data Set 0 2 - Say Line 1'),
(21851,0,2,0,38,0,100,0,0,3,0,0,0,1,2,0,0,0,0,0,12,1,0,0,0,0,0,0,'The Voice of the Raven God - On Data Set 0 3 - Say Line 2'),
(21851,0,3,0,38,0,100,0,0,4,0,0,0,1,3,0,0,0,0,0,12,1,0,0,0,0,0,0,'The Voice of the Raven God - On Data Set 0 4 - Say Line 3');
DELETE FROM `creature_text` WHERE `entry` IN (21851);
INSERT INTO `creature_text` (`entry`,`groupid`,`id`,`text`,`type`,`language`,`probability`,`emote`,`duration`,`sound`,`comment`) VALUES
(21851,0,0,'From the darkest night shall rise again the raven, shall take flight in the shadows, shall reveal the nature of its kind. Prepare yourself for its coming, for the faithful shall be elevated to take flight with the raven, the rest be forgotten to walk upon the ground, clipped wings and shame.',15,0,100,0,0,0,'The Voice of the Raven God'),
(21851,1,0,'Steel your minds and guard your thoughts. The dark wings will cloud and consume the minds of the weak, a flock of thralls whose feet may never leave the ground.',15,0,100,0,0,0,'The Voice of the Raven God'),
(21851,2,0,'The old blood will flow once again with the coming of the raven, the return of the darkness in the skies. Scarlet night, and the rise of the old.',15,0,100,0,0,0,'The Voice of the Raven God'),
(21851,3,0,'The raven was struck down once for flying too high, unready. The eons have prepared the Dark Watcher for its ascent, to draw the dark cloak across the horizon.',15,0,100,0,0,0,'The Voice of the Raven God');
UPDATE `creature_template` SET `AIName` = 'SmartAI' WHERE `entry` IN (19988,19989,19990,21460,21851) AND `AIName` = '' AND `ScriptName` = '';

-- The four Prophecy objects
DELETE FROM `smart_scripts` WHERE `source_type` = 1 AND `entryorguid` IN (184950,184967,184968,184969);
INSERT INTO `smart_scripts` (`entryorguid`,`source_type`,`id`,`link`,`event_type`,`event_phase_mask`,`event_chance`,`event_flags`,`event_param1`,`event_param2`,`event_param3`,`event_param4`,`event_param5`,`action_type`,`action_param1`,`action_param2`,`action_param3`,`action_param4`,`action_param5`,`action_param6`,`target_type`,`target_param1`,`target_param2`,`target_param3`,`target_x`,`target_y`,`target_z`,`target_o`,`comment`) VALUES
(184950,1,0,1,70,0,100,0,2,0,0,0,0,33,22798,0,0,0,0,0,7,0,0,0,0,0,0,0,'The First Prophecy - On GO State Change - Quest Credit'),
(184950,1,1,2,61,0,100,0,0,0,0,0,0,64,1,0,0,0,0,0,7,0,0,0,0,0,0,0,'The First Prophecy - Linked To Id 0 - Store Target List'),
(184950,1,2,3,61,0,100,0,0,0,0,0,0,100,1,0,0,0,0,0,19,21851,5,0,0,0,0,0,'The First Prophecy - Linked To Id 1 - Send Target To Target'),
(184950,1,3,0,61,0,100,0,0,0,0,0,0,45,0,1,0,0,0,0,19,21851,5,0,0,0,0,0,'The First Prophecy - Linked To Id 2 - Set Data 0 1'),
(184967,1,0,1,70,0,100,0,2,0,0,0,0,33,22799,0,0,0,0,0,7,0,0,0,0,0,0,0,'The Second Prophecy - On GO State Change - Quest Credit'),
(184967,1,1,2,61,0,100,0,0,0,0,0,0,64,1,0,0,0,0,0,7,0,0,0,0,0,0,0,'The Second Prophecy - Linked To Id 0 - Store Target List'),
(184967,1,2,3,61,0,100,0,0,0,0,0,0,100,1,0,0,0,0,0,19,21851,5,0,0,0,0,0,'The Second Prophecy - Linked To Id 1 - Send Target To Target'),
(184967,1,3,0,61,0,100,0,0,0,0,0,0,45,0,2,0,0,0,0,19,21851,5,0,0,0,0,0,'The Second Prophecy - Linked To Id 2 - Set Data 0 2'),
(184968,1,0,1,70,0,100,0,2,0,0,0,0,33,22800,0,0,0,0,0,7,0,0,0,0,0,0,0,'The Third Prophecy - On GO State Change - Quest Credit'),
(184968,1,1,2,61,0,100,0,0,0,0,0,0,64,1,0,0,0,0,0,7,0,0,0,0,0,0,0,'The Third Prophecy - Linked To Id 0 - Store Target List'),
(184968,1,2,3,61,0,100,0,0,0,0,0,0,100,1,0,0,0,0,0,19,21851,5,0,0,0,0,0,'The Third Prophecy - Linked To Id 1 - Send Target To Target'),
(184968,1,3,0,61,0,100,0,0,0,0,0,0,45,0,3,0,0,0,0,19,21851,5,0,0,0,0,0,'The Third Prophecy - Linked To Id 2 - Set Data 0 3'),
(184969,1,0,1,70,0,100,0,2,0,0,0,0,33,22801,0,0,0,0,0,7,0,0,0,0,0,0,0,'The Fourth Prophecy - On GO State Change - Quest Credit'),
(184969,1,1,2,61,0,100,0,0,0,0,0,0,64,1,0,0,0,0,0,7,0,0,0,0,0,0,0,'The Fourth Prophecy - Linked To Id 0 - Store Target List'),
(184969,1,2,3,61,0,100,0,0,0,0,0,0,100,1,0,0,0,0,0,19,21851,5,0,0,0,0,0,'The Fourth Prophecy - Linked To Id 1 - Send Target To Target'),
(184969,1,3,0,61,0,100,0,0,0,0,0,0,45,0,4,0,0,0,0,19,21851,5,0,0,0,0,0,'The Fourth Prophecy - Linked To Id 2 - Set Data 0 4');
UPDATE `gameobject_template` SET `AIName` = 'SmartGameObjectAI' WHERE `entry` IN (184950,184967,184968,184969) AND `AIName` = '' AND `ScriptName` = '';
