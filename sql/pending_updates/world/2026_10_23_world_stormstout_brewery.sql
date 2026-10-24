-- Stormstout Brewery (map 961, dungeon, MoP): missing creatures, gameobjects, loot and scripts
-- Idempotent: every INSERT is preceded by a DELETE of its keys.

-- creature_template
DELETE FROM `creature_template` WHERE `entry` IN (900039,900040,900041,900042,900043,900044,900045,900046,900047,900048,900049,900050,900051,900052,900053,900054,900055,900056,900057,900058,900059,900060,900061,900062,900063,900064,900065,900066,900067,900068,900069);
INSERT INTO `creature_template` (`entry`,`difficulty_entry_1`,`difficulty_entry_2`,`difficulty_entry_3`,`KillCredit1`,`KillCredit2`,`modelid1`,`modelid2`,`modelid3`,`modelid4`,`name`,`subname`,`IconName`,`gossip_menu_id`,`minlevel`,`maxlevel`,`exp`,`exp_unk`,`faction_A`,`faction_H`,`npcflag`,`speed_walk`,`speed_run`,`scale`,`npc_rank`,`mindmg`,`maxdmg`,`dmgschool`,`attackpower`,`dmg_multiplier`,`baseattacktime`,`rangeattacktime`,`unit_class`,`unit_flags`,`unit_flags2`,`dynamicflags`,`family`,`trainer_type`,`trainer_class`,`trainer_race`,`minrangedmg`,`maxrangedmg`,`rangedattackpower`,`type`,`type_flags`,`type_flags2`,`lootid`,`pickpocketloot`,`skinloot`,`resistance1`,`resistance2`,`resistance3`,`resistance4`,`resistance5`,`resistance6`,`spell1`,`spell2`,`spell3`,`spell4`,`spell5`,`spell6`,`spell7`,`spell8`,`PetSpellDataId`,`VehicleId`,`mingold`,`maxgold`,`AIName`,`MovementType`,`InhabitType`,`HoverHeight`,`Health_mod`,`Mana_mod`,`Mana_mod_extra`,`Armor_mod`,`RacialLeader`,`questItem1`,`questItem2`,`questItem3`,`questItem4`,`questItem5`,`questItem6`,`movementId`,`RegenHealth`,`mechanic_immune_mask`,`flags_extra`,`ScriptName`,`ModLevel`,`detection_range`,`WDBVerified`) VALUES
(900039,0,0,0,0,0,39498,0,0,0,'Ook-Ook','','',0,92,92,4,0,35,35,0,1.0,1.14286,1.0,1,19140.3,28710.449999999997,0,0,2.0,2000,2000,1,0,0,0,0,0,0,0,19140.3,28710.449999999997,0,7,2097256,0,900039,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,440000,450000,'',0,3,1.0,40.0,1.0,1.0,1.0,0,0,0,0,0,0,0,187,1,0,0,'',0,10.0,17688),
(900040,0,0,0,0,0,39498,0,0,0,'Ook-Ook','','',0,93,93,4,0,35,35,0,1.0,1.14286,1.0,1,24431.0,36646.5,0,0,4.0,2000,2000,1,0,0,0,0,0,0,0,24431.0,36646.5,0,7,2097256,0,56637,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,440000,450000,'',0,3,1.0,60.0,1.0,1.0,1.0,0,0,0,0,0,0,0,187,1,0,0,'',0,10.0,17688),
(900041,0,0,0,0,0,40339,0,0,0,'Hoptallus','','',0,92,92,4,0,35,35,0,1.0,1.14286,1.0,1,19140.3,28710.449999999997,0,0,2.0,2000,2000,1,0,0,0,0,0,0,0,19140.3,28710.449999999997,0,7,2097256,0,900041,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,440000,450000,'',0,3,1.0,30.0,1.0,1.0,1.0,0,0,0,0,0,0,0,144,1,0,0,'',0,10.0,17688),
(900042,0,0,0,0,0,40339,0,0,0,'Hoptallus','','',0,93,93,4,0,35,35,0,1.0,1.14286,1.0,1,24431.0,36646.5,0,0,4.0,2000,2000,1,0,0,0,0,0,0,0,24431.0,36646.5,0,7,2097256,0,56717,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,440000,450000,'',0,3,1.0,45.0,1.0,1.0,1.0,0,0,0,0,0,0,0,144,1,0,0,'',0,10.0,17688),
(900043,0,0,0,0,0,41261,0,0,0,'Hozen Hollerer','','',0,91,91,4,0,35,35,0,1.0,1.14286,1.0,0,14995.0,22492.5,0,0,1.4,2000,2000,1,0,0,0,0,0,0,0,14995.0,22492.5,0,7,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,'',0,3,1.0,5.0,1.0,1.0,1.0,0,0,0,0,0,0,0,0,1,0,0,'',0,10.0,17688),
(900044,0,0,0,0,0,41262,0,0,0,'Hozen Bouncer','','',0,92,92,4,0,190,190,0,1.0,1.14286,1.0,1,19140.3,28710.449999999997,0,0,2.0,2000,2000,1,0,0,0,0,0,0,0,19140.3,28710.449999999997,0,7,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,'',0,3,1.0,50.0,1.0,1.0,1.0,0,0,0,0,0,0,0,187,1,0,0,'',0,20.0,17688),
(900045,0,0,0,0,0,41262,0,0,0,'Hozen Bouncer','','',0,92,92,4,0,190,190,0,1.0,1.14286,1.0,1,19140.3,28710.449999999997,0,0,4.0,2000,2000,1,0,0,0,0,0,0,0,19140.3,28710.449999999997,0,7,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,'',0,3,1.0,50.0,1.0,1.0,1.0,0,0,0,0,0,0,0,187,1,0,0,'',0,20.0,17688),
(900046,0,0,0,0,0,41260,0,0,0,'Drunken Hozen Brawler','','',0,91,91,4,0,16,16,0,1.0,1.14286,1.0,1,14995.0,22492.5,0,0,2.0,2000,2000,1,0,0,0,0,0,0,0,14995.0,22492.5,0,7,0,0,900046,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,9700,9800,'',0,3,1.0,8.0,1.0,1.0,1.0,0,0,0,0,0,0,0,144,1,0,0,'',0,20.0,17688),
(900047,0,0,0,0,0,41260,0,0,0,'Drunken Hozen Brawler','','',0,92,92,4,0,16,16,0,1.0,1.14286,1.0,1,19140.3,28710.449999999997,0,0,4.0,2000,2000,1,0,0,0,0,0,0,0,19140.3,28710.449999999997,0,7,0,0,900047,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,9700,9800,'',0,3,1.0,14.0,1.0,1.0,1.0,0,0,0,0,0,0,0,144,1,0,0,'',0,20.0,17688),
(900048,0,0,0,0,0,41259,0,0,0,'Sleepy Hozen Brawler','','',0,91,91,4,0,2136,2136,0,1.0,1.14286,1.0,1,14995.0,22492.5,0,0,2.0,2000,2000,1,0,0,0,0,0,0,0,14995.0,22492.5,0,7,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,'',0,3,1.0,5.0,1.0,1.0,1.0,0,0,0,0,0,0,0,0,1,0,0,'',0,20.0,17688),
(900049,0,0,0,0,0,41259,0,0,0,'Sleepy Hozen Brawler','','',0,92,92,4,0,2136,2136,0,1.0,1.14286,1.0,1,19140.3,28710.449999999997,0,0,4.0,2000,2000,1,0,0,0,0,0,0,0,19140.3,28710.449999999997,0,7,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,'',0,3,1.0,7.0,1.0,1.0,1.0,0,0,0,0,0,0,0,0,1,0,0,'',0,20.0,17688),
(900050,0,0,0,0,0,42956,0,0,0,'Aqua Dancer','','',0,90,90,4,0,35,35,0,1.0,1.14286,1.0,1,11747.7,17621.550000000003,0,0,2.8,2000,2000,1,0,0,0,0,0,0,0,11747.7,17621.550000000003,0,4,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,'',0,3,1.0,0.5,1.0,1.0,1.0,0,0,0,0,0,0,0,0,1,0,0,'',0,10.0,17688),
(900051,0,0,0,0,0,42956,0,0,0,'Aqua Dancer','','',0,91,91,4,0,35,35,0,1.0,1.14286,1.0,1,14995.0,22492.5,0,0,5.6,2000,2000,1,0,0,0,0,0,0,0,14995.0,22492.5,0,4,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,'',0,3,1.0,0.7,1.0,1.0,1.0,0,0,0,0,0,0,0,0,1,0,0,'',0,10.0,17688),
(900052,0,0,0,0,0,39916,0,0,0,'Fiery Trickster','','',0,90,90,4,0,190,190,0,1.0,1.14286,1.0,1,11747.7,17621.550000000003,0,0,2.0,2000,2000,1,0,0,0,0,0,0,0,11747.7,17621.550000000003,0,4,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,'',0,4,1.0,0.5,1.0,1.0,1.0,0,0,0,0,0,0,0,76,1,0,0,'',0,20.0,17688),
(900053,0,0,0,0,0,39916,0,0,0,'Fiery Trickster','','',0,91,91,4,0,190,190,0,1.0,1.14286,1.0,1,14995.0,22492.5,0,0,4.0,2000,2000,1,0,0,0,0,0,0,0,14995.0,22492.5,0,4,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,'',0,4,1.0,0.7,1.0,1.0,1.0,0,0,0,0,0,0,0,76,1,0,0,'',0,20.0,17688),
(900054,0,0,0,0,0,42957,0,0,0,'Inflamed Hozen Brawler','','',0,92,92,4,0,16,16,0,1.0,1.14286,1.0,1,19140.3,28710.449999999997,0,0,2.0,2000,2000,1,0,0,0,0,0,0,0,19140.3,28710.449999999997,0,7,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,'',0,3,1.0,6.0,1.0,1.0,1.0,0,0,0,0,0,0,0,144,1,0,0,'',0,20.0,17688),
(900055,0,0,0,0,0,42957,0,0,0,'Inflamed Hozen Brawler','','',0,92,92,4,0,16,16,0,1.0,1.14286,1.0,1,19140.3,28710.449999999997,0,0,4.0,2000,2000,1,0,0,0,0,0,0,0,19140.3,28710.449999999997,0,7,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,'',0,3,1.0,10.0,1.0,1.0,1.0,0,0,0,0,0,0,0,144,1,0,0,'',0,20.0,17688),
(900056,0,0,0,0,0,42377,42378,42379,42380,'Hozen Party Animal','','',0,90,90,4,0,2136,2136,0,1.0,1.14286,1.0,0,11747.7,17621.550000000003,0,0,2.0,2000,2000,1,0,0,0,0,0,0,0,11747.7,17621.550000000003,0,7,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,'',0,3,1.0,1.5,1.0,1.0,1.0,0,0,0,0,0,0,0,0,1,0,0,'',0,20.0,17688),
(900057,0,0,0,0,0,42377,42378,42379,42380,'Hozen Party Animal','','',0,91,91,4,0,2136,2136,0,1.0,1.14286,1.0,0,14995.0,22492.5,0,0,4.0,2000,2000,1,0,0,0,0,0,0,0,14995.0,22492.5,0,7,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,'',0,3,1.0,3.0,1.0,1.0,1.0,0,0,0,0,0,0,0,0,1,0,0,'',0,20.0,17688),
(900058,0,0,0,0,0,42377,42378,42379,42380,'Hozen Party Animal','','',0,90,90,4,0,35,35,0,1.0,1.14286,1.0,0,11747.7,17621.550000000003,0,0,2.0,2000,2000,1,0,0,0,0,0,0,0,11747.7,17621.550000000003,0,7,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,'',0,3,1.0,2.0,1.0,1.0,1.0,0,0,0,0,0,0,0,0,1,0,0,'',0,10.0,17688),
(900059,0,0,0,0,0,42377,42378,42379,42380,'Hozen Party Animal','','',0,91,91,4,0,35,35,0,1.0,1.14286,1.0,0,14995.0,22492.5,0,0,4.0,2000,2000,1,0,0,0,0,0,0,0,14995.0,22492.5,0,7,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,'',0,3,1.0,3.0,1.0,1.0,1.0,0,0,0,0,0,0,0,0,1,0,0,'',0,10.0,17688),
(900060,0,0,0,0,0,42967,0,0,0,'Stout Brew Alemental','','',0,91,91,4,0,35,35,0,1.0,1.14286,1.0,1,14995.0,22492.5,0,0,2.8,2000,2000,1,0,0,0,0,0,0,0,14995.0,22492.5,0,4,0,0,900060,0,0,0,0,0,0,0,0,106851,115652,0,0,0,0,0,0,0,0,0,0,'',0,3,1.0,8.0,1.0,1.0,1.0,0,0,0,0,0,0,0,0,1,0,0,'',0,10.0,17688),
(900061,0,0,0,0,0,42967,0,0,0,'Stout Brew Alemental','','',0,91,91,4,0,35,35,0,1.0,1.14286,1.0,1,14995.0,22492.5,0,0,5.6,2000,2000,1,0,0,0,0,0,0,0,14995.0,22492.5,0,4,0,0,900061,0,0,0,0,0,0,0,0,106851,115652,0,0,0,0,0,0,0,0,0,0,'',0,3,1.0,12.0,1.0,1.0,1.0,0,0,0,0,0,0,0,0,1,0,0,'',0,10.0,17688),
(900062,0,0,0,0,0,42966,0,0,0,'Fizzy Brew Alemental','','',0,91,91,4,0,35,35,0,1.0,1.14286,1.0,1,14995.0,22492.5,0,0,2.8,2000,2000,1,0,0,0,0,0,0,0,14995.0,22492.5,0,4,0,0,900062,0,0,0,0,0,0,0,0,115650,116162,0,0,0,0,0,0,0,0,0,0,'',0,3,1.0,6.0,1.0,1.0,1.0,0,0,0,0,0,0,0,0,1,0,0,'',0,10.0,17688),
(900063,0,0,0,0,0,42966,0,0,0,'Fizzy Brew Alemental','','',0,91,91,4,0,35,35,0,1.0,1.14286,1.0,1,14995.0,22492.5,0,0,5.6,2000,2000,1,0,0,0,0,0,0,0,14995.0,22492.5,0,4,0,0,900063,0,0,0,0,0,0,0,0,115650,116162,0,0,0,0,0,0,0,0,0,0,'',0,3,1.0,9.5,1.0,1.0,1.0,0,0,0,0,0,0,0,0,1,0,0,'',0,10.0,17688),
(900064,0,0,0,0,0,42968,0,0,0,'Bubbling Brew Alemental','','',0,90,90,4,0,35,35,0,1.0,1.14286,1.0,1,11747.7,17621.550000000003,0,0,2.0,2000,2000,1,0,0,0,0,0,0,0,11747.7,17621.550000000003,0,4,0,0,900064,0,0,0,0,0,0,0,0,116155,128708,0,0,0,0,0,0,0,0,0,0,'',0,3,1.0,3.0,1.0,1.0,1.0,0,0,0,0,0,0,0,0,1,0,0,'',0,10.0,17688),
(900065,0,0,0,0,0,42968,0,0,0,'Bubbling Brew Alemental','','',0,91,91,4,0,35,35,0,1.0,1.14286,1.0,1,14995.0,22492.5,0,0,4.0,2000,2000,1,0,0,0,0,0,0,0,14995.0,22492.5,0,4,0,0,900065,0,0,0,0,0,0,0,0,116155,128708,0,0,0,0,0,0,0,0,0,0,'',0,3,1.0,5.0,1.0,1.0,1.0,0,0,0,0,0,0,0,0,1,0,0,'',0,10.0,17688),
(900066,0,0,0,0,0,41257,0,0,0,'Sodden Hozen Brawler','','',0,91,91,4,0,16,16,0,1.0,1.14286,1.0,1,14995.0,22492.5,0,0,2.0,2000,2000,1,0,0,0,0,0,0,0,14995.0,22492.5,0,7,0,0,0,0,0,0,0,0,0,0,0,0,107046,0,0,0,0,0,0,0,0,0,0,'',0,3,1.0,7.0,1.0,1.0,1.0,0,0,0,0,0,0,0,144,1,0,0,'',0,20.0,17688),
(900067,0,0,0,0,0,41257,0,0,0,'Sodden Hozen Brawler','','',0,92,92,4,0,16,16,0,1.0,1.14286,1.0,1,19140.3,28710.449999999997,0,0,4.0,2000,2000,1,0,0,0,0,0,0,0,19140.3,28710.449999999997,0,7,0,0,0,0,0,0,0,0,0,0,0,0,107046,0,0,0,0,0,0,0,0,0,0,'',0,3,1.0,11.0,1.0,1.0,1.0,0,0,0,0,0,0,0,144,1,0,0,'',0,20.0,17688),
(900068,0,0,0,0,0,42377,42378,42379,42380,'Hozen Party Animal','','',0,90,90,4,0,2136,2136,0,1.0,1.14286,1.0,0,11747.7,17621.550000000003,0,0,2.0,2000,2000,1,0,0,0,0,0,0,0,11747.7,17621.550000000003,0,7,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,'',0,3,1.0,2.0,1.0,1.0,1.0,0,0,0,0,0,0,0,0,1,0,0,'',0,20.0,17688),
(900069,0,0,0,0,0,42377,42378,42379,42380,'Hozen Party Animal','','',0,91,91,4,0,2136,2136,0,1.0,1.14286,1.0,0,14995.0,22492.5,0,0,4.0,2000,2000,1,0,0,0,0,0,0,0,14995.0,22492.5,0,7,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,'',0,3,1.0,3.0,1.0,1.0,1.0,0,0,0,0,0,0,0,0,1,0,0,'',0,20.0,17688);

