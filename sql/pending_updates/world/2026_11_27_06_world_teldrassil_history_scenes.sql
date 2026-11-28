-- Restore the Starbreeze Village and Oracle Glade moonwell history scenes.
-- Restrict each summon to its active quest and suppress nearby duplicate shades.
UPDATE `creature_template` SET `AIName`='SmartAI' WHERE `entry` IN (34583,34584) AND `ScriptName`='';
DELETE FROM `smart_scripts` WHERE (`source_type`=0 AND `entryorguid` IN (-138623,-140238,34583,34584)) OR (`source_type`=9 AND `entryorguid` IN (3458300,3458400));
INSERT INTO `smart_scripts` (`entryorguid`,`source_type`,`id`,`link`,`event_type`,`event_phase_mask`,`event_chance`,`event_flags`,`event_param1`,`event_param2`,`event_param3`,`event_param4`,`event_param5`,`action_type`,`action_param1`,`action_param2`,`action_param3`,`action_param4`,`action_param5`,`action_param6`,`target_type`,`target_param1`,`target_param2`,`target_param3`,`target_x`,`target_y`,`target_z`,`target_o`,`comment`) VALUES
(-138623,0,0,1,25,0,100,0,0,0,0,0,0,8,2,0,0,0,0,0,1,0,0,0,0,0,0,0,'Moonwell Bunny - On Reset - Set React Aggressive'),
(-138623,0,1,0,61,0,100,0,0,0,0,0,0,47,0,0,0,0,0,0,1,0,0,0,0,0,0,0,'Moonwell Bunny - On Reset - Set Invisible'),
(-138623,0,2,0,10,0,100,0,1,10,1000,1000,0,11,65678,0,0,0,0,0,7,0,0,0,0,0,0,0,'Moonwell Bunny - On OOC Los - Cast Forcecast Summon Shade of the Kaldorei'),
(-140238,0,0,1,25,0,100,0,0,0,0,0,0,8,2,0,0,0,0,0,1,0,0,0,0,0,0,0,'Moonwell Bunny - On Reset - Set React Aggressive'),
(-140238,0,1,0,61,0,100,0,0,0,0,0,0,47,0,0,0,0,0,0,1,0,0,0,0,0,0,0,'Moonwell Bunny - On Reset - Set Invisible'),
(-140238,0,2,0,10,0,100,0,1,10,1000,1000,0,11,65680,0,0,0,0,0,7,0,0,0,0,0,0,0,'Moonwell Bunny - On OOC Los - Cast Forcecast Summon Shade of the Kaldorei'),
(34583,0,0,0,54,0,100,0,0,0,0,0,0,80,3458300,2,0,0,0,0,1,0,0,0,0,0,0,0,'Shade of the Kaldorei - On Just Summoned - Run Script'),
(3458300,9,0,0,0,0,100,0,0,0,0,0,0,1,0,0,0,0,0,0,1,0,0,0,0,0,0,0,'Shade of the Kaldorei - History - Say Line 0'),
(3458300,9,1,0,0,0,100,0,3000,3000,0,0,0,1,1,0,0,0,0,0,1,0,0,0,0,0,0,0,'Shade of the Kaldorei - History - Say Line 1'),
(3458300,9,2,0,0,0,100,0,9000,9000,0,0,0,1,2,0,0,0,0,0,1,0,0,0,0,0,0,0,'Shade of the Kaldorei - History - Say Line 2'),
(3458300,9,3,0,0,0,100,0,9000,9000,0,0,0,1,3,0,0,0,0,0,1,0,0,0,0,0,0,0,'Shade of the Kaldorei - History - Say Line 3'),
(3458300,9,4,0,0,0,100,0,9000,9000,0,0,0,1,4,0,0,0,0,0,1,0,0,0,0,0,0,0,'Shade of the Kaldorei - History - Say Line 4'),
(3458300,9,5,0,0,0,100,0,9000,9000,0,0,0,1,5,0,0,0,0,0,1,0,0,0,0,0,0,0,'Shade of the Kaldorei - History - Say Line 5'),
(3458300,9,6,0,0,0,100,0,3000,3000,0,0,0,41,0,0,0,0,0,0,1,0,0,0,0,0,0,0,'Shade of the Kaldorei - Script - Despawn'),
(34584,0,0,0,54,0,100,0,0,0,0,0,0,80,3458400,2,0,0,0,0,1,0,0,0,0,0,0,0,'Shade of the Kaldorei - On Just Summoned - Run Script'),
(3458400,9,0,0,0,0,100,0,0,0,0,0,0,1,0,0,0,0,0,0,1,0,0,0,0,0,0,0,'Shade of the Kaldorei - History - Say Line 0'),
(3458400,9,1,0,0,0,100,0,3000,3000,0,0,0,1,1,0,0,0,0,0,1,0,0,0,0,0,0,0,'Shade of the Kaldorei - History - Say Line 1'),
(3458400,9,2,0,0,0,100,0,9000,9000,0,0,0,1,2,0,0,0,0,0,1,0,0,0,0,0,0,0,'Shade of the Kaldorei - History - Say Line 2'),
(3458400,9,3,0,0,0,100,0,9000,9000,0,0,0,1,3,0,0,0,0,0,1,0,0,0,0,0,0,0,'Shade of the Kaldorei - History - Say Line 3'),
(3458400,9,4,0,0,0,100,0,9000,9000,0,0,0,1,4,0,0,0,0,0,1,0,0,0,0,0,0,0,'Shade of the Kaldorei - History - Say Line 4'),
(3458400,9,5,0,0,0,100,0,9000,9000,0,0,0,1,5,0,0,0,0,0,1,0,0,0,0,0,0,0,'Shade of the Kaldorei - History - Say Line 5'),
(3458400,9,6,0,0,0,100,0,9000,9000,0,0,0,1,6,0,0,0,0,0,1,0,0,0,0,0,0,0,'Shade of the Kaldorei - History - Say Line 6'),
(3458400,9,7,0,0,0,100,0,3000,3000,0,0,0,41,0,0,0,0,0,0,1,0,0,0,0,0,0,0,'Shade of the Kaldorei - Script - Despawn');
DELETE FROM `creature_text` WHERE `entry` IN (34583,34584);
INSERT INTO `creature_text` (`entry`,`groupid`,`id`,`text`,`type`,`language`,`probability`,`emote`,`duration`,`sound`,`comment`) VALUES
(34583,0,0,'%s stares into the moonwell, lost in distant memories.',16,0,100,0,0,0,'Shade of the Kaldorei - Moonwell history'),
(34583,1,0,'The Arch Druid approached the dragons for their blessings, as the dragons had placed on Nordrassil in ancient times.',12,0,100,1,0,0,'Shade of the Kaldorei - Moonwell history'),
(34583,2,0,'But Nozdormu, Lord of Time, refused to give his blessing, chiding the druid for his arrogance.',12,0,100,1,0,0,'Shade of the Kaldorei - Moonwell history'),
(34583,3,0,'In agreement, Alexstrasza also refused Staghelm, and without her blessing, Teldrassil''s growth has been flawed and unpredictable.',12,0,100,1,0,0,'Shade of the Kaldorei - Moonwell history'),
(34583,4,0,'Staghelm retreated to his enclave in Darnassus, ever seeking a new direction by which to bless Teldrassil, and restore the immortality of our people.',12,0,100,1,0,0,'Shade of the Kaldorei - Moonwell history'),
(34583,5,0,'The Shade of the Kaldorei closes its eyes and fades away.',16,0,100,0,0,0,'Shade of the Kaldorei - Moonwell history'),
(34584,0,0,'%s approaches the moonwell slowly, a familiar, thoughtful look upon its face.',16,0,100,0,0,0,'Shade of the Kaldorei - Moonwell history'),
(34584,1,0,'Over time, Staghelm only grew more disconnected from our people.',12,0,100,1,0,0,'Shade of the Kaldorei - Moonwell history'),
(34584,2,0,'Some pose that he blames us, his people, for the loss of his son, long ago during the War of the Shifting Sands.',12,0,100,1,0,0,'Shade of the Kaldorei - Moonwell history'),
(34584,3,0,'Others cite rumors of his escalating conflicts with Tyrande Whisperwind, the shining light of our people, our leader in Darnassus.',12,0,100,1,0,0,'Shade of the Kaldorei - Moonwell history'),
(34584,4,0,'But many druids of the Cenarion Circle stand firm that he wants only what is best for all of the Kaldorei, and serve him unerringly towards that end.',12,0,100,1,0,0,'Shade of the Kaldorei - Moonwell history'),
(34584,5,0,'Fandral Staghelm carries the burden of our sins and our follies, and in so doing deserves the respect of us all.',12,0,100,1,0,0,'Shade of the Kaldorei - Moonwell history'),
(34584,6,0,'The Shade of the Kaldorei closes its eyes and fades away.',16,0,100,66,0,0,'Shade of the Kaldorei - Moonwell history');
DELETE FROM `conditions` WHERE `SourceTypeOrReferenceId`=17 AND `SourceEntry` IN (65678,65680);
INSERT INTO `conditions` (`SourceTypeOrReferenceId`,`SourceGroup`,`SourceEntry`,`SourceId`,`ElseGroup`,`ConditionTypeOrReference`,`ConditionTarget`,`ConditionValue1`,`ConditionValue2`,`ConditionValue3`,`NegativeCondition`,`ErrorType`,`ErrorTextId`,`ScriptName`,`Comment`) VALUES
(17,0,65678,0,0,9,1,929,0,0,0,0,0,'','Moonwell history - active quest and no nearby duplicate'),
(17,0,65678,0,0,29,1,34583,30,0,1,0,0,'','Moonwell history - active quest and no nearby duplicate'),
(17,0,65680,0,0,9,1,7383,0,0,0,0,0,'','Moonwell history - active quest and no nearby duplicate'),
(17,0,65680,0,0,29,1,34584,30,0,1,0,0,'','Moonwell history - active quest and no nearby duplicate');
DELETE FROM `spell_target_position` WHERE `id` IN (65668,65672) AND `effIndex`=0;
INSERT INTO `spell_target_position` (`id`,`effIndex`,`target_map`,`target_position_x`,`target_position_y`,`target_position_z`,`target_orientation`) VALUES
(65668,0,1,9866.79,592.149,1302.05,3.56403),
(65672,0,1,10675.6,1857.79,1324.26,2.44346);
