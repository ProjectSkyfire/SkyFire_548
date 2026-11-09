-- Ashenvale: item sources of Respect for the Fallen, Recover the Fallen and In a Bind, and the Chief Murgut event of King of the Foulweald.
-- Based on Legends of Azeroth 5.4.8.
UPDATE `creature_template` SET `npcflag`=16777216, `AIName`='SmartAI' WHERE `entry` IN (4054,33347,33348);
DELETE FROM `smart_scripts` WHERE `source_type`=0 AND `entryorguid` IN (4054, 33347, 33348);
INSERT INTO `smart_scripts` (`entryorguid`,`source_type`,`id`,`link`,`event_type`,`event_phase_mask`,`event_chance`,`event_flags`,`event_param1`,`event_param2`,`event_param3`,`event_param4`,`event_param5`,`action_type`,`action_param1`,`action_param2`,`action_param3`,`action_param4`,`action_param5`,`action_param6`,`target_type`,`target_param1`,`target_param2`,`target_param3`,`target_x`,`target_y`,`target_z`,`target_o`,`comment`) VALUES
(4054,0,0,0,2,0,100,1,0,15,0,0,0,25,1,0,0,0,0,0,7,0,0,0,0,0,0,0,'Laughing Sister - Between 0-15% Health - Flee For Assist'),
(4054,0,1,2,73,0,100,0,0,0,0,0,0,56,46543,1,0,0,0,0,7,0,0,0,0,0,0,0,'Laughing Sister - On SpellClick - Add Item Laughing Sister''s Corpse'),
(4054,0,2,0,61,0,100,0,0,0,0,0,0,41,0,0,0,0,0,0,1,0,0,0,0,0,0,0,'Laughing Sister Linked To Id 1 - Despawn'),
(33347,0,0,1,73,0,100,0,0,0,0,0,0,11,62839,16,0,0,0,0,7,0,0,0,0,0,0,0,'Delgren the Purifier - On Spellclick - Cast Create The Purifier''s Prayer Book Cover'),
(33347,0,1,0,61,0,100,0,0,0,0,0,0,56,45043,1,0,0,0,0,7,0,0,0,0,0,0,0,'Delgren the Purifier - Linked To Id 0 - Add Item The Purifier''s Prayer Book'),
(33348,0,0,1,73,0,100,0,0,0,0,0,0,11,62837,16,0,0,0,0,7,0,0,0,0,0,0,0,'Feero Ironhand - On Spellclick - Cast Create Feero''s Holy Hammer Cover'),
(33348,0,1,0,61,0,100,0,0,0,0,0,0,56,45042,1,0,0,0,0,7,0,0,0,0,0,0,0,'Feero Ironhand - Linked To Id 0 - Add Item Feero''s Holy Hammer');
DELETE FROM `npc_spellclick_spells` WHERE `npc_entry` IN (4054,33347,33348);
INSERT INTO `npc_spellclick_spells` (`npc_entry`,`spell_id`,`cast_flags`,`user_type`) VALUES
(4054,82781,1,0),
(33347,62839,1,0),
(33348,62837,1,0);
DELETE FROM `conditions` WHERE `SourceTypeOrReferenceId`=18 AND `SourceGroup` IN (4054,33347,33348);
INSERT INTO `conditions` (`SourceTypeOrReferenceId`,`SourceGroup`,`SourceEntry`,`SourceId`,`ElseGroup`,`ConditionTypeOrReference`,`ConditionTarget`,`ConditionValue1`,`ConditionValue2`,`ConditionValue3`,`NegativeCondition`,`ErrorType`,`ErrorTextId`,`ScriptName`,`Comment`) VALUES
(18,33347,62839,0,0,2,0,45043,1,0,1,0,0,'','Spellclick allow if player does not have The Purifier''s Prayer Book'),
(18,33347,62839,0,0,9,0,13626,0,0,0,0,0,'','Spellclick allow if player have quest 13626'),
(18,33348,62837,0,0,2,0,45042,1,0,1,0,0,'','Spellclick allow if player does not have Feero''s Holy Hammer'),
(18,33348,62837,0,0,9,0,13626,0,0,0,0,0,'','Spellclick allow if player have quest 13626');