-- creature_template updates (difficulty slots, loot ids, SmartAI)
UPDATE `creature_template` SET `difficulty_entry_1`=900039, `difficulty_entry_2`=900040 WHERE `entry`=56637;
UPDATE `creature_template` SET `difficulty_entry_1`=900041, `difficulty_entry_2`=900042 WHERE `entry`=56717;
UPDATE `creature_template` SET `difficulty_entry_1`=900043 WHERE `entry`=56783;
UPDATE `creature_template` SET `difficulty_entry_1`=900044, `difficulty_entry_2`=900045 WHERE `entry`=56849;
UPDATE `creature_template` SET `difficulty_entry_1`=900046, `difficulty_entry_2`=900047 WHERE `entry`=56862;
UPDATE `creature_template` SET `difficulty_entry_1`=900048, `difficulty_entry_2`=900049 WHERE `entry`=56863;
UPDATE `creature_template` SET `difficulty_entry_1`=900050, `difficulty_entry_2`=900051 WHERE `entry`=56865;
UPDATE `creature_template` SET `difficulty_entry_1`=900052, `difficulty_entry_2`=900053 WHERE `entry`=56867;
UPDATE `creature_template` SET `difficulty_entry_1`=900054, `difficulty_entry_2`=900055 WHERE `entry`=56924;
UPDATE `creature_template` SET `difficulty_entry_1`=900056, `difficulty_entry_2`=900057 WHERE `entry`=56927;
UPDATE `creature_template` SET `difficulty_entry_1`=900058, `difficulty_entry_2`=900059 WHERE `entry`=57097;
UPDATE `creature_template` SET `difficulty_entry_1`=900060, `difficulty_entry_2`=900061 WHERE `entry`=59519;
UPDATE `creature_template` SET `difficulty_entry_1`=900062, `difficulty_entry_2`=900063 WHERE `entry`=59520;
UPDATE `creature_template` SET `difficulty_entry_1`=900064, `difficulty_entry_2`=900065 WHERE `entry`=59521;
UPDATE `creature_template` SET `difficulty_entry_1`=900066, `difficulty_entry_2`=900067 WHERE `entry`=59605;
UPDATE `creature_template` SET `difficulty_entry_1`=900068, `difficulty_entry_2`=900069 WHERE `entry`=59684;

