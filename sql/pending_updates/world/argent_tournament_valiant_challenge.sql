-- Argent Tournament: "The Valiant's Challenge" (Squire Danny and the Argent Champion).
-- Squire Danny summons the Argent Champion for a mounted fight and gives the quest credit when the champion sets his
-- data after yielding. The champion runs the same charge and thrust SmartAI as the faction valiants. The gossip option
-- only shows to a mounted player with the quest who is not already fighting a champion.

UPDATE `creature_template` SET `AIName` = 'SmartAI' WHERE `entry` IN (33518,33707);

DELETE FROM `smart_scripts` WHERE `source_type` = 0 AND `entryorguid` IN (33518,33707);

DELETE FROM `smart_scripts` WHERE `source_type` = 9 AND `entryorguid` IN (3370700);

INSERT INTO `smart_scripts` (`entryorguid`,`source_type`,`id`,`link`,`event_type`,`event_phase_mask`,`event_chance`,`event_flags`,`event_param1`,`event_param2`,`event_param3`,`event_param4`,`event_param5`,`action_type`,`action_param1`,`action_param2`,`action_param3`,`action_param4`,`action_param5`,`action_param6`,`target_type`,`target_param1`,`target_param2`,`target_param3`,`target_x`,`target_y`,`target_z`,`target_o`,`comment`) VALUES
(33518, 0, 0, 1, 62, 0, 100, 0, 10343, 0, 0, 0, 0, 11, 63171, 0, 0, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0, 'Squire Danny  -On gossip select, summons Argent Champion'),
(33518, 0, 1, 2, 61, 0, 100, 0, 0, 0, 0, 0, 0, 72, 0, 0, 0, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0, 'Squire Danny - On gossip select - Close gossip'),
(33518, 0, 2, 0, 61, 0, 100, 0, 0, 0, 0, 0, 0, 64, 1, 0, 0, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0, 'Squire Danny  - Linked with previous event - store target'),
(33518, 0, 3, 0, 38, 0, 100, 0, 0, 1, 0, 0, 0, 85, 63516, 0, 0, 0, 0, 0, 12, 1, 0, 0, 0, 0, 0, 0, 'Squire Danny  - On data set - cast quest credit'),
(33707, 0, 0, 0, 25, 0, 100, 0, 0, 0, 0, 0, 0, 28, 64223, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Arg Champion - On Reset - Remove auras from Defend'),
(33707, 0, 1, 0, 63, 0, 100, 0, 0, 0, 0, 0, 0, 80, 3370700, 2, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Arg Champion - Just created - Call timed actionlist'),
(33707, 0, 2, 0, 52, 0, 100, 0, 0, 33707, 0, 0, 0, 11, 63010, 0, 0, 0, 0, 0, 12, 1, 0, 0, 0, 0, 0, 0, 'Arg Champion - On text over - Cast Charge'),
(33707, 0, 3, 0, 0, 0, 100, 0, 0, 0, 1500, 4000, 0, 11, 62544, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 'Arg Champion - IC - Cast Thrust'),
(33707, 0, 4, 0, 9, 0, 100, 0, 5, 5, 4000, 7000, 0, 11, 62575, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 'Arg Champion - On more than 5 yard range - Cast Shield-Breaker'),
(33707, 0, 5, 0, 0, 0, 100, 0, 9000, 11000, 9000, 10000, 0, 114, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 20, 0, 0, 'Arg Champion - IC - Move forward 20 yards'),
(33707, 0, 6, 7, 9, 0, 100, 0, 9, 15, 3000, 3000, 0, 11, 63010, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 'Arg Champion - On 10-12 yard range - Cast Charge'),
(33707, 0, 7, 0, 61, 0, 100, 0, 0, 0, 0, 0, 0, 11, 62575, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 'Arg Champion - Linked with previous event - Cast Shield-Breaker'),
(33707, 0, 8, 9, 2, 0, 100, 1, 1, 6, 60000, 60000, 0, 2, 35, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Arg Champion - On HP% between 1% and 6% - Change faction to 35'),
(33707, 0, 9, 10, 61, 0, 100, 0, 0, 0, 0, 0, 0, 45, 0, 1, 0, 0, 0, 0, 10, 85139, 33518, 0, 0, 0, 0, 0, 'Arg Champion - On HP% between 1% and 6% - set data'),
(33707, 0, 10, 11, 61, 0, 100, 0, 0, 0, 0, 0, 0, 1, 2, 0, 0, 0, 0, 0, 21, 50, 0, 0, 0, 0, 0, 0, 'Arg Champion - Linked with previous event - Say text'),
(33707, 0, 11, 0, 61, 0, 100, 0, 0, 0, 0, 0, 0, 41, 3000, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Arg Champion - Linked with previous event - Despawn in 3 sec'),
(33707, 0, 12, 13, 5, 0, 100, 0, 60000, 60000, 1, 0, 0, 1, 1, 0, 0, 0, 0, 0, 21, 50, 0, 0, 0, 0, 0, 0, 'Arg Champion - On player killed - Say text'),
(33707, 0, 13, 0, 61, 0, 100, 0, 0, 0, 0, 0, 0, 41, 4000, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Arg Champion - Linked with previous event - Despawn in 1 ms'),
(3370700, 9, 0, 0, 0, 0, 100, 0, 0, 0, 0, 0, 0, 59, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Arg Champion - Script 1 - Set run ON'),
(3370700, 9, 1, 0, 0, 0, 100, 0, 0, 0, 0, 0, 0, 42, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Arg Champion - Script 1 - Set INVINCIBILITY'),
(3370700, 9, 2, 0, 0, 0, 100, 0, 0, 0, 0, 0, 0, 11, 62719, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Arg Champion - Script 1 - Cast Defend on self/Layer 1/'),
(3370700, 9, 3, 0, 0, 0, 100, 0, 0, 0, 0, 0, 0, 11, 62719, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Arg Champion - Script 1 - Cast Defend on self/Layer 2/'),
(3370700, 9, 4, 0, 0, 0, 100, 0, 0, 0, 0, 0, 0, 11, 62719, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Arg Champion - Script 1 - Cast Defend on self/Layer 3/'),
(3370700, 9, 5, 0, 0, 0, 100, 0, 0, 0, 0, 0, 0, 69, 0, 0, 0, 0, 0, 0, 8, 0, 0, 0, 8562.08, 1115.05, 556.787, 2.58798, 'Arg Champion - Script 1 - Move to pos'),
(3370700, 9, 6, 0, 0, 0, 100, 0, 0, 0, 0, 0, 0, 59, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Arg Champion - Script 1 - Set run Off'),
(3370700, 9, 7, 0, 0, 0, 100, 0, 0, 0, 0, 0, 0, 11, 64223, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Arg Champion - Script 1 - Cast aura TriggerS for Defend /10 sec one/'),
(3370700, 9, 8, 0, 0, 0, 100, 0, 3500, 3500, 0, 0, 0, 2, 14, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Arg Champion - Script 1 - Change faction to 14'),
(3370700, 9, 9, 0, 0, 0, 100, 0, 0, 0, 0, 0, 0, 1, 0, 1500, 0, 0, 0, 0, 21, 50, 0, 0, 0, 0, 0, 0, 'Arg Champion - Script 1 - Say text');

DELETE FROM `creature_text` WHERE `entry` = 33707;

INSERT INTO `creature_text` (`entry`,`groupid`,`id`,`text`,`type`,`language`,`probability`,`emote`,`duration`,`sound`,`comment`) VALUES
(33707, 0, 1, 'Ready yourself!', 12, 0, 100, 1, 1000, 0, 'Argent Champion'),
(33707, 0, 2, 'Prepare yourself!', 12, 0, 100, 1, 1000, 0, 'Argent Champion'),
(33707, 0, 3, 'On your guard!', 12, 0, 100, 1, 1000, 0, 'Argent Champion'),
(33707, 0, 4, 'On your guard!', 12, 0, 100, 1, 1000, 0, 'Argent Champion'),
(33707, 0, 5, 'Let it begin!', 12, 0, 100, 1, 1000, 0, 'Argent Champion'),
(33707, 0, 6, 'Argent Champion is looking winded!', 12, 0, 100, 1, 1000, 0, 'Argent Champion'),
(33707, 1, 7, 'Victory is mine!', 12, 0, 100, 1, 1000, 0, 'Argent Champion'),
(33707, 1, 8, 'It seems you still need more practice. Perhaps another time.', 12, 0, 100, 1, 1000, 0, 'Argent Champion'),
(33707, 1, 9, 'I have won. Better luck another time, friend.', 12, 0, 100, 1, 1000, 0, 'Argent Champion'),
(33707, 1, 10, 'I am afraid you will need more practice to defeat me.', 12, 0, 100, 1, 1000, 0, 'Argent Champion'),
(33707, 2, 11, 'I yield to you.', 12, 0, 100, 1, 1000, 0, 'Argent Champion'),
(33707, 2, 12, 'That was a well fought battle. I yield to you.', 12, 0, 100, 1, 1000, 0, 'Argent Champion'),
(33707, 2, 13, 'It would seem I underestimated your skills. Well done..', 12, 0, 100, 1, 1000, 0, 'Argent Champion'),
(33707, 2, 14, 'I have been defeated. Good fight!', 12, 0, 100, 1, 1000, 0, 'Argent Champion');

DELETE FROM `conditions` WHERE `SourceTypeOrReferenceId` = 15 AND `SourceGroup` = 10343;

INSERT INTO `conditions` (`SourceTypeOrReferenceId`,`SourceGroup`,`SourceEntry`,`SourceId`,`ElseGroup`,`ConditionTypeOrReference`,`ConditionTarget`,`ConditionValue1`,`ConditionValue2`,`ConditionValue3`,`NegativeCondition`,`ErrorType`,`ErrorTextId`,`ScriptName`,`Comment`) VALUES
(15, 10343, 0, 0, 0, 1, 0, 63034, 0, 0, 0, 0, 0, '', 'Squire Danny gossip: the player has aura 63034'),
(15, 10343, 0, 0, 0, 28, 0, 13731, 0, 0, 1, 0, 0, '', 'Squire Danny gossip: quest 13731 is not complete'),
(15, 10343, 0, 0, 0, 29, 0, 33707, 100, 0, 1, 0, 0, '', 'Squire Danny gossip: no Argent Champion within 100 yards'),
(15, 10343, 0, 0, 0, 47, 0, 13731, 10, 0, 0, 0, 0, '', 'Squire Danny gossip: quest 13731 is in progress or complete'),
(15, 10343, 0, 0, 1, 1, 0, 63034, 0, 0, 0, 0, 0, '', 'Squire Danny gossip: the player has aura 63034'),
(15, 10343, 0, 0, 1, 28, 0, 13729, 0, 0, 1, 0, 0, '', 'Squire Danny gossip: quest 13729 is not complete'),
(15, 10343, 0, 0, 1, 29, 0, 33707, 100, 0, 1, 0, 0, '', 'Squire Danny gossip: no Argent Champion within 100 yards'),
(15, 10343, 0, 0, 1, 47, 0, 13729, 10, 0, 0, 0, 0, '', 'Squire Danny gossip: quest 13729 is in progress or complete'),
(15, 10343, 0, 0, 2, 1, 0, 63034, 0, 0, 0, 0, 0, '', 'Squire Danny gossip: the player has aura 63034'),
(15, 10343, 0, 0, 2, 28, 0, 13728, 0, 0, 1, 0, 0, '', 'Squire Danny gossip: quest 13728 is not complete'),
(15, 10343, 0, 0, 2, 29, 0, 33707, 100, 0, 1, 0, 0, '', 'Squire Danny gossip: no Argent Champion within 100 yards'),
(15, 10343, 0, 0, 2, 47, 0, 13728, 10, 0, 0, 0, 0, '', 'Squire Danny gossip: quest 13728 is in progress or complete'),
(15, 10343, 0, 0, 3, 1, 0, 63034, 0, 0, 0, 0, 0, '', 'Squire Danny gossip: the player has aura 63034'),
(15, 10343, 0, 0, 3, 28, 0, 13727, 0, 0, 1, 0, 0, '', 'Squire Danny gossip: quest 13727 is not complete'),
(15, 10343, 0, 0, 3, 29, 0, 33707, 100, 0, 1, 0, 0, '', 'Squire Danny gossip: no Argent Champion within 100 yards'),
(15, 10343, 0, 0, 3, 47, 0, 13727, 10, 0, 0, 0, 0, '', 'Squire Danny gossip: quest 13727 is in progress or complete'),
(15, 10343, 0, 0, 4, 1, 0, 63034, 0, 0, 0, 0, 0, '', 'Squire Danny gossip: the player has aura 63034'),
(15, 10343, 0, 0, 4, 28, 0, 13726, 0, 0, 1, 0, 0, '', 'Squire Danny gossip: quest 13726 is not complete'),
(15, 10343, 0, 0, 4, 29, 0, 33707, 100, 0, 1, 0, 0, '', 'Squire Danny gossip: no Argent Champion within 100 yards'),
(15, 10343, 0, 0, 4, 47, 0, 13726, 10, 0, 0, 0, 0, '', 'Squire Danny gossip: quest 13726 is in progress or complete'),
(15, 10343, 0, 0, 5, 1, 0, 63034, 0, 0, 0, 0, 0, '', 'Squire Danny gossip: the player has aura 63034'),
(15, 10343, 0, 0, 5, 28, 0, 13725, 0, 0, 1, 0, 0, '', 'Squire Danny gossip: quest 13725 is not complete'),
(15, 10343, 0, 0, 5, 29, 0, 33707, 100, 0, 1, 0, 0, '', 'Squire Danny gossip: no Argent Champion within 100 yards'),
(15, 10343, 0, 0, 5, 47, 0, 13725, 10, 0, 0, 0, 0, '', 'Squire Danny gossip: quest 13725 is in progress or complete'),
(15, 10343, 0, 0, 6, 1, 0, 63034, 0, 0, 0, 0, 0, '', 'Squire Danny gossip: the player has aura 63034'),
(15, 10343, 0, 0, 6, 28, 0, 13724, 0, 0, 1, 0, 0, '', 'Squire Danny gossip: quest 13724 is not complete'),
(15, 10343, 0, 0, 6, 29, 0, 33707, 100, 0, 1, 0, 0, '', 'Squire Danny gossip: no Argent Champion within 100 yards'),
(15, 10343, 0, 0, 6, 47, 0, 13724, 10, 0, 0, 0, 0, '', 'Squire Danny gossip: quest 13724 is in progress or complete'),
(15, 10343, 0, 0, 7, 1, 0, 63034, 0, 0, 0, 0, 0, '', 'Squire Danny gossip: the player has aura 63034'),
(15, 10343, 0, 0, 7, 28, 0, 13723, 0, 0, 1, 0, 0, '', 'Squire Danny gossip: quest 13723 is not complete'),
(15, 10343, 0, 0, 7, 29, 0, 33707, 100, 0, 1, 0, 0, '', 'Squire Danny gossip: no Argent Champion within 100 yards'),
(15, 10343, 0, 0, 7, 47, 0, 13723, 10, 0, 0, 0, 0, '', 'Squire Danny gossip: quest 13723 is in progress or complete'),
(15, 10343, 0, 0, 8, 1, 0, 63034, 0, 0, 0, 0, 0, '', 'Squire Danny gossip: the player has aura 63034'),
(15, 10343, 0, 0, 8, 28, 0, 13713, 0, 0, 1, 0, 0, '', 'Squire Danny gossip: quest 13713 is not complete'),
(15, 10343, 0, 0, 8, 29, 0, 33707, 100, 0, 1, 0, 0, '', 'Squire Danny gossip: no Argent Champion within 100 yards'),
(15, 10343, 0, 0, 8, 47, 0, 13713, 10, 0, 0, 0, 0, '', 'Squire Danny gossip: quest 13713 is in progress or complete'),
(15, 10343, 0, 0, 9, 1, 0, 63034, 0, 0, 0, 0, 0, '', 'Squire Danny gossip: the player has aura 63034'),
(15, 10343, 0, 0, 9, 28, 0, 13699, 0, 0, 1, 0, 0, '', 'Squire Danny gossip: quest 13699 is not complete'),
(15, 10343, 0, 0, 9, 29, 0, 33707, 100, 0, 1, 0, 0, '', 'Squire Danny gossip: no Argent Champion within 100 yards'),
(15, 10343, 0, 0, 9, 47, 0, 13699, 10, 0, 0, 0, 0, '', 'Squire Danny gossip: quest 13699 is in progress or complete');