UPDATE `creature_template` SET `AIName`='SmartAI' WHERE `entry` IN (3698,34602);
UPDATE `creature_template` SET `minlevel`=26, `maxlevel`=26, `faction_A`=1801, `faction_H`=1801, `lootid`=34602 WHERE `entry`=34602;
DELETE FROM `creature_loot_template` WHERE `entry`=34602 AND `item`=46774;
INSERT INTO `creature_loot_template` (`entry`,`item`,`ChanceOrQuestChance`,`lootmode`,`groupid`,`mincountOrRef`,`maxcount`) VALUES
(34602,46774,-100,1,0,1,1);
DELETE FROM `smart_scripts` WHERE `source_type`=0 AND `entryorguid` IN (3698, 34602);
INSERT INTO `smart_scripts` (`entryorguid`,`source_type`,`id`,`link`,`event_type`,`event_phase_mask`,`event_chance`,`event_flags`,`event_param1`,`event_param2`,`event_param3`,`event_param4`,`event_param5`,`action_type`,`action_param1`,`action_param2`,`action_param3`,`action_param4`,`action_param5`,`action_param6`,`target_type`,`target_param1`,`target_param2`,`target_param3`,`target_x`,`target_y`,`target_z`,`target_o`,`comment`) VALUES
(3698,0,0,1,19,0,100,0,13982,0,0,0,0,64,1,0,0,0,0,0,7,0,0,0,0,0,0,0,'Bolyun - On Accepted Quest - Store Target List'),
(3698,0,1,0,61,0,100,0,0,0,0,0,0,12,34602,1,60000,2,0,0,8,0,0,0,2384.69,636.644,147.78,2.07595,'Bolyun - Linked To Id 0 - Summon Creature Kaliva'),
(3698,0,2,0,38,0,100,0,0,1,0,0,0,100,1,0,0,0,0,0,11,34602,100,0,0,0,0,0,'Bolyun - On Data Set 0 1 - Send Target To Target'),
(34602,0,0,1,54,0,100,0,0,0,0,0,0,45,0,1,0,0,0,0,11,3698,100,0,0,0,0,0,'Kaliva - On Summoned - Set Data 0 1'),
(34602,0,1,0,61,0,100,0,0,0,0,0,0,1,0,0,0,0,0,0,12,1,0,0,0,0,0,0,'Kaliva - Linked To Id 0 - Say Line 0'),
(34602,0,2,0,61,0,100,0,0,0,0,0,0,49,0,0,0,0,0,0,12,1,0,0,0,0,0,0,'Kaliva - Linked To Id 1 - Attack Start');