-- gameobject_template
DELETE FROM `gameobject_template` WHERE `entry` IN (211126,211132,211133,211134,211135,211136,211137,211138,211313,211314,211315,211319,211326,211989,213795);
INSERT INTO `gameobject_template` (`entry`,`type`,`displayId`,`name`,`IconName`,`castBarCaption`,`unk1`,`faction`,`flags`,`size`,`questItem1`,`questItem2`,`questItem3`,`questItem4`,`questItem5`,`questItem6`,`data0`,`data1`,`data2`,`data3`,`data4`,`data5`,`data6`,`data7`,`data8`,`data9`,`data10`,`data11`,`data12`,`data13`,`data14`,`data15`,`data16`,`data17`,`data18`,`data19`,`data20`,`data21`,`data22`,`data23`,`data24`,`data25`,`data26`,`data27`,`data28`,`data29`,`data30`,`data31`,`unkInt32`,`AIName`,`ScriptName`,`WDBVerified`) VALUES
(211126,0,9244,'Carrot Door','','','',114,0,3.0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,'','',19116),
(211132,0,11041,'Sliding Door','','','',1375,34,1.0,0,0,0,0,0,0,0,1745,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,'','',19116),
(211133,0,11041,'Sliding Door','','','',1375,32,1.0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,'','',19116),
(211134,0,11041,'Sliding Door','','','',1375,32,1.0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,'','',19116),
(211135,0,11041,'Sliding Door','','','',0,32,1.0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,'','',19116),
(211136,0,11041,'Sliding Door','','','',1375,32,1.0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,'','',19116),
(211137,0,11041,'Sliding Door','','','',0,32,1.0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,'','',19116),
(211138,0,11367,'Mysteriously-Shaking Keg','','','',1375,32,1.0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,'','',19116),
(211313,10,2270,'Brew Tap','','','',0,34,0.5,0,0,0,0,0,0,2029,0,31137,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,'','',19116),
(211314,10,2270,'Brew Tap','','','',0,34,0.5,0,0,0,0,0,0,2029,0,31138,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,'','',19116),
(211315,10,2270,'Brew Tap','','','',0,34,0.5,0,0,0,0,0,0,2029,0,31139,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,'','',19116),
(211319,10,11350,'Relief Bucket','','','',0,0,0.3,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,112954,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,1,0,0,0,0,0,0,'','',19116),
(211326,3,6867,'Tap Tool','','','',0,0,1.0,0,0,0,0,0,0,1691,41530,0,1,0,0,0,0,0,0,0,0,0,0,37733,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,'','',19116),
(211989,0,11638,'Challenge Mode - Start Door (Line 2.0)','','','',1375,0,2.0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,'','',19342),
(213795,3,11267,'Stormstout Secrets','','','',0,0,1.0,86431,0,0,0,0,0,43,42921,30,1,0,0,0,0,0,0,0,0,0,0,19676,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,'','',19116);

