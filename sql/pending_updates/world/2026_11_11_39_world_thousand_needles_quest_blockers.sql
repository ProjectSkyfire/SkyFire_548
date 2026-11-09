-- Thousand Needles: the starter of To the Withering and the Shu'halo artifact event of Invoking the Serpent (event script, Arikara, the three artifact summons and the Grimtotem Spirits).
-- Based on Legends of Azeroth 5.4.8.
DELETE FROM `creature_queststarter` WHERE `id`=47745 AND `quest`=28142;
INSERT INTO `creature_queststarter` (`id`,`quest`) VALUES
(47745,28142);

UPDATE `creature_template` SET `AIName`='SmartAI' WHERE `entry` IN (45447,45463,45466,45468,46067);
UPDATE `creature_template` SET `faction_A`=14, `faction_H`=14 WHERE `entry`=45447;
UPDATE `creature_template` SET `unit_flags`=33554432 WHERE `entry` IN (45463,45466,45468);
UPDATE `creature_template` SET `minlevel`=42, `maxlevel`=42, `faction_A`=189, `faction_H`=189, `unit_flags`=32768 WHERE `entry`=46067;
DELETE FROM `smart_scripts` WHERE `source_type`=0 AND `entryorguid` IN (45447, 45463, 45466, 45468, 46067);
DELETE FROM `smart_scripts` WHERE `source_type`=9 AND `entryorguid` IN (4546300, 4546600, 4546800);
INSERT INTO `smart_scripts` (`entryorguid`,`source_type`,`id`,`link`,`event_type`,`event_phase_mask`,`event_chance`,`event_flags`,`event_param1`,`event_param2`,`event_param3`,`event_param4`,`event_param5`,`action_type`,`action_param1`,`action_param2`,`action_param3`,`action_param4`,`action_param5`,`action_param6`,`target_type`,`target_param1`,`target_param2`,`target_param3`,`target_x`,`target_y`,`target_z`,`target_o`,`comment`) VALUES
(45447,0,0,1,54,0,100,0,0,0,0,0,0,8,1,0,0,0,0,0,1,0,0,0,0,0,0,0,'Arikara - On Summoned - Set React State Defensive'),
(45447,0,1,2,61,0,100,0,0,0,0,0,0,42,1,0,0,0,0,0,1,0,0,0,0,0,0,0,'Arikara - Linked To Id 0 - Set Invincibility Hp 1'),
(45447,0,2,3,61,0,100,0,0,0,0,0,0,11,43759,0,0,0,0,0,1,0,0,0,0,0,0,0,'Arikara - Linked To Id 1 - Cast Arcane Explosion Visual'),
(45447,0,3,4,61,0,100,0,0,0,0,0,0,3,0,34507,0,0,0,0,1,0,0,0,0,0,0,0,'Arikara - Linked To Id 2 - Morph To Entry Or Model'),
(45447,0,4,5,61,0,100,0,0,0,0,0,0,18,768,0,0,0,0,0,1,0,0,0,0,0,0,0,'Arikara - Linked To Id 3 - Set UnitFlag 768'),
(45447,0,5,0,61,0,100,0,0,0,0,0,0,67,45447,2000,2000,0,0,0,1,0,0,0,0,0,0,0,'Arikara - Linked To Id 4 - Create Timed Event'),
(45447,0,6,0,59,0,100,0,45447,0,0,0,0,1,0,1000,0,0,0,0,21,10,0,0,0,0,0,0,'Arikara - On Timed Event Triggered - Say Line 0'),
(45447,0,7,8,52,0,100,0,0,45447,0,0,0,19,768,0,0,0,0,0,1,0,0,0,0,0,0,0,'Arikara - On Text Over 0 - Remove UnitFlag 768'),
(45447,0,8,0,61,0,100,0,0,0,0,0,0,49,0,0,0,0,0,0,21,10,0,0,0,0,0,0,'Arikara - Linked To Id 7 - Attack Start'),
(45447,0,9,0,0,0,100,0,0,0,3500,3500,0,11,79085,0,0,0,0,0,2,0,0,0,0,0,0,0,'Arikara - IC - Cast Lightning Bolt'),
(45447,0,10,0,2,0,100,1,0,80,0,0,0,12,46067,5,0,0,0,0,8,0,0,0,-4994.97,-2110.89,84.2303,2.86234,'Arikara - Between 0-80% Health - Summon Creature Grimtotem Spirit'),
(45447,0,11,0,2,0,100,1,0,60,0,0,0,12,46067,5,0,0,0,0,8,0,0,0,-5019.61,-2107.33,83.64,6.21337,'Arikara - Between 0-60% Health - Summon Creature Grimtotem Spirit'),
(45447,0,12,0,2,0,100,1,0,40,0,0,0,12,46067,5,0,0,0,0,8,0,0,0,-4998.63,-2125.59,83.8122,1.88496,'Arikara - Between 0-40% Health - Summon Creature Grimtotem Spirit'),
(45447,0,13,0,2,0,100,1,0,20,0,0,0,12,46067,5,0,0,0,0,8,0,0,0,-5011.31,-2095.41,84.0287,5.21853,'Arikara - Between 0-20% Health - Summon Creature Grimtotem Spirit'),
(45447,0,14,15,2,0,100,1,0,1,0,0,0,92,0,79085,1,0,0,0,1,0,0,0,0,0,0,0,'Arikara - Between 0-1% Health - Interrupt Spell Lightning Bolt'),
(45447,0,15,16,61,0,100,1,0,0,0,0,0,8,0,0,0,0,0,0,1,0,0,0,0,0,0,0,'Arikara - Linked To Id 14 - Set React State Passive'),
(45447,0,16,17,61,0,100,1,0,0,0,0,0,27,0,0,0,0,0,0,1,0,0,0,0,0,0,0,'Arikara - Linked To Id 15 - Combat Stop'),
(45447,0,17,18,61,0,100,0,0,0,0,0,0,18,768,0,0,0,0,0,1,0,0,0,0,0,0,0,'Arikara - Linked To Id 16 - Set UnitFlag 768'),
(45447,0,18,19,61,0,100,0,0,0,0,0,0,33,45447,0,0,0,0,0,18,10,0,0,0,0,0,0,'Arikara - Linked To Id 17 - Quest Credit'),
(45447,0,19,20,61,0,100,0,0,0,0,0,0,66,0,0,0,0,0,0,21,10,0,0,0,0,0,0,'Arikara - Linked To Id 18 - Set Orientation'),
(45447,0,20,21,61,0,100,0,0,0,0,0,0,45,0,1,0,0,0,0,11,46067,20,0,0,0,0,0,'Arikara - Linked To Id 19 - Set Data 0 1'),
(45447,0,21,0,61,0,100,0,0,0,0,0,0,1,1,3000,0,0,0,0,1,0,0,0,0,0,0,0,'Arikara - Linked To Id 20 - Say Line 1'),
(45447,0,22,0,52,0,100,0,1,45447,0,0,0,1,2,4000,0,0,0,0,1,0,0,0,0,0,0,0,'Arikara - On Text Over 1 - Say Line 2'),
(45447,0,23,0,52,0,100,0,2,45447,0,0,0,1,3,3000,0,0,0,0,1,0,0,0,0,0,0,0,'Arikara - On Text Over 2 - Say Line 3'),
(45447,0,24,25,52,0,100,0,3,45447,0,0,0,53,1,45447,0,0,0,0,1,0,0,0,0,0,0,0,'Arikara - On Text Over 3 - Waypoint Start'),
(45447,0,25,0,61,0,100,0,0,0,0,0,0,45,0,1,0,0,0,0,19,45468,20,0,0,0,0,0,'Arikara - Linked To Id 24 - Set Data 0 1'),
(45447,0,26,0,58,0,100,0,4,45447,0,0,0,41,0,0,0,0,0,0,1,0,0,0,0,0,0,0,'Arikara - On Waypoint Ended - Despawn'),
(45447,0,27,28,7,0,100,0,0,0,0,0,0,45,0,1,0,0,0,0,11,46067,20,0,0,0,0,0,'Arikara - On Evade - Set Data 0 1'),
(45447,0,28,29,61,0,100,0,0,0,0,0,0,45,0,1,0,0,0,0,19,45468,20,0,0,0,0,0,'Arikara - Linked To Id 27 - Set Data 01'),
(45447,0,29,0,61,0,100,0,0,0,0,0,0,41,0,0,0,0,0,0,1,0,0,0,0,0,0,0,'Arikara - Linked To Id 28 - Despawn'),
(45463,0,0,1,54,0,100,0,0,0,0,0,0,53,1,45463,1,0,0,0,1,0,0,0,0,0,0,0,'The Rattle of Bones - On Summoned - Waypoint Start'),
(45463,0,1,0,61,0,100,0,0,0,0,0,0,80,4546300,2,0,0,0,0,1,0,0,0,0,0,0,0,'The Rattle of Bones - Linked To Id 0 - Run Script'),
(45466,0,0,1,54,0,100,0,0,0,0,0,0,53,1,45466,1,0,0,0,1,0,0,0,0,0,0,0,'The Writ of History - On Summoned - Waypoint Start'),
(45466,0,1,0,61,0,100,0,0,0,0,0,0,80,4546600,2,0,0,0,0,1,0,0,0,0,0,0,0,'The Writ of History - Linked To Id 0 -Run Script'),
(45468,0,0,1,54,0,100,0,0,0,0,0,0,53,1,45468,1,0,0,0,1,0,0,0,0,0,0,0,'The Drums of War - On Summoned - Waypoint Start'),
(45468,0,1,0,61,0,100,0,0,0,0,0,0,80,4546800,2,0,0,0,0,1,0,0,0,0,0,0,0,'The Drums of War - Linked To Id 0 - Run Script'),
(45468,0,2,0,38,0,100,0,0,1,0,0,0,41,0,0,0,0,0,0,1,0,0,0,0,0,0,0,'The Drums of War - On Data Set 0 1 - Despawn'),
(46067,0,0,1,54,0,100,0,0,0,0,0,0,11,85838,0,0,0,0,0,19,45447,30,0,0,0,0,0,'Grimtotem Spirit - On Summoned - Cast Ancestral Fortitude'),
(46067,0,1,2,61,0,100,0,0,0,0,0,0,45,0,1,0,0,0,0,19,45278,30,0,0,0,0,0,'Grimtotem Spirit - Linked To Id 0 - Set Data 0 1'),
(46067,0,2,0,61,0,100,0,0,0,0,0,0,45,0,1,0,0,0,0,19,45277,30,0,0,0,0,0,'Grimtotem Spirit - Linked To Id 1 - Set Data 0 1'),
(46067,0,3,0,38,0,100,0,0,1,0,0,0,41,0,0,0,0,0,0,1,0,0,0,0,0,0,0,'Grimtotem Spirit - On Data 0 1 - Despawn'),
(4546300,9,0,0,0,0,100,0,20000,20000,0,0,0,11,43759,0,0,0,0,0,1,0,0,0,0,0,0,0,'The Rattle of Bones - On Script - Cast Arcane Explosion Visual'),
(4546300,9,1,0,0,0,100,0,0,0,0,0,0,41,0,0,0,0,0,0,1,0,0,0,0,0,0,0,'The Rattle of Bones - On Script - Despawn'),
(4546600,9,0,0,0,0,100,0,20000,20000,0,0,0,11,43759,0,0,0,0,0,1,0,0,0,0,0,0,0,'The Writ of History - On Script - Cast Arcane Explosion Visual'),
(4546600,9,1,0,0,0,100,0,0,0,0,0,0,41,0,0,0,0,0,0,1,0,0,0,0,0,0,0,'The Writ of History - On Script - Despawn'),
(4546800,9,0,0,0,0,100,0,20000,20000,0,0,0,11,43759,0,0,0,0,0,1,0,0,0,0,0,0,0,'The Drums of War - On Script - Cast Arcane Explosion Visual'),
(4546800,9,1,0,0,0,100,0,0,0,0,0,0,12,45447,8,0,0,0,0,8,0,0,0,-5003.94,-2108.51,86.1941,0.977384,'The Drums of War - On Script - Summon Creature Arikara'),
(4546800,9,2,0,0,0,100,0,0,0,0,0,0,28,84954,0,0,0,0,0,1,0,0,0,0,0,0,0,'The Drums of War - On Script - Remove Aura Invoking the Serpent: The Drums of War');
DELETE FROM `waypoints` WHERE `entry` IN (45447,45463,45466,45468);
INSERT INTO `waypoints` (`entry`,`pointid`,`position_x`,`position_y`,`position_z`,`point_comment`) VALUES
(45447,1,-5011,-2121.3,102.92,'Arikara'),
(45447,2,-5019.51,-2166.33,102.92,'Arikara'),
(45447,3,-5007.47,-2220.29,102.92,'Arikara'),
(45447,4,-5027.35,-2277.75,102.92,'Arikara'),
(45463,1,-4998.55,-2108.52,83.908,'The Rattle of Bones'),
(45463,2,-5001.11,-2114,83.716,'The Rattle of Bones'),
(45463,3,-5007.45,-2115.05,83.678,'The Rattle of Bones'),
(45463,4,-5011.73,-2108.97,83.973,'The Rattle of Bones'),
(45463,5,-5011.23,-2104.06,83.993,'The Rattle of Bones'),
(45463,6,-5006.45,-2101.09,84.091,'The Rattle of Bones'),
(45466,1,-5011.23,-2104.06,83.993,'The Writ of History'),
(45466,2,-5006.45,-2101.09,84.091,'The Writ of History'),
(45466,3,-5000.78,-2102.25,84.132,'The Writ of History'),
(45466,4,-4998.55,-2108.52,83.908,'The Writ of History'),
(45466,5,-5001.11,-2114,83.716,'The Writ of History'),
(45466,6,-5007.45,-2115.05,83.678,'The Writ of History'),
(45468,1,-5007.45,-2115.05,83.678,'The Drums of War'),
(45468,2,-5011.73,-2108.97,83.973,'The Drums of War'),
(45468,3,-5011.23,-2104.06,83.993,'The Drums of War'),
(45468,4,-5006.45,-2101.09,84.091,'The Drums of War'),
(45468,5,-5000.78,-2102.25,84.132,'The Drums of War'),
(45468,6,-4998.55,-2108.52,83.908,'The Drums of War');
DELETE FROM `creature_text` WHERE `entry` IN (45447);
INSERT INTO `creature_text` (`entry`,`groupid`,`id`,`text`,`type`,`language`,`probability`,`emote`,`duration`,`sound`,`comment`) VALUES
(45447,0,0,'Thank you for the gifts. Let us see if you are worthy, $c!',14,0,100,0,0,0,'Arikara'),
(45447,1,0,'Stop! You have bested my wind serpent.',14,0,100,0,0,0,'Arikara'),
(45447,2,0,'I have been taken prisoner by the Twilight''s Hammer.',14,0,100,0,0,0,'Arikara'),
(45447,3,0,'I''m not certain where. Start with their western base overlooking Highperch.',14,0,100,0,0,0,'Arikara');
DELETE FROM `event_scripts` WHERE `id`=26323;
INSERT INTO `event_scripts` (`id`,`delay`,`command`,`datalong`,`datalong2`,`dataint`,`x`,`y`,`z`,`o`) VALUES
(26323,0,10,45468,180000,0,-5001.11,-2114,83.716,3.40388),
(26323,0,10,45466,180000,0,-5011.73,-2108.97,83.973,1.0249),
(26323,0,10,45463,180000,0,-5000.78,-2102.25,84.132,5.25034);
