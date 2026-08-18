-- Quest 13946 "Nature's Reprisal": Ireroot Seeds (item 46716) cast spell 65455, but nothing
-- implemented the hit, so no credit for 34440 "Fel Rock Grellkin Kill Credit" was granted.
-- Targets inside the quest POI polygon are Rascal Sprite (2002) and Shadow Sprite (2003).
UPDATE `creature_template` SET `AIName`='SmartAI' WHERE `entry`=2003;

DELETE FROM `smart_scripts` WHERE `source_type`=0 AND `entryorguid`=2002 AND `id` IN (1,2);
DELETE FROM `smart_scripts` WHERE `source_type`=0 AND `entryorguid`=2003 AND `id` IN (0,1);

INSERT INTO `smart_scripts`
(`entryorguid`,`source_type`,`id`,`link`,`event_type`,`event_phase_mask`,`event_chance`,`event_flags`,
 `event_param1`,`event_param2`,`action_type`,`action_param1`,`target_type`,`comment`) VALUES
(2002,0,1,2, 8,0,100,0,65455,0,33,34440,7,'Rascal Sprite - On spellhit Ireroot Seeds - Quest credit'),
(2002,0,2,0,61,0,100,0,    0,0,37,    0,1,'Rascal Sprite - Linked - Die'),
(2003,0,0,1, 8,0,100,0,65455,0,33,34440,7,'Shadow Sprite - On spellhit Ireroot Seeds - Quest credit'),
(2003,0,1,0,61,0,100,0,    0,0,37,    0,1,'Shadow Sprite - Linked - Die');