-- creature_loot_template (10 loot ids)
DELETE FROM `creature_loot_template` WHERE `entry` IN (900039,900041,900046,900047,900060,900061,900062,900063,900064,900065);
INSERT INTO `creature_loot_template` (`entry`,`item`,`ChanceOrQuestChance`,`lootmode`,`groupid`,`mincountOrRef`,`maxcount`) VALUES
(900039,81061,0.0,1,1,1,1),
(900039,81064,0.0,1,1,1,1),
(900039,81080,0.0,1,1,1,1),
(900039,81133,0.0,1,1,1,1),
(900039,81134,0.0,1,1,1,1),
(900041,71715,7.0,1,0,1,1),
(900041,81065,0.0,1,1,1,1),
(900041,81076,0.0,1,1,1,1),
(900041,81077,0.0,1,1,1,1),
(900041,81135,0.0,1,1,1,1),
(900041,81136,0.0,1,1,1,1),
(900046,72988,128.218,1,0,1,5),
(900046,95470,0.3,1,0,1,1),
(900046,95471,0.3,1,0,1,1),
(900047,72988,128.218,1,0,1,5),
(900047,95470,0.3,1,0,1,1),
(900047,95471,0.3,1,0,1,1),
(900060,81202,196.414,1,0,2,4),
(900060,89112,11.4219,1,0,1,1),
(900060,103651,-100.0,1,0,1,1),
(900060,104212,0.05,1,0,1,1),
(900061,81202,196.414,1,0,2,4),
(900061,89112,11.4219,1,0,1,1),
(900061,103651,-100.0,1,0,1,1),
(900061,104212,0.05,1,0,1,1),
(900062,81202,201.252,1,0,2,4),
(900062,89112,13.6707,1,0,1,1),
(900062,103651,-100.0,1,0,1,1),
(900063,81202,201.252,1,0,2,4),
(900063,89112,13.6707,1,0,1,1),
(900063,103651,-100.0,1,0,1,1),
(900064,81202,210.225,1,0,2,4),
(900064,89112,11.4955,1,0,1,1),
(900064,103651,-100.0,1,0,1,1),
(900064,104212,0.05,1,0,1,1),
(900064,104221,0.01,1,0,1,1),
(900065,81202,210.225,1,0,2,4),
(900065,89112,11.4955,1,0,1,1),
(900065,103651,-100.0,1,0,1,1),
(900065,104212,0.05,1,0,1,1),
(900065,104221,0.01,1,0,1,1);

-- gameobject_loot_template (2 loot ids)
DELETE FROM `gameobject_loot_template` WHERE `entry` IN (41530,42921);
INSERT INTO `gameobject_loot_template` (`entry`,`item`,`ChanceOrQuestChance`,`lootmode`,`groupid`,`mincountOrRef`,`maxcount`) VALUES
(41530,80546,86.0,1,0,1,1),
(42921,86431,80.0,1,0,1,1);

