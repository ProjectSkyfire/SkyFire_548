-- Blade's Edge Mountains: combat SmartAI for 4 creatures that had no spells and no AI here. The rows are the 5.4.8 ones; the
-- spells they cast agree with the 3.3.5 SmartAI of the same creatures.

-- Blade's Edge Mountains combat abilities
DELETE FROM `smart_scripts` WHERE `source_type` = 0 AND `entryorguid` IN (20095,20216,20768,22175);
INSERT INTO `smart_scripts` (`entryorguid`,`source_type`,`id`,`link`,`event_type`,`event_phase_mask`,`event_chance`,`event_flags`,`event_param1`,`event_param2`,`event_param3`,`event_param4`,`event_param5`,`action_type`,`action_param1`,`action_param2`,`action_param3`,`action_param4`,`action_param5`,`action_param6`,`target_type`,`target_param1`,`target_param2`,`target_param3`,`target_x`,`target_y`,`target_z`,`target_o`,`comment`) VALUES
(20095,0,0,0,6,0,100,0,0,0,0,0,0,1,1,0,0,0,0,0,2,0,0,0,0,0,0,0,'Grimnok Battleborn - On Death - Say text 1 CONDITIONED'),
(20095,0,1,0,0,0,100,0,6000,10000,6000,10000,0,11,15496,0,0,0,0,0,1,0,0,0,0,0,0,0,'Grimnok Battleborn - In Combat - Cast Cleave'),
(20095,0,2,3,0,0,100,0,10000,12000,10000,12000,0,11,37786,0,0,0,0,0,1,0,0,0,0,0,0,0,'Grimnok Battleborn - In Combat - Cast Bloodmaul Rage'),
(20095,0,3,0,61,0,100,0,0,0,0,0,0,1,0,0,0,0,0,0,2,0,0,0,0,0,0,0,'Grimnok Battleborn - In Combat - Say text 0 when cast Cast Bloodmaul Rage'),
(20095,0,4,0,4,0,35,0,0,0,0,0,0,1,0,0,0,0,0,0,2,0,0,0,0,0,0,0,'Grimnok Battleborn - On Aggro - Say text 0'),
(20095,0,5,0,1,0,100,0,120000,120000,180000,180000,0,1,2,0,0,0,0,0,1,0,0,0,0,0,0,0,'Grimnok Battleborn - Out of Combat - Say text 2'),
(20216,0,0,0,0,0,100,0,20000,20000,20000,25000,0,11,38771,1,0,0,0,0,1,0,0,0,0,0,0,0,'Grulloc - In Combat - Cast ''Burning Rage'' (No Repeat)'),
(20216,0,1,0,0,0,100,0,7000,7000,12000,16000,0,11,21055,0,0,0,0,0,2,0,0,0,0,0,0,0,'Grulloc - In Combat - Cast ''Crush Armor'' (No Repeat)'),
(20216,0,2,0,0,0,100,0,10000,10000,45000,45000,0,11,38772,0,0,0,0,0,2,0,0,0,0,0,0,0,'Grulloc - In Combat - Cast ''Grievous Wound'' (No Repeat)'),
(20216,0,3,0,6,0,100,1,0,0,0,0,0,11,39890,7,0,0,0,0,1,0,0,0,0,0,0,0,'Grulloc - On Just Died - Cast ''Grulloc: Summon Grulloc''s Dragon Skull Chest'' (No Repeat)'),
(20768,0,0,0,6,0,100,0,0,0,0,0,0,1,1,0,0,0,0,0,2,0,0,0,0,0,0,0,'Gnosh Brognat - On Death - Say text 1 CONDITIONED'),
(20768,0,1,0,4,0,35,0,0,0,0,0,0,1,0,0,0,0,0,0,1,0,0,0,0,0,0,0,'Gnosh Brognat - On Aggro - Say Text 0'),
(20768,0,2,0,0,0,100,0,15000,20000,15000,20000,0,11,37597,0,0,0,0,0,2,0,0,0,0,0,0,0,'Gnosh Brognat - In Combat - Cast Meat Slap'),
(20768,0,3,0,0,0,100,0,15000,20000,15000,20000,0,11,37596,32,0,0,0,0,2,0,0,0,0,0,0,0,'Gnosh Brognat - In Combat - Cast Tenderize'),
(22175,0,0,0,0,0,100,0,8300,13300,22000,26000,0,11,13443,0,0,0,0,0,5,0,0,0,0,0,0,0,'Apexis Flayer - In Combat - Cast ''Rend'' (No Repeat)'),
(22175,0,1,0,0,0,100,0,4200,6200,28100,31600,0,11,40770,0,0,0,0,0,2,0,0,0,0,0,0,0,'Apexis Flayer - In Combat - Cast ''Shred Armor'' (No Repeat)');
DELETE FROM `creature_text` WHERE `entry` IN (20095,20768);
INSERT INTO `creature_text` (`entry`,`groupid`,`id`,`text`,`type`,`language`,`probability`,`emote`,`duration`,`sound`,`comment`) VALUES
(20095,0,0,'Stupid puny thing.  Me smash!',12,0,100,0,0,0,'Grimnok Battleborn on Aggro Text'),
(20095,0,1,'Me mad. You get smash in face!',12,0,100,0,0,0,'Grimnok Battleborn on Aggro Text'),
(20095,0,2,'Raaar!!! Me smash $r!',12,0,100,0,0,0,'Grimnok Battleborn on Aggro Text'),
(20095,1,0,'Puny $r... you were a... horrible $g king : queen;.',12,0,100,0,0,0,'Grimnok Battleborn on death Quest 11000 complete'),
(20095,1,1,'Agh... me am...dead.',12,0,100,0,0,0,'Grimnok Battleborn on death Quest 11000 complete'),
(20095,1,2,'Me go to... Ogri''la.',12,0,100,0,0,0,'Grimnok Battleborn on death Quest 11000 complete'),
(20095,1,3,'$G King : Queen; think... there really is... an Ogri''la?',12,0,100,0,0,0,'Grimnok Battleborn on death Quest 11000 complete'),
(20095,1,4,'Me honored... $g king : queen; kill me.',12,0,100,0,0,0,'Grimnok Battleborn on death Quest 11000 complete'),
(20095,1,5,'$G King : Queen; $n, me die now.',12,0,100,0,0,0,'Grimnok Battleborn on death Quest 11000 complete'),
(20095,2,0,'You!  I see you!  One of you, kill them!',14,0,100,0,0,0,'Grimnok Battleborn OOC'),
(20095,2,1,'Don''t let any of them escape, you dogs!',14,0,100,0,0,0,'Grimnok Battleborn OOC'),
(20095,2,2,'I want these Alliance and Horde pigs dead!',14,0,100,0,0,0,'Grimnok Battleborn OOC'),
(20095,2,3,'Get up there.  March on Bladespire Hold.  I want to adorn my tower with Gorr''Dim''s head!',14,0,100,0,0,0,'Grimnok Battleborn OOC'),
(20768,0,0,'Stupid puny thing.  Me smash!',12,0,100,0,0,0,'Gnosh Brognat on Aggro Text'),
(20768,0,1,'Me mad. You get smash in face!',12,0,100,0,0,0,'Gnosh Brognat on Aggro Text'),
(20768,0,2,'Raaar!!! Me smash $r!',12,0,100,0,0,0,'Gnosh Brognat on Aggro Text'),
(20768,1,0,'Puny $r... you were a... horrible $g king : queen;.',12,0,100,0,0,0,'Gnosh Brognat on death Quest 11000 complete'),
(20768,1,1,'Agh... me am...dead.',12,0,100,0,0,0,'Gnosh Brognat on death Quest 11000 complete'),
(20768,1,2,'Me go to... Ogri''la.',12,0,100,0,0,0,'Gnosh Brognat on death Quest 11000 complete'),
(20768,1,3,'$G King : Queen; think... there really is... an Ogri''la?',12,0,100,0,0,0,'Gnosh Brognat on death Quest 11000 complete'),
(20768,1,4,'Me honored... $g king : queen; kill me.',12,0,100,0,0,0,'Gnosh Brognat on death Quest 11000 complete'),
(20768,1,5,'$G King : Queen; $n, me die now.',12,0,100,0,0,0,'Gnosh Brognat on death Quest 11000 complete');
UPDATE `creature_template` SET `AIName` = 'SmartAI' WHERE `entry` IN (20095,20216,20768,22175) AND `AIName` = '' AND `ScriptName` = '';