UPDATE `creature_template` SET `AIName`='SmartAI' WHERE `entry`=12918;
UPDATE `gameobject_template` SET `AIName`='SmartGameObjectAI' WHERE `entry`=178205;
DELETE FROM `smart_scripts` WHERE `source_type`=0 AND `entryorguid` IN (12918);
DELETE FROM `smart_scripts` WHERE `source_type`=1 AND `entryorguid` IN (178205);
DELETE FROM `smart_scripts` WHERE `source_type`=9 AND `entryorguid` IN (17820500);
INSERT INTO `smart_scripts` (`entryorguid`,`source_type`,`id`,`link`,`event_type`,`event_phase_mask`,`event_chance`,`event_flags`,`event_param1`,`event_param2`,`event_param3`,`event_param4`,`event_param5`,`action_type`,`action_param1`,`action_param2`,`action_param3`,`action_param4`,`action_param5`,`action_param6`,`target_type`,`target_param1`,`target_param2`,`target_param3`,`target_x`,`target_y`,`target_z`,`target_o`,`comment`) VALUES
(12918,0,0,1,54,0,100,1,0,0,0,0,0,49,0,0,0,0,0,0,21,100,0,0,0,0,0,0,'Chief Murgut - On Just Summoned - Attack Closest Player'),
(12918,0,1,0,61,0,100,1,0,0,0,0,0,1,0,0,0,0,0,0,1,0,0,0,0,0,0,0,'Chief Murgut - Linked To Id 0 - Say Line 0'),
(12918,0,2,3,6,0,100,512,0,0,0,0,0,11,20818,2,0,0,0,0,1,0,0,0,0,0,0,0,'Chief Murgut - On Just Died - Cast Summon Foulweald Totem Basket'),
(12918,0,3,0,61,0,100,0,0,0,0,0,0,1,1,0,0,0,0,0,1,0,0,0,0,0,0,0,'Chief Murgut - Linked To Id 2 - Say Line 1'),
(178205,1,0,0,1,0,100,1,5000,5000,0,0,0,80,17820500,2,0,0,0,0,1,0,0,0,0,0,0,0,'Karang''s Banner - Out of Combat - Run Script (No Repeat)'),
(17820500,9,0,0,0,0,100,0,0,0,0,0,0,12,3749,3,180000,0,0,0,8,0,0,0,2237.48,-1524.45,89.7827,0,'On Script - Summon Creature ''Foulweald Ursa'''),
(17820500,9,1,0,0,0,100,0,0,0,0,0,0,12,3743,3,180000,0,0,0,8,0,0,0,2202.16,-1544.48,87.796,0,'On Script - Summon Creature ''Foulweald Warrior'''),
(17820500,9,2,0,0,0,100,0,20000,20000,0,0,0,12,3750,3,180000,0,0,0,8,0,0,0,2208.93,-1567.59,87.2283,0,'On Script - Summon Creature ''Foulweald Totemic'''),
(17820500,9,3,0,0,0,100,0,0,0,0,0,0,12,3749,3,180000,0,0,0,8,0,0,0,2235.44,-1578.43,86.4944,0,'On Script - Summon Creature ''Foulweald Ursa'''),
(17820500,9,4,0,0,0,100,0,0,0,0,0,0,12,3743,3,180000,0,0,0,8,0,0,0,2260.9,-1547.91,89.1733,0,'On Script - Summon Creature ''Foulweald Warrior'''),
(17820500,9,5,0,0,0,100,0,30000,30000,0,0,0,12,3750,3,180000,0,0,0,8,0,0,0,2237.48,-1524.45,89.7827,0,'On Script - Summon Creature ''Foulweald Totemic'''),
(17820500,9,6,0,0,0,100,0,0,0,0,0,0,12,3749,3,180000,0,0,0,8,0,0,0,2202.16,-1544.48,87.796,0,'On Script - Summon Creature ''Foulweald Ursa'''),
(17820500,9,7,0,0,0,100,0,20000,20000,0,0,0,12,3743,3,180000,0,0,0,8,0,0,0,2208.93,-1567.59,87.2283,0,'On Script - Summon Creature ''Foulweald Warrior'''),
(17820500,9,8,0,0,0,100,0,0,0,0,0,0,12,3750,3,180000,0,0,0,8,0,0,0,2235.44,-1578.43,86.4944,0,'On Script - Summon Creature ''Foulweald Totemic'''),
(17820500,9,9,0,0,0,100,0,0,0,0,0,0,12,3749,3,180000,0,0,0,8,0,0,0,2260.9,-1547.91,89.1733,0,'On Script - Summon Creature ''Foulweald Ursa'''),
(17820500,9,10,0,0,0,100,0,30000,30000,0,0,0,12,3743,3,180000,0,0,0,8,0,0,0,2237.48,-1524.45,89.7827,0,'On Script - Summon Creature ''Foulweald Warrior'''),
(17820500,9,11,0,0,0,100,0,0,0,0,0,0,12,3750,3,180000,0,0,0,8,0,0,0,2202.16,-1544.48,87.796,0,'On Script - Summon Creature ''Foulweald Totemic'''),
(17820500,9,12,0,0,0,100,0,20000,20000,0,0,0,12,3749,3,180000,0,0,0,8,0,0,0,2208.93,-1567.59,87.2283,0,'On Script - Summon Creature ''Foulweald Ursa'''),
(17820500,9,13,0,0,0,100,0,0,0,0,0,0,12,3743,3,180000,0,0,0,8,0,0,0,2235.44,-1578.43,86.4944,0,'On Script - Summon Creature ''Foulweald Warrior'''),
(17820500,9,14,0,0,0,100,0,0,0,0,0,0,12,3750,3,180000,0,0,0,8,0,0,0,2260.9,-1547.91,89.1733,0,'On Script - Summon Creature ''Foulweald Totemic'''),
(17820500,9,15,0,0,0,100,0,30000,30000,0,0,0,12,3749,3,180000,0,0,0,8,0,0,0,2237.48,-1524.45,89.7827,0,'On Script - Summon Creature ''Foulweald Ursa'''),
(17820500,9,16,0,0,0,100,0,0,0,0,0,0,12,3743,3,180000,0,0,0,8,0,0,0,2202.16,-1544.48,87.796,0,'On Script - Summon Creature ''Foulweald Warrior'''),
(17820500,9,17,0,0,0,100,0,25000,25000,0,0,0,12,12918,3,180000,0,0,0,8,0,0,0,2208.93,-1567.59,87.2283,0,'On Script - Summon Creature ''Chief Murgut''');
DELETE FROM `creature_text` WHERE `entry` IN (12918);
INSERT INTO `creature_text` (`entry`,`groupid`,`id`,`text`,`type`,`language`,`probability`,`emote`,`duration`,`sound`,`comment`) VALUES
(12918,0,0,'You are a little gnat to the Foulweald!  Die!',12,0,100,0,0,0,'on Aggro Text'),
(12918,1,0,'No!  You cannot be stronger than the Foulweald!  No!!',12,0,100,0,0,0,'on Death Text');