-- creature spawns
DELETE FROM `creature` WHERE `guid` IN (8202655,8202656,8202657,8202658,8202659,8202660,8202661,8202662,8202663,8202664,8202665,8202666,8202667,8202668,8202669,8202670,8202671,8202672,8202673,8202674,8202675,8202676,8202677,8202678,8202679,8202680,8202681,8202682,8202683,8202684,8202685,8202686,8202687,8202688,8202689,8202690,8202691,8202692,8202693,8202694,8202695,8202696,8202697,8202698,8202699,8202700,8202701,8202702,8202703,8202704,8202705,8202706);
INSERT INTO `creature` (`guid`,`id`,`map`,`spawnMask`,`phaseId`,`phaseGroup`,`modelid`,`equipment_id`,`position_x`,`position_y`,`position_z`,`orientation`,`spawntimesecs`,`spawndist`,`currentwaypoint`,`curhealth`,`curmana`,`MovementType`,`npcflag`,`unit_flags`,`dynamicflags`) VALUES
(8202655,57097,961,262,0,0,0,0,-768.445,1428.78,139.58,3.8363,7200,0,0,2,0,0,0,0,0),
(8202656,57097,961,262,0,0,0,0,-740.951,1384.77,146.699,4.16833,7200,0,0,2,0,0,0,0,0),
(8202657,57097,961,262,0,0,0,0,-784.91,1371.2,146.719,6.01402,7200,0,0,2,0,0,0,0,0),
(8202658,57097,961,262,0,0,0,0,-734.919,1380.32,146.706,4.16833,7200,0,0,2,0,0,0,0,0),
(8202659,57097,961,262,0,0,0,0,-739.746,1379.36,146.704,4.16833,7200,0,0,2,0,0,0,0,0),
(8202660,57097,961,262,0,0,0,0,-789.649,1364.73,146.728,5.34093,7200,0,0,2,0,0,0,0,0),
(8202661,57097,961,262,0,0,0,0,-783.725,1365.47,146.728,6.01402,7200,0,0,2,0,0,0,0,0),
(8202662,57097,961,262,0,0,0,0,-729.605,1376.46,146.715,3.48975,7200,0,0,2,0,0,0,0,0),
(8202663,57097,961,262,0,0,0,0,-733.609,1372.46,146.739,3.48975,7200,0,0,2,0,0,0,0,0),
(8202664,57097,961,262,0,0,0,0,-727.792,1371.47,146.681,3.48975,7200,0,0,2,0,0,0,0,0),
(8202665,57097,961,262,0,0,0,0,-753.915,1363.18,146.864,4.76838,7200,0,0,2,0,0,0,0,0),
(8202666,57097,961,262,0,0,0,0,-758.378,1362.13,146.858,5.0794,7200,0,0,2,0,0,0,0,0),
(8202667,65376,961,262,0,0,0,0,-657.281,1087.68,158.7,5.69293,7200,0,0,196971,0,0,0,0,0),
(8202668,65376,961,262,0,0,0,0,-655.411,1092.8,158.7,3.96662,7200,0,0,196971,0,0,0,0,0),
(8202669,65376,961,262,0,0,0,0,-660.903,1090.18,158.7,0.569775,7200,0,0,196971,0,0,0,0,0),
(8202670,65376,961,262,0,0,0,0,-703.31,1075.1,147.197,4.26978,7200,0,0,196971,0,0,0,0,0),
(8202671,65376,961,262,0,0,0,0,-709.13,1080.19,147.197,4.26978,7200,0,0,196971,0,0,0,0,0),
(8202672,65376,961,262,0,0,0,0,-674.097,1175.21,139.155,2.89927,7200,0,0,196971,0,0,0,0,0),
(8202673,65376,961,262,0,0,0,0,-669.303,1160.86,139.173,2.88984,7200,0,0,196971,0,0,0,0,0),
(8202674,65376,961,262,0,0,0,0,-684.685,1187.63,139.172,4.12607,7200,0,0,196971,0,0,0,0,0),
(8202675,65376,961,262,0,0,0,0,-676.663,1148.36,139.17,2.22304,7200,0,0,196971,0,0,0,0,0),
(8202676,65376,961,262,0,0,0,0,-699.089,1185.26,139.168,5.59475,7200,0,0,196971,0,0,0,0,0),
(8202677,65376,961,262,0,0,0,0,-681.836,1146.7,139.17,1.97171,7200,0,0,196971,0,0,0,0,0),
(8202678,65376,961,262,0,0,0,0,-709.548,1182.28,139.168,5.10466,7200,0,0,196971,0,0,0,0,0),
(8202679,65376,961,262,0,0,0,0,-720.064,1105.31,139.138,4.10092,7200,0,0,196971,0,0,0,0,0),
(8202680,65376,961,262,0,0,0,0,-692.222,1142.53,139.167,1.99527,7200,0,0,196971,0,0,0,0,0),
(8202681,65376,961,262,0,0,0,0,-724.536,1109.02,139.166,4.10092,7200,0,0,196971,0,0,0,0,0),
(8202682,65376,961,262,0,0,0,0,-722.979,1179.38,139.167,5.56648,7200,0,0,196971,0,0,0,0,0),
(8202683,65376,961,262,0,0,0,0,-698.279,1143.28,139.15,1.99527,7200,0,0,196971,0,0,0,0,0),
(8202684,65376,961,262,0,0,0,0,-731.574,1172.61,139.172,5.56648,7200,0,0,196971,0,0,0,0,0),
(8202685,65376,961,262,0,0,0,0,-748.074,1166.38,139.165,5.45495,7200,0,0,196971,0,0,0,0,0),
(8202686,65376,961,262,0,0,0,0,-704.741,1141.44,139.141,2.22854,7200,0,0,196971,0,0,0,0,0),
(8202687,65376,961,262,0,0,0,0,-751.185,1158.82,139.15,6.044,7200,0,0,196971,0,0,0,0,0),
(8202688,65376,961,262,0,0,0,0,-710.946,1138.08,139.117,1.45413,7200,0,0,196971,0,0,0,0,0),
(8202689,65376,961,262,0,0,0,0,-749.793,1148.87,139.148,0.696225,7200,0,0,196971,0,0,0,0,0),
(8202690,65376,961,262,0,0,0,0,-745.926,1140.6,139.152,0.0679064,7200,0,0,196971,0,0,0,0,0),
(8202691,59519,961,262,0,0,0,0,-656.278,1187.56,139.104,0.0985465,7200,0,0,3151528,0,0,0,0,0),
(8202692,64361,961,262,0,0,0,0,-668.155,1132.58,166.89,1.87028,7200,0,0,1,0,0,0,0,0),
(8202693,54020,961,262,0,0,0,0,-672.475,1169.96,142.727,3.97638,7200,0,0,1,0,0,0,0,0),
(8202694,54020,961,262,0,0,0,0,-727.96,1175.94,142.522,6.19677,7200,0,0,1,0,0,0,0,0),
(8202695,54020,961,262,0,0,0,0,-697.542,1138.21,143.421,6.11264,7200,0,0,1,0,0,0,0,0),
(8202696,59394,961,262,0,0,0,0,-678.611,1156.4,139.158,4.95107,7200,0,0,1,0,0,0,0,0),
(8202697,59394,961,262,0,0,0,0,-744.33,1151.28,139.147,2.69352,7200,0,0,1,0,0,0,0,0),
(8202698,59394,961,262,0,0,0,0,-696.758,1152.33,139.64,5.8766,7200,0,0,1,0,0,0,0,0),
(8202699,59394,961,262,0,0,0,0,-722.774,1170.21,139.156,2.33019,7200,0,0,1,0,0,0,0,0),
(8202700,59394,961,262,0,0,0,0,-711.516,1179.34,139.169,2.74645,7200,0,0,1,0,0,0,0,0),
(8202701,59394,961,262,0,0,0,0,-684.561,1179.22,139.161,1.88016,7200,0,0,1,0,0,0,0,0),
(8202702,59394,961,262,0,0,0,0,-675.972,1167.55,139.179,6.10481,7200,0,0,1,0,0,0,0,0),
(8202703,59521,961,262,0,0,0,0,-710.488,1169.26,140.64,0.647557,7200,0,0,468000,0,0,0,0,0),
(8202704,59521,961,262,0,0,0,0,-707.276,1162.78,140.641,1.37719,7200,0,0,468000,0,0,0,0,0),
(8202705,59521,961,262,0,0,0,0,-709.264,1165.29,140.64,1.01277,7200,0,0,468000,0,0,0,0,0),
(8202706,59521,961,262,0,0,0,0,-708.632,1173.74,140.64,0.439426,7200,0,0,468000,0,0,0,0,0);

-- creature_addon
DELETE FROM `creature_addon` WHERE `guid` IN (8202693,8202694,8202695);
INSERT INTO `creature_addon` (`guid`,`path_id`,`mount`,`bytes1`,`bytes2`,`emote`,`auras`) VALUES
(8202693,0,0,0,0,0,'114379'),
(8202694,0,0,0,0,0,'114379'),
(8202695,0,0,0,0,0,'114379');

-- gameobject spawns
DELETE FROM `gameobject` WHERE `guid` IN (901370,901371,901372);
INSERT INTO `gameobject` (`guid`,`id`,`map`,`spawnMask`,`phaseId`,`phaseGroup`,`position_x`,`position_y`,`position_z`,`orientation`,`rotation0`,`rotation1`,`rotation2`,`rotation3`,`spawntimesecs`,`animprogress`,`state`) VALUES
(901370,211126,961,262,0,0,-678.101,1251.45,162.037,4.06543,0.0,0.0,0.0,1.0,7200,255,1),
(901371,211674,961,256,0,0,-734.767,1276.05,116.107,1.84356,0.0,0.0,0.796679,0.604403,7200,255,1),
(901372,211989,961,256,0,0,-739.015,1291.8,116.544,1.82393,0.0,0.0,0.790707,0.612195,7200,255,1);

-- creature_text
DELETE FROM `creature_text` WHERE `entry` IN (45979,56717,59074,59704,59822,64361);
INSERT INTO `creature_text` (`entry`,`groupid`,`id`,`text`,`type`,`language`,`probability`,`emote`,`duration`,`sound`,`comment`) VALUES
(45979,0,0,'A stray jolt from the Foe Reaper has disrupted the foundry controls!',41,0,100.0,0,0,0,'General Purpose Bunny JMF'),
(45979,1,0,'The molten slag begins to bubble furiously!',41,0,100.0,0,0,0,'General Purpose Bunny JMF'),
(45979,2,0,'The cannon was rigged with an explosive trap!',41,0,100.0,0,0,0,'General Purpose Bunny JMF to Player'),
(45979,3,0,'A shadowy figure appears in the ship\'s cabin!',41,0,100.0,0,0,0,'General Purpose Bunny JMF to Player'),
(45979,4,0,'The Nightmare Elixir takes hold!',41,0,100.0,0,0,0,'General Purpose Bunny JMF to Steam Valve'),
(45979,5,0,'The nightmare shifts!',41,0,100.0,0,0,0,'General Purpose Bunny JMF to Player'),
(45979,6,0,'Save Emme Harrington!',41,0,100.0,0,0,0,'General Purpose Bunny JMF to Player'),
(45979,7,0,'Save Erik Harrington!',41,0,100.0,0,0,0,'General Purpose Bunny JMF'),
(45979,8,0,'Save Calissa Harrington!',41,0,100.0,0,0,0,'General Purpose Bunny JMF'),
(45979,9,0,'The Nightmare Elixir wears off!',41,0,100.0,0,0,0,'General Purpose Bunny JMF'),
(56717,0,0,'Oh yeah!',14,0,100.0,0,0,28789,'Hoptallus Aggro'),
(56717,1,0,'Gonna spins around!',14,0,100.0,0,0,0,'Hoptallus Furlwind'),
(56717,2,0,'Urp...eats too many carrots...',14,0,100.0,0,0,0,'Hoptallus Carrot Breath'),
(56717,3,0,'You have...turnip...for a head...',14,0,100.0,0,0,0,'Hoptallus Death'),
(56717,4,0,'Hoptallus lets out a loud screech! The virmen are coming!',41,0,100.0,0,0,0,'Hoptallus Screech'),
(56717,5,0,'You die!',14,0,100.0,0,0,0,'Hoptallus Kill Player'),
(59074,0,0,'Yes yes yes! So close! Just a pinch of...oops...',14,0,100.0,0,0,28290,'Uncle Gao to Player'),
(59074,1,0,'Too bitter! Just a drop of honey, and a cane of sugar, and maybe some corn? Yes, more corn!',14,0,100.0,0,0,28291,'Uncle Gao'),
(59074,2,0,'Yes, yes, yes...no, no, no no no! Yes! No! Peppers!',14,0,100.0,0,0,28292,'Uncle Gao'),
(59074,3,0,'Ahh! Help! What is that thing!? It doesn\'t matter, nothing will stop me now, not when I\'m so close! Maybe if I don\'t look at it...?',14,0,100.0,0,0,28289,'Uncle Gao'),
(59074,4,0,'Is it...can it be???',12,0,100.0,0,0,28293,'Uncle Gao to Player'),
(59074,5,0,'Such harmony of flavor, such heady aroma! It is...the perfect brew!',12,0,100.0,0,0,28294,'Uncle Gao to Player'),
(59074,6,0,'The name of Stormstout will be sung once again throughout the hills!',14,0,100.0,0,0,28295,'Uncle Gao to Player'),
(59074,7,0,'You again! Don\'t you see? I have made a name for myself at last: I have brewed perfection!',14,0,100.0,0,0,31999,'Uncle Gao to Player'),
(59074,8,0,'Details, details.',12,0,100.0,0,0,32000,'Uncle Gao to Player'),
(59074,9,0,'Look, \"Chen Stormstout,\" we all can\'t be heroes, running from our responsibilities, tromping around the Dread Wastes, saving the world. Some of us are \"Artists.\"',12,0,100.0,0,0,32001,'Uncle Gao to Player'),
(59074,10,0,'Yes, yes. That happens.',12,0,100.0,0,0,32002,'Uncle Gao to Player'),
(59704,0,0,'I am not Zan - I am Chen Stormstout!',12,0,100.0,0,0,29766,'Chen Stormstout to Player'),
(59704,1,0,'Tell me, what has happened here?',12,0,100.0,0,0,29767,'Chen Stormstout to Player'),
(59704,2,0,'Where are the other Stormstouts? Why are hozen all over the brewery?',12,0,100.0,0,0,29768,'Chen #4'),
(59704,3,0,'Auntie Stormstout...why is the brewery abandoned?',12,0,100.0,0,0,29769,'Chen #5'),
(59704,4,0,'I see - and where is Uncle Gao?',12,0,100.0,0,0,29770,'Chen #6'),
(59704,5,0,'There is no time for cookies! Well, one cookie. Just one.',12,0,100.0,0,0,29771,'Chen #7'),
(59704,6,0,'Wait - these are ghost cookies. These aren\'t filling at all!',12,0,100.0,0,0,29772,'Chen #8'),
(59822,0,0,'Oh, hello Zan, it is good of you to visit ol\' Auntie Stormstout.',12,0,100.0,0,0,28528,'Auntie Stormstout to Player'),
(59822,1,0,'Oh Zan! You remind me so much of your father.',12,0,100.0,0,0,28529,'Auntie Stormstout to Player'),
(59822,2,0,'It is certainly a nice day outside!',12,0,100.0,0,0,28530,'Auntie Stormstout to Player'),
(59822,3,0,'Have you seen the size of Esme\'s turnips?',12,0,100.0,0,0,28531,'Auntie #4'),
(59822,4,0,'Abandoned? Oh heavens no! Uncle Gao is in charge while the others are beyond the wall. Isn\'t that exciting?',12,0,100.0,0,0,28532,'Auntie #5'),
(59822,5,0,'I have some cookies for you!',12,0,100.0,0,0,28533,'Auntie #6'),
(64361,0,0,'Uncle Gao, this brewery was left in your care! What have you done?',12,0,100.0,0,0,31998,'Chen Stormstout to Player'),
(64361,1,0,'At what cost? The Brewery is trashed! Infested!',12,0,100.0,0,0,29773,'Chen Stormstout to Player'),
(64361,2,0,'And there are Virmen in the main store!',12,0,100.0,0,0,29774,'Chen Stormstout to Player'),
(64361,3,0,'I think the brewery might be on fire.',12,0,100.0,0,0,29775,'Chen Stormstout to Player');

-- conditions
DELETE FROM `conditions` WHERE `SourceTypeOrReferenceId`=1 AND `SourceGroup`=900046 AND `SourceEntry`=95470 AND `SourceId`=0 AND `ElseGroup`=0 AND `ConditionTypeOrReference`=7 AND `ConditionTarget`=0 AND `ConditionValue1`=755 AND `ConditionValue2`=500 AND `ConditionValue3`=0;
DELETE FROM `conditions` WHERE `SourceTypeOrReferenceId`=1 AND `SourceGroup`=900047 AND `SourceEntry`=95470 AND `SourceId`=0 AND `ElseGroup`=0 AND `ConditionTypeOrReference`=7 AND `ConditionTarget`=0 AND `ConditionValue1`=755 AND `ConditionValue2`=500 AND `ConditionValue3`=0;
DELETE FROM `conditions` WHERE `SourceTypeOrReferenceId`=1 AND `SourceGroup`=900046 AND `SourceEntry`=95471 AND `SourceId`=0 AND `ElseGroup`=0 AND `ConditionTypeOrReference`=7 AND `ConditionTarget`=0 AND `ConditionValue1`=755 AND `ConditionValue2`=500 AND `ConditionValue3`=0;
DELETE FROM `conditions` WHERE `SourceTypeOrReferenceId`=1 AND `SourceGroup`=900047 AND `SourceEntry`=95471 AND `SourceId`=0 AND `ElseGroup`=0 AND `ConditionTypeOrReference`=7 AND `ConditionTarget`=0 AND `ConditionValue1`=755 AND `ConditionValue2`=500 AND `ConditionValue3`=0;
INSERT INTO `conditions` (`SourceTypeOrReferenceId`,`SourceGroup`,`SourceEntry`,`SourceId`,`ElseGroup`,`ConditionTypeOrReference`,`ConditionTarget`,`ConditionValue1`,`ConditionValue2`,`ConditionValue3`,`NegativeCondition`,`ErrorType`,`ErrorTextId`,`ScriptName`,`Comment`) VALUES
(1,900046,95470,0,0,7,0,755,500,0,0,0,0,'',NULL),
(1,900047,95470,0,0,7,0,755,500,0,0,0,0,'',NULL),
(1,900046,95471,0,0,7,0,755,500,0,0,0,0,'',NULL),
(1,900047,95471,0,0,7,0,755,500,0,0,0,0,'',NULL);

-- creature_template_addon
DELETE FROM `creature_template_addon` WHERE `entry` IN (45979,56637,56717,56731,56783,56849,56862,56863,56865,56867,56924,56927,57097,59074,59075,59519,59520,59521,59605,59684,59704,59822,60276,64361,65375,65376,65392);
INSERT INTO `creature_template_addon` (`entry`,`path_id`,`mount`,`bytes1`,`bytes2`,`emote`,`auras`) VALUES
(45979,0,0,0,1,0,''),
(56637,0,0,0,1,0,''),
(56717,0,0,0,1,0,''),
(56731,0,0,0,1,0,'106787'),
(56783,0,0,0,1,0,''),
(56849,0,0,0,1,0,'107019'),
(56862,0,0,0,1,0,''),
(56863,0,0,0,1,0,'107022'),
(56865,0,0,50331648,1,0,''),
(56867,0,0,50331648,1,0,''),
(56924,0,0,0,1,0,''),
(56927,0,0,1,1,0,'106650'),
(57097,0,0,1,1,0,''),
(59074,0,0,0,1,0,''),
(59075,0,0,8,1,0,'113124 114370'),
(59519,0,0,50331648,1,0,'114930 116259'),
(59520,0,0,50331648,1,0,'114934 116259'),
(59521,0,0,50331648,1,0,'114931 116259'),
(59605,0,0,0,1,0,'18950'),
(59684,0,0,0,1,0,''),
(59704,0,0,0,1,0,''),
(59822,0,0,0,1,0,'115618'),
(60276,0,0,50331648,1,0,'136684'),
(64361,0,0,0,1,0,''),
(65375,0,0,0,1,0,'113124 114370'),
(65376,0,0,0,1,0,'114370 114955'),
(65392,0,0,0,1,0,'128188');

-- creature_equip_template
DELETE FROM `creature_equip_template` WHERE `entry` IN (56927);
INSERT INTO `creature_equip_template` (`entry`,`id`,`itemEntry1`,`itemEntry2`,`itemEntry3`) VALUES
(56927,1,80580,0,0);

-- instance_encounters
DELETE FROM `instance_encounters` WHERE `entry` IN (1414);
INSERT INTO `instance_encounters` (`entry`,`creditType`,`creditEntry`,`lastEncounterDungeon`,`comment`) VALUES
(1414,0,59479,465,'Yan-Zhu the Uncasked');

-- access_requirement
DELETE FROM `access_requirement` WHERE `mapId`=961 AND `difficulty`=8;
INSERT INTO `access_requirement` (`mapId`,`difficulty`,`level_min`,`level_max`,`item`,`item2`,`quest_done_A`,`quest_done_H`,`completed_achievement`,`quest_failed_text`,`iLvl`,`comment`) VALUES
(961,8,90,0,0,0,0,0,6456,NULL,0,'Stormstout Brewery - Entrance');

-- spell_script_names
DELETE FROM `spell_script_names` WHERE `spell_id`=106546 AND `ScriptName`='spell_bloat_aura';
DELETE FROM `spell_script_names` WHERE `spell_id`=106614 AND `ScriptName`='spell_brew_barrel_ride';
DELETE FROM `spell_script_names` WHERE `spell_id`=106807 AND `ScriptName`='spell_ground_pound';
DELETE FROM `spell_script_names` WHERE `spell_id`=106851 AND `ScriptName`='spell_blackout_brew';
DELETE FROM `spell_script_names` WHERE `spell_id`=107205 AND `ScriptName`='spell_spicy_explosion';
DELETE FROM `spell_script_names` WHERE `spell_id`=107351 AND `ScriptName`='spell_barrel_hostile';
DELETE FROM `spell_script_names` WHERE `spell_id`=111666 AND `ScriptName`='spell_hammer_smash';
DELETE FROM `spell_script_names` WHERE `spell_id`=112944 AND `ScriptName`='spell_carrot_breath';
DELETE FROM `spell_script_names` WHERE `spell_id`=112992 AND `ScriptName`='spell_furlwind';
DELETE FROM `spell_script_names` WHERE `spell_id`=114291 AND `ScriptName`='spell_sb_explosive_brew';
DELETE FROM `spell_script_names` WHERE `spell_id`=114386 AND `ScriptName`='spell_carbonation';
DELETE FROM `spell_script_names` WHERE `spell_id`=114459 AND `ScriptName`='spell_fizzi_buble';
DELETE FROM `spell_script_names` WHERE `spell_id`=128254 AND `ScriptName`='spell_brew_finale_wheat_effect';
DELETE FROM `spell_script_names` WHERE `spell_id`=128256 AND `ScriptName`='spell_brew_finale_wheat_effect';
DELETE FROM `spell_script_names` WHERE `spell_id`=128258 AND `ScriptName`='spell_brew_finale_wheat_effect';
INSERT INTO `spell_script_names` (`spell_id`,`ScriptName`) VALUES
(106546,'spell_bloat_aura'),
(106614,'spell_brew_barrel_ride'),
(106807,'spell_ground_pound'),
(106851,'spell_blackout_brew'),
(107205,'spell_spicy_explosion'),
(107351,'spell_barrel_hostile'),
(111666,'spell_hammer_smash'),
(112944,'spell_carrot_breath'),
(112992,'spell_furlwind'),
(114291,'spell_sb_explosive_brew'),
(114386,'spell_carbonation'),
(114459,'spell_fizzi_buble'),
(128254,'spell_brew_finale_wheat_effect'),
(128256,'spell_brew_finale_wheat_effect'),
(128258,'spell_brew_finale_wheat_effect');

-- ScriptName bindings
UPDATE `creature_template` SET `ScriptName`='npc_sb_hopling' WHERE `entry`=56631;
UPDATE `creature_template` SET `ScriptName`='npc_barrel' WHERE `entry`=56682;
UPDATE `creature_template` SET `ScriptName`='npc_explosive_hopper' WHERE `entry`=56718;
UPDATE `creature_template` SET `ScriptName`='npc_habanero_brew' WHERE `entry`=56731;
UPDATE `creature_template` SET `ScriptName`='npc_carbonate_trigger' WHERE `entry`=56746;
UPDATE `creature_template` SET `ScriptName`='npc_suds_trigger' WHERE `entry`=56748;
UPDATE `creature_template` SET `ScriptName`='npc_hozen_hollerer' WHERE `entry`=56783;
UPDATE `creature_template` SET `ScriptName`='npc_hozen_bouncer' WHERE `entry`=56849;
UPDATE `creature_template` SET `ScriptName`='npc_aqua_dancer' WHERE `entry`=56865;
UPDATE `creature_template` SET `ScriptName`='npc_fiery_trickster' WHERE `entry`=56867;
UPDATE `creature_template` SET `ScriptName`='npc_controlled_hozen' WHERE `entry`=56924;
UPDATE `creature_template` SET `ScriptName`='npc_hozen_party_animal' WHERE `entry`=56927;
UPDATE `creature_template` SET `ScriptName`='npc_hozen_party_animal' WHERE `entry`=57097;
UPDATE `creature_template` SET `ScriptName`='boss_ook_ook' WHERE `entry`=57963;
UPDATE `creature_template` SET `ScriptName`='npc_carrot_breath_stalker' WHERE `entry`=59018;
UPDATE `creature_template` SET `ScriptName`='npc_uncle_gao' WHERE `entry`=59074;
UPDATE `creature_template` SET `ScriptName`='npc_hammer_bopper' WHERE `entry`=59426;
UPDATE `creature_template` SET `ScriptName`='npc_sb_hopling' WHERE `entry`=59458;
UPDATE `creature_template` SET `ScriptName`='npc_sb_hopling' WHERE `entry`=59459;
UPDATE `creature_template` SET `ScriptName`='npc_sb_hopling' WHERE `entry`=59461;
UPDATE `creature_template` SET `ScriptName`='boss_yanzhu' WHERE `entry`=59479;
UPDATE `creature_template` SET `ScriptName`='npc_yeasty_alemental' WHERE `entry`=59494;
UPDATE `creature_template` SET `ScriptName`='npc_wall_of_suds' WHERE `entry`=59512;
UPDATE `creature_template` SET `ScriptName`='npc_bloated_alemental' WHERE `entry`=59518;
UPDATE `creature_template` SET `ScriptName`='npc_stout_alemental' WHERE `entry`=59519;
UPDATE `creature_template` SET `ScriptName`='npc_fizzy_alemental' WHERE `entry`=59520;
UPDATE `creature_template` SET `ScriptName`='npc_bubbling_alemental' WHERE `entry`=59521;
UPDATE `creature_template` SET `ScriptName`='npc_sudsy_alemental' WHERE `entry`=59522;
UPDATE `creature_template` SET `ScriptName`='npc_hammer' WHERE `entry`=59539;
UPDATE `creature_template` SET `ScriptName`='npc_controlled_hozen' WHERE `entry`=59605;
UPDATE `creature_template` SET `ScriptName`='npc_hozen_party_animal' WHERE `entry`=59684;
UPDATE `creature_template` SET `ScriptName`='npc_chen_stormstout' WHERE `entry`=59704;
UPDATE `creature_template` SET `ScriptName`='npc_fizzy_bubble' WHERE `entry`=59799;
UPDATE `creature_template` SET `ScriptName`='npc_auntie_stormstout' WHERE `entry`=59822;
UPDATE `creature_template` SET `ScriptName`='npc_chen_yanzhu' WHERE `entry`=64361;
UPDATE `creature_template` SET `ScriptName`='npc_nibbler' WHERE `entry`=65392;
UPDATE `creature_template` SET `ScriptName`='npc_bubble_shield' WHERE `entry`=65522;

-- Stub scripts replaced by the ones above
UPDATE `creature_template` SET `ScriptName`='' WHERE `entry` IN (56862,56863,59551,60276);
DELETE FROM `areatrigger_scripts` WHERE `entry` IN (7998,8355);
INSERT INTO `areatrigger_scripts` (`entry`,`ScriptName`) VALUES
(7998,'at_stormstout_intro');

-- conditions (spell targets)
DELETE FROM `conditions` WHERE `SourceTypeOrReferenceId` IN (13,17) AND `SourceEntry` IN (114379);
INSERT INTO `conditions` (`SourceTypeOrReferenceId`,`SourceGroup`,`SourceEntry`,`SourceId`,`ElseGroup`,`ConditionTypeOrReference`,`ConditionTarget`,`ConditionValue1`,`ConditionValue2`,`ConditionValue3`,`NegativeCondition`,`ErrorType`,`ErrorTextId`,`ScriptName`,`Comment`) VALUES
(13,1,114379,0,0,29,1,54020,15,0,0,0,0,'',NULL);

-- Creature templates: level, class and hostile faction
UPDATE `creature_template` SET `minlevel`=87, `maxlevel`=87, `exp`=4, `unit_class`=1, `faction_A`=16, `faction_H`=16 WHERE `entry` IN (56637,56717);
UPDATE `creature_template` SET `minlevel`=86, `maxlevel`=86, `exp`=4, `unit_class`=1, `faction_A`=190, `faction_H`=190 WHERE `entry`=56783;
UPDATE `creature_template` SET `minlevel`=85, `maxlevel`=85, `exp`=4, `unit_class`=8, `faction_A`=190, `faction_H`=190 WHERE `entry`=56865;
UPDATE `creature_template` SET `minlevel`=85, `maxlevel`=85, `exp`=4, `unit_class`=1, `faction_A`=190, `faction_H`=190 WHERE `entry`=57097;
UPDATE `creature_template` SET `minlevel`=90, `maxlevel`=90, `exp`=4, `unit_class`=1, `faction_A`=16, `faction_H`=16 WHERE `entry` IN (59494,59518);
UPDATE `creature_template` SET `minlevel`=86, `maxlevel`=86, `exp`=4, `unit_class`=1, `faction_A`=16, `faction_H`=16 WHERE `entry` IN (59519,59520);
UPDATE `creature_template` SET `minlevel`=85, `maxlevel`=85, `exp`=4, `unit_class`=1, `faction_A`=16, `faction_H`=16 WHERE `entry`=59521;
UPDATE `creature_template` SET `minlevel`=90, `maxlevel`=90, `exp`=4, `unit_class`=1 WHERE `entry` IN (56821,59394,59464,59551,59799,59822,59824,60276,65392);
UPDATE `creature_template` SET `minlevel`=90, `maxlevel`=90, `exp`=4, `unit_class`=2 WHERE `entry` IN (59074,59075,59704,64361,65375,65376);
UPDATE `creature_template` SET `minlevel`=85, `maxlevel`=85, `exp`=4, `faction_A`=14, `faction_H`=14 WHERE `entry`=56718;
UPDATE `creature_template` SET `minlevel`=90, `maxlevel`=90, `exp`=4, `faction_A`=16, `faction_H`=16 WHERE `entry` IN (59479,59512,59522,65522);
UPDATE `creature_template` SET `minlevel`=90, `maxlevel`=90, `exp`=4 WHERE `entry`=59482;
