-- Temple of the Jade Serpent (map 960, dungeon, MoP): missing creatures, gameobjects, loot and scripts
-- Idempotent: every INSERT is preceded by a DELETE of its keys.

-- creature_template
DELETE FROM `creature_template` WHERE `entry` IN (900001,900002,900003,900004,900005,900006,900007,900008,900009,900010,900011,900012,900013,900014,900015,900016,900017,900018,900019,900020,900021,900022,900023,900024,900025,900026,900027,900028,900029,900030,900031,900032,900033,900034,900035,900036,900037,900038);
INSERT INTO `creature_template` (`entry`,`difficulty_entry_1`,`difficulty_entry_2`,`difficulty_entry_3`,`KillCredit1`,`KillCredit2`,`modelid1`,`modelid2`,`modelid3`,`modelid4`,`name`,`subname`,`IconName`,`gossip_menu_id`,`minlevel`,`maxlevel`,`exp`,`exp_unk`,`faction_A`,`faction_H`,`npcflag`,`speed_walk`,`speed_run`,`scale`,`npc_rank`,`mindmg`,`maxdmg`,`dmgschool`,`attackpower`,`dmg_multiplier`,`baseattacktime`,`rangeattacktime`,`unit_class`,`unit_flags`,`unit_flags2`,`dynamicflags`,`family`,`trainer_type`,`trainer_class`,`trainer_race`,`minrangedmg`,`maxrangedmg`,`rangedattackpower`,`type`,`type_flags`,`type_flags2`,`lootid`,`pickpocketloot`,`skinloot`,`resistance1`,`resistance2`,`resistance3`,`resistance4`,`resistance5`,`resistance6`,`spell1`,`spell2`,`spell3`,`spell4`,`spell5`,`spell6`,`spell7`,`spell8`,`PetSpellDataId`,`VehicleId`,`mingold`,`maxgold`,`AIName`,`MovementType`,`InhabitType`,`HoverHeight`,`Health_mod`,`Mana_mod`,`Mana_mod_extra`,`Armor_mod`,`RacialLeader`,`questItem1`,`questItem2`,`questItem3`,`questItem4`,`questItem5`,`questItem6`,`movementId`,`RegenHealth`,`mechanic_immune_mask`,`flags_extra`,`ScriptName`,`ModLevel`,`detection_range`,`WDBVerified`) VALUES
(900001,0,0,0,0,56843,45980,0,0,0,'Sha of Doubt','','',0,92,92,4,0,16,16,0,10.0,7.0,1.0,1,19140.3,28710.449999999997,0,0,4.0,2000,2000,2,32832,2048,0,0,0,0,0,19140.3,28710.449999999997,0,4,2149580904,0,900001,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,450000,460000,'',0,3,1.0,30.0,7.0,1.0,1.0,0,0,0,0,0,0,0,154,1,0,0,'',0,20.0,17688),
(900002,0,0,0,0,56843,45980,0,0,0,'Sha of Doubt','','',0,93,93,4,0,16,16,0,10.0,7.0,1.0,1,24431.0,36646.5,0,0,8.0,2000,2000,2,32832,2048,0,0,0,0,0,24431.0,36646.5,0,4,2149580904,0,56439,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,450000,460000,'',0,3,1.0,45.0,7.0,1.0,1.0,0,0,0,0,0,0,0,154,1,0,0,'',0,20.0,17688),
(900003,0,0,0,0,0,41125,0,0,0,'Wise Mari','Waterspeaker','',0,92,92,4,0,14,14,0,6.94444,2.5,1.0,1,19140.3,28710.449999999997,0,0,4.0,2000,2000,1,32832,2048,0,0,0,0,0,19140.3,28710.449999999997,0,7,2149580872,0,900003,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,440000,450000,'',0,3,1.0,17.5,1.0,1.0,1.0,0,0,0,0,0,0,0,0,1,803979263,0,'',0,20.0,17688),
(900004,0,0,0,0,0,41125,0,0,0,'Wise Mari','Waterspeaker','',0,93,93,4,0,14,14,0,6.94444,2.5,1.0,1,24431.0,36646.5,0,0,8.0,2000,2000,1,32832,2048,0,0,0,0,0,24431.0,36646.5,0,7,2149580872,0,56448,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,440000,450000,'',0,3,1.0,24.0,1.0,1.0,1.0,0,0,0,0,0,0,0,0,1,803979263,0,'',0,20.0,17688),
(900005,0,0,0,0,0,39442,0,0,0,'Corrupt Droplet','','',0,91,91,4,0,14,14,0,3.0,3.0,1.0,0,14995.0,22492.5,0,0,1.0,2000,2000,1,0,2048,0,0,0,0,0,14995.0,22492.5,0,4,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,'',0,3,1.0,1.0,1.0,1.0,1.0,0,0,0,0,0,0,0,76,1,0,0,'',0,20.0,17688),
(900006,0,0,0,0,0,39442,0,0,0,'Corrupt Droplet','','',0,91,91,4,0,14,14,0,3.0,3.0,1.0,0,14995.0,22492.5,0,0,2.0,2000,2000,1,0,2048,0,0,0,0,0,14995.0,22492.5,0,4,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,'',0,3,1.0,1.5,1.0,1.0,1.0,0,0,0,0,0,0,0,76,1,0,0,'',0,20.0,17688),
(900007,0,0,0,0,0,39487,0,0,0,'Liu Flameheart','Priestess of the Jade Serpent','',0,92,92,4,0,16,16,0,10.0,4.0,1.0,1,19140.3,28710.449999999997,0,0,2.0,2000,2000,1,32832,2048,0,0,0,0,0,19140.3,28710.449999999997,0,7,2149580904,0,900007,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,450000,460000,'',0,3,1.0,25.0,7.0,1.0,1.0,0,0,0,0,0,0,0,150,1,0,0,'',0,20.0,17688),
(900008,0,0,0,0,0,39487,0,0,0,'Liu Flameheart','Priestess of the Jade Serpent','',0,93,93,4,0,16,16,0,10.0,4.0,1.0,1,24431.0,36646.5,0,0,4.0,2000,2000,1,32832,2048,0,0,0,0,0,24431.0,36646.5,0,7,2149580904,0,56732,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,450000,460000,'',0,3,1.0,40.0,7.0,1.0,1.0,0,0,0,0,0,0,0,150,1,0,0,'',0,20.0,17688),
(900009,0,0,0,0,0,42914,0,0,0,'Corrupted Scroll','','',0,90,90,4,0,14,14,0,8.0,2.5,1.0,1,11747.7,17621.550000000003,0,0,3.0,2000,2000,1,33280,2048,0,0,0,0,0,11747.7,17621.550000000003,0,10,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,'',0,3,1.0,1.0,1.0,1.0,1.0,0,0,0,0,0,0,0,0,1,0,0,'',0,20.0,17688),
(900010,0,0,0,0,0,42914,0,0,0,'Corrupted Scroll','','',0,91,91,4,0,14,14,0,8.0,2.5,1.0,1,14995.0,22492.5,0,0,6.0,2000,2000,1,33280,2048,0,0,0,0,0,14995.0,22492.5,0,10,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,'',0,3,1.0,1.0,1.0,1.0,1.0,0,0,0,0,0,0,0,0,1,0,0,'',0,20.0,17688),
(900011,0,0,0,0,0,41677,0,0,0,'Minion of Doubt','','',0,90,90,4,0,16,16,0,8.0,2.5,1.0,1,11747.7,17621.550000000003,0,0,4.0,2000,2000,1,32768,2048,0,0,0,0,0,11747.7,17621.550000000003,0,4,72,0,900011,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,450000,460000,'',0,3,1.0,8.0,1.0,1.0,1.0,0,0,0,0,0,0,0,0,1,0,0,'',0,20.0,17688),
(900012,0,0,0,0,0,41677,0,0,0,'Minion of Doubt','','',0,91,91,4,0,16,16,0,8.0,2.5,1.0,1,14995.0,22492.5,0,0,8.0,2000,2000,1,32768,2048,0,0,0,0,0,14995.0,22492.5,0,4,72,0,900012,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,450000,460000,'',0,3,1.0,12.0,1.0,1.0,1.0,0,0,0,0,0,0,0,0,1,0,0,'',0,20.0,17688),
(900013,0,0,0,0,0,40884,0,0,0,'Lesser Sha','','',0,90,90,4,0,16,16,0,8.0,2.5,1.0,1,11747.7,17621.550000000003,0,0,4.0,2000,2000,1,32768,2048,0,0,0,0,0,11747.7,17621.550000000003,0,4,72,0,900013,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,'',0,3,1.0,1.5,1.0,1.0,1.0,0,0,0,0,0,0,0,0,1,0,0,'',0,20.0,17688),
(900014,0,0,0,0,0,40884,0,0,0,'Lesser Sha','','',0,91,91,4,0,16,16,0,8.0,2.5,1.0,1,14995.0,22492.5,0,0,8.0,2000,2000,1,32768,2048,0,0,0,0,0,14995.0,22492.5,0,4,72,0,900014,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,'',0,3,1.0,3.0,1.0,1.0,1.0,0,0,0,0,0,0,0,0,1,0,0,'',0,20.0,17688),
(900015,0,0,0,0,0,39065,0,0,0,'The Nodding Tiger','','',0,90,90,4,0,16,16,0,8.0,2.5,1.0,1,11747.7,17621.550000000003,0,0,3.0,2000,2000,2,32832,2048,0,0,0,0,0,11747.7,17621.550000000003,0,1,72,0,900015,0,0,0,0,0,0,0,0,31289,0,0,0,0,0,0,0,0,0,10000,11000,'',0,3,1.0,3.0,7.0,1.0,1.0,0,0,0,0,0,0,0,0,1,0,0,'',0,20.0,17688),
(900016,0,0,0,0,0,39065,0,0,0,'The Nodding Tiger','','',0,91,91,4,0,16,16,0,8.0,2.5,1.0,1,14995.0,22492.5,0,0,6.0,2000,2000,2,32832,2048,0,0,0,0,0,14995.0,22492.5,0,1,72,0,900016,0,0,0,0,0,0,0,0,31289,0,0,0,0,0,0,0,0,0,10000,11000,'',0,3,1.0,5.0,7.0,1.0,1.0,0,0,0,0,0,0,0,0,1,0,0,'',0,20.0,17688),
(900017,0,0,0,0,0,31460,0,0,0,'The Golden Beetle','','',0,90,90,4,0,16,16,0,8.0,2.5,1.0,1,11747.7,17621.550000000003,0,0,3.0,2000,2000,2,32832,2048,0,0,0,0,0,11747.7,17621.550000000003,0,1,72,0,900017,0,0,0,0,0,0,0,0,128051,88023,31589,0,0,0,0,0,0,0,11000,12000,'',0,3,1.0,3.0,7.0,1.0,1.0,0,0,0,0,0,0,0,0,1,0,0,'',0,20.0,17688),
(900018,0,0,0,0,0,31460,0,0,0,'The Golden Beetle','','',0,91,91,4,0,16,16,0,8.0,2.5,1.0,1,14995.0,22492.5,0,0,6.0,2000,2000,2,32832,2048,0,0,0,0,0,14995.0,22492.5,0,1,72,0,900018,0,0,0,0,0,0,0,0,128051,88023,31589,0,0,0,0,0,0,0,11000,12000,'',0,3,1.0,5.0,7.0,1.0,1.0,0,0,0,0,0,0,0,0,1,0,0,'',0,20.0,17688),
(900019,0,0,0,0,0,40855,0,0,0,'The Talking Fish','','',0,90,90,4,0,16,16,0,8.0,2.5,1.0,1,11747.7,17621.550000000003,0,0,3.0,2000,2000,2,32832,2048,0,0,0,0,0,11747.7,17621.550000000003,0,1,72,0,900019,0,0,0,0,0,0,0,0,114811,0,0,0,0,0,0,0,0,0,9500,9600,'',0,3,1.0,8.0,6.0,1.0,1.0,0,0,0,0,0,0,0,0,1,0,0,'',0,20.0,17688),
(900020,0,0,0,0,0,40855,0,0,0,'The Talking Fish','','',0,91,91,4,0,16,16,0,8.0,2.5,1.0,1,14995.0,22492.5,0,0,6.0,2000,2000,2,32832,2048,0,0,0,0,0,14995.0,22492.5,0,1,72,0,900020,0,0,0,0,0,0,0,0,114811,0,0,0,0,0,0,0,0,0,9500,9600,'',0,3,1.0,12.0,6.0,1.0,1.0,0,0,0,0,0,0,0,0,1,0,0,'',0,20.0,17688),
(900021,0,0,0,0,0,42431,0,0,0,'Jiang','The Two Jugglers','',0,90,90,4,0,16,16,0,8.0,2.5,1.0,1,11747.7,17621.550000000003,0,0,3.0,2000,2000,2,32768,2048,0,0,0,0,0,11747.7,17621.550000000003,0,7,72,0,900021,59547,0,0,0,0,0,0,0,114805,0,114803,0,0,0,0,0,0,0,9400,9500,'',0,3,1.0,3.0,7.0,1.0,1.0,0,0,0,0,0,0,0,0,1,0,0,'',0,20.0,17688),
(900022,0,0,0,0,0,42431,0,0,0,'Jiang','The Two Jugglers','',0,91,91,4,0,16,16,0,8.0,2.5,1.0,1,14995.0,22492.5,0,0,6.0,2000,2000,2,32768,2048,0,0,0,0,0,14995.0,22492.5,0,7,72,0,900022,59547,0,0,0,0,0,0,0,114805,0,114803,0,0,0,0,0,0,0,9400,9500,'',0,3,1.0,5.0,7.0,1.0,1.0,0,0,0,0,0,0,0,0,1,0,0,'',0,20.0,17688),
(900023,0,0,0,0,0,39483,0,0,0,'The Crybaby Hozen','','',0,90,90,4,0,16,16,0,6.94444,2.22222,1.0,1,11747.7,17621.550000000003,0,0,3.0,2000,2000,2,32832,2048,0,0,0,0,0,11747.7,17621.550000000003,0,1,72,0,900023,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,9300,9400,'',0,3,1.0,3.0,7.0,1.0,1.0,0,0,0,0,0,0,0,0,1,0,0,'',0,20.0,17688),
(900024,0,0,0,0,0,39483,0,0,0,'The Crybaby Hozen','','',0,91,91,4,0,16,16,0,6.94444,2.22222,1.0,1,14995.0,22492.5,0,0,6.0,2000,2000,2,32832,2048,0,0,0,0,0,14995.0,22492.5,0,1,72,0,900024,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,9300,9400,'',0,3,1.0,5.0,7.0,1.0,1.0,0,0,0,0,0,0,0,0,1,0,0,'',0,20.0,17688),
(900025,0,0,0,0,0,40856,0,0,0,'The Songbird Queen','','',0,90,90,4,0,16,16,0,8.0,1.66667,1.0,1,11747.7,17621.550000000003,0,0,3.0,2000,2000,2,32832,2048,0,0,0,0,0,11747.7,17621.550000000003,0,1,72,0,900025,0,0,0,0,0,0,0,0,0,114826,0,0,0,0,0,0,0,0,9700,9800,'',0,3,1.0,3.0,7.0,1.0,1.0,0,0,0,0,0,0,0,0,1,0,0,'',0,20.0,17688),
(900026,0,0,0,0,0,40856,0,0,0,'The Songbird Queen','','',0,91,91,4,0,16,16,0,8.0,1.66667,1.0,1,14995.0,22492.5,0,0,6.0,2000,2000,2,32832,2048,0,0,0,0,0,14995.0,22492.5,0,1,72,0,900026,0,0,0,0,0,0,0,0,0,114826,0,0,0,0,0,0,0,0,9700,9800,'',0,3,1.0,5.0,7.0,1.0,1.0,0,0,0,0,0,0,0,0,1,0,0,'',0,20.0,17688),
(900027,0,0,0,0,0,40907,0,0,0,'Haunting Sha','','',0,90,90,4,0,16,16,0,8.0,2.5,1.0,1,11747.7,17621.550000000003,0,0,3.0,2000,2000,1,32768,2048,0,0,0,0,0,11747.7,17621.550000000003,0,4,72,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,'',0,3,1.0,5.0,1.0,1.0,1.0,0,0,0,0,0,0,0,0,1,0,0,'',0,20.0,17688),
(900028,0,0,0,0,0,40907,0,0,0,'Haunting Sha','','',0,91,91,4,0,16,16,0,8.0,2.5,1.0,1,14995.0,22492.5,0,0,6.0,2000,2000,1,32768,2048,0,0,0,0,0,14995.0,22492.5,0,4,72,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,'',0,3,1.0,7.0,1.0,1.0,1.0,0,0,0,0,0,0,0,0,1,0,0,'',0,20.0,17688),
(900029,0,0,0,0,0,40884,0,0,0,'Lesser Sha','','',0,90,90,4,0,16,16,0,8.0,2.5,1.0,1,11747.7,17621.550000000003,0,0,3.0,2000,2000,1,32768,2048,0,0,0,0,0,11747.7,17621.550000000003,0,4,72,0,0,0,0,0,0,0,0,0,0,0,114599,0,0,0,0,0,0,0,0,0,0,'',0,3,1.0,1.5,1.0,1.0,1.0,0,0,0,0,0,0,0,0,1,0,0,'',0,20.0,17688),
(900030,0,0,0,0,0,40884,0,0,0,'Lesser Sha','','',0,91,91,4,0,16,16,0,8.0,2.5,1.0,1,14995.0,22492.5,0,0,6.0,2000,2000,1,32768,2048,0,0,0,0,0,14995.0,22492.5,0,4,72,0,0,0,0,0,0,0,0,0,0,0,114599,0,0,0,0,0,0,0,0,0,0,'',0,3,1.0,3.0,1.0,1.0,1.0,0,0,0,0,0,0,0,0,1,0,0,'',0,20.0,17688),
(900031,0,0,0,0,0,43999,0,0,0,'Corrupt Living Water','','',0,90,90,4,0,14,14,0,9.02778,2.5,1.0,1,11747.7,17621.550000000003,0,0,3.0,2000,2000,1,64,2048,0,0,0,0,0,11747.7,17621.550000000003,0,4,0,0,0,0,0,0,0,0,0,0,0,0,106653,0,0,0,0,0,0,0,0,0,0,'',0,3,1.0,5.0,1.0,1.0,1.0,0,0,0,0,0,0,0,0,1,0,0,'',0,20.0,17688),
(900032,0,0,0,0,0,43999,0,0,0,'Corrupt Living Water','','',0,91,91,4,0,14,14,0,9.02778,2.5,1.0,1,14995.0,22492.5,0,0,6.0,2000,2000,1,64,2048,0,0,0,0,0,14995.0,22492.5,0,4,0,0,0,0,0,0,0,0,0,0,0,0,106653,0,0,0,0,0,0,0,0,0,0,'',0,3,1.0,7.0,1.0,1.0,1.0,0,0,0,0,0,0,0,0,1,0,0,'',0,20.0,17688),
(900033,0,0,0,0,0,45552,45553,0,0,'Serpent Warrior','','',0,90,90,4,0,1665,1665,0,8.0,2.5,1.0,0,11747.7,17621.550000000003,0,0,4.0,2000,2000,1,32832,2048,0,0,0,0,0,11747.7,17621.550000000003,0,7,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,'',0,3,1.0,3.0,1.0,1.0,1.0,0,0,0,0,0,0,0,0,1,0,0,'',0,20.0,17688),
(900034,0,0,0,0,0,45552,45553,0,0,'Serpent Warrior','','',0,91,91,4,0,1665,1665,0,8.0,2.5,1.0,0,14995.0,22492.5,0,0,8.0,2000,2000,1,32832,2048,0,0,0,0,0,14995.0,22492.5,0,7,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,'',0,3,1.0,5.0,1.0,1.0,1.0,0,0,0,0,0,0,0,0,1,0,0,'',0,20.0,17688),
(900035,0,0,0,0,0,39442,0,0,0,'Corrupt Droplet','','',0,90,90,4,0,16,16,0,3.0,3.0,1.0,0,11747.7,17621.550000000003,0,0,1.4,2000,2000,1,0,2048,0,0,0,0,0,11747.7,17621.550000000003,0,4,0,0,900035,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,'',0,3,1.0,1.0,1.0,1.0,1.0,0,0,0,0,0,0,0,76,1,0,0,'',0,20.0,17688),
(900036,0,0,0,0,0,39442,0,0,0,'Corrupt Droplet','','',0,91,91,4,0,16,16,0,3.0,3.0,1.0,0,14995.0,22492.5,0,0,2.8,2000,2000,1,0,2048,0,0,0,0,0,14995.0,22492.5,0,4,0,0,900036,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,'',0,3,1.0,1.5,1.0,1.0,1.0,0,0,0,0,0,0,0,76,1,0,0,'',0,20.0,17688),
(900037,0,0,0,0,0,42431,0,0,0,'Xiang','The Two Jugglers','',0,90,90,4,0,16,16,0,8.0,2.5,1.0,1,11747.7,17621.550000000003,0,0,3.0,2000,2000,2,32768,2048,0,0,0,0,0,11747.7,17621.550000000003,0,7,72,0,900037,65317,0,0,0,0,0,0,0,114805,0,114803,0,0,0,0,0,0,0,9400,9500,'',0,3,1.0,3.0,7.0,1.0,1.0,0,0,0,0,0,0,0,0,1,0,0,'',0,20.0,17688),
(900038,0,0,0,0,0,42431,0,0,0,'Xiang','The Two Jugglers','',0,91,91,4,0,16,16,0,8.0,2.5,1.0,1,14995.0,22492.5,0,0,6.0,2000,2000,2,32768,2048,0,0,0,0,0,14995.0,22492.5,0,7,72,0,900038,65317,0,0,0,0,0,0,0,114805,0,114803,0,0,0,0,0,0,0,9400,9500,'',0,3,1.0,5.0,7.0,1.0,1.0,0,0,0,0,0,0,0,0,1,0,0,'',0,20.0,17688);

-- creature_template updates (difficulty slots, loot ids, SmartAI)
UPDATE `creature_template` SET `difficulty_entry_1`=900001, `difficulty_entry_2`=900002 WHERE `entry`=56439;
UPDATE `creature_template` SET `difficulty_entry_1`=900003, `difficulty_entry_2`=900004 WHERE `entry`=56448;
UPDATE `creature_template` SET `difficulty_entry_1`=900005, `difficulty_entry_2`=900006 WHERE `entry`=56658;
UPDATE `creature_template` SET `difficulty_entry_1`=900007, `difficulty_entry_2`=900008 WHERE `entry`=56732;
UPDATE `creature_template` SET `difficulty_entry_1`=900009, `difficulty_entry_2`=900010 WHERE `entry`=57080;
UPDATE `creature_template` SET `difficulty_entry_1`=900011, `difficulty_entry_2`=900012 WHERE `entry`=57109;
UPDATE `creature_template` SET `difficulty_entry_1`=900013, `difficulty_entry_2`=900014 WHERE `entry`=58319;
UPDATE `creature_template` SET `difficulty_entry_1`=900015, `difficulty_entry_2`=900016 WHERE `entry`=59544;
UPDATE `creature_template` SET `difficulty_entry_1`=900017, `difficulty_entry_2`=900018 WHERE `entry`=59545;
UPDATE `creature_template` SET `difficulty_entry_1`=900019, `difficulty_entry_2`=900020 WHERE `entry`=59546;
UPDATE `creature_template` SET `difficulty_entry_1`=900021, `difficulty_entry_2`=900022, `AIName`='SmartAI' WHERE `entry`=59547 AND `ScriptName`='';
UPDATE `creature_template` SET `difficulty_entry_1`=900023, `difficulty_entry_2`=900024 WHERE `entry`=59552;
UPDATE `creature_template` SET `difficulty_entry_1`=900025, `difficulty_entry_2`=900026 WHERE `entry`=59553;
UPDATE `creature_template` SET `difficulty_entry_1`=900027, `difficulty_entry_2`=900028, `AIName`='SmartAI' WHERE `entry`=59555 AND `ScriptName`='';
UPDATE `creature_template` SET `difficulty_entry_1`=900029, `difficulty_entry_2`=900030 WHERE `entry`=59598;
UPDATE `creature_template` SET `difficulty_entry_1`=900031, `difficulty_entry_2`=900032 WHERE `entry`=59873;
UPDATE `creature_template` SET `difficulty_entry_1`=900033, `difficulty_entry_2`=900034 WHERE `entry`=62171;
UPDATE `creature_template` SET `difficulty_entry_1`=900035, `difficulty_entry_2`=900036 WHERE `entry`=62358;
UPDATE `creature_template` SET `difficulty_entry_1`=900037, `difficulty_entry_2`=900038 WHERE `entry`=65317;

-- gameobject_template
DELETE FROM `gameobject_template` WHERE `entry` IN (211674,211991);
INSERT INTO `gameobject_template` (`entry`,`type`,`displayId`,`name`,`IconName`,`castBarCaption`,`unk1`,`faction`,`flags`,`size`,`questItem1`,`questItem2`,`questItem3`,`questItem4`,`questItem5`,`questItem6`,`data0`,`data1`,`data2`,`data3`,`data4`,`data5`,`data6`,`data7`,`data8`,`data9`,`data10`,`data11`,`data12`,`data13`,`data14`,`data15`,`data16`,`data17`,`data18`,`data19`,`data20`,`data21`,`data22`,`data23`,`data24`,`data25`,`data26`,`data27`,`data28`,`data29`,`data30`,`data31`,`unkInt32`,`AIName`,`ScriptName`,`WDBVerified`) VALUES
(211674,10,11554,'Challenger\'s Orb','','','',0,262144,1.5,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,13872,1,0,0,0,0,0,1,0,0,0,0,0,0,'','',19342),
(211991,0,11638,'Challenge Mode - Start Door (Line 3.0)','','','',1375,0,3.0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,'','',18414);

-- creature_loot_template (23 loot ids)
DELETE FROM `creature_loot_template` WHERE `entry` IN (900001,900003,900007,900011,900012,900013,900014,900015,900016,900017,900018,900019,900020,900021,900022,900023,900024,900025,900026,900035,900036,900037,900038);
INSERT INTO `creature_loot_template` (`entry`,`item`,`ChanceOrQuestChance`,`lootmode`,`groupid`,`mincountOrRef`,`maxcount`) VALUES
(900001,81130,100.0,1,0,-50018,2),
(900001,87544,1.5,1,0,1,1),
(900003,81072,0.0,1,1,1,1),
(900003,81075,0.0,1,1,1,1),
(900003,81083,0.0,1,1,1,1),
(900003,81123,0.0,1,1,1,1),
(900003,81124,0.0,1,1,1,1),
(900007,81067,0.0,1,1,1,1),
(900007,81070,0.0,1,1,1,1),
(900007,81084,0.0,1,1,1,1),
(900007,81127,0.0,1,1,1,1),
(900007,81128,0.0,1,1,1,1),
(900011,81200,200.727,1,0,2,4),
(900011,89112,11.398,1,0,1,1),
(900011,104212,0.05,1,0,1,1),
(900011,104221,0.01,1,0,1,1),
(900012,81200,200.727,1,0,2,4),
(900012,89112,11.398,1,0,1,1),
(900012,104212,0.05,1,0,1,1),
(900012,104221,0.01,1,0,1,1),
(900013,81200,209.78,1,0,2,4),
(900013,89112,12.6973,1,0,1,1),
(900014,81200,209.78,1,0,2,4),
(900014,89112,12.6973,1,0,1,1),
(900015,83156,203.317,1,0,2,4),
(900015,89112,10.707,1,0,1,1),
(900015,104221,0.01,1,0,1,1),
(900016,83156,203.317,1,0,2,4),
(900016,89112,10.707,1,0,1,1),
(900016,104221,0.01,1,0,1,1),
(900017,82328,202.46,1,0,1,1),
(900017,89112,9.80966,1,0,1,1),
(900018,82328,202.46,1,0,1,1),
(900018,89112,9.80966,1,0,1,1),
(900019,81194,200.226,1,0,2,4),
(900019,89112,10.7498,1,0,1,1),
(900020,81194,200.226,1,0,2,4),
(900020,89112,10.7498,1,0,1,1),
(900021,72988,140.936,1,0,1,8),
(900021,81400,28.1287,1,0,1,1),
(900021,81406,14.5614,1,0,1,1),
(900021,81408,9.12281,1,0,1,1),
(900021,89112,22.2807,1,0,1,1),
(900021,95470,0.3,1,0,1,1),
(900021,95471,0.3,1,0,1,1),
(900022,72988,140.936,1,0,1,8),
(900022,81400,28.1287,1,0,1,1),
(900022,81406,14.5614,1,0,1,1),
(900022,81408,9.12281,1,0,1,1),
(900022,89112,22.2807,1,0,1,1),
(900022,95470,0.3,1,0,1,1),
(900022,95471,0.3,1,0,1,1),
(900023,72988,134.816,1,0,1,6),
(900023,81405,30.1741,1,0,1,1),
(900023,89112,22.147,1,0,1,1),
(900023,95470,0.3,1,0,1,1),
(900023,95471,0.3,1,0,1,1),
(900024,72988,134.816,1,0,1,6),
(900024,81405,30.1741,1,0,1,1),
(900024,89112,22.147,1,0,1,1),
(900024,95470,0.3,1,0,1,1),
(900024,95471,0.3,1,0,1,1),
(900025,74839,134.261,1,0,1,1),
(900025,81203,198.216,1,0,2,4),
(900025,89112,11.2539,1,0,1,1),
(900026,74839,134.261,1,0,1,1),
(900026,81203,198.216,1,0,2,4),
(900026,89112,11.2539,1,0,1,1),
(900035,81202,226.187,1,0,1,1),
(900035,81219,6.35528,1,0,1,1),
(900035,89112,12.0597,1,0,1,1),
(900035,104221,0.01,1,0,1,1),
(900036,81202,226.187,1,0,1,1),
(900036,81219,6.35528,1,0,1,1),
(900036,89112,12.0597,1,0,1,1),
(900036,104221,0.01,1,0,1,1),
(900037,72988,144.83,1,0,1,9),
(900037,81400,30.1255,1,0,1,1),
(900037,81406,14.8834,1,0,1,1),
(900037,89112,23.8494,1,0,1,1),
(900037,95470,0.3,1,0,1,1),
(900037,95471,0.3,1,0,1,1),
(900038,72988,144.83,1,0,1,9),
(900038,81400,30.1255,1,0,1,1),
(900038,81406,14.8834,1,0,1,1),
(900038,89112,23.8494,1,0,1,1),
(900038,95470,0.3,1,0,1,1),
(900038,95471,0.3,1,0,1,1);

-- reference_loot_template (1 loot ids)
DELETE FROM `reference_loot_template` WHERE `entry` IN (50018);
INSERT INTO `reference_loot_template` (`entry`,`item`,`ChanceOrQuestChance`,`lootmode`,`groupid`,`mincountOrRef`,`maxcount`) VALUES
(50018,81063,10.0,1,1,1,1),
(50018,81069,10.0,1,1,1,1),
(50018,81071,10.0,1,1,1,1),
(50018,81074,10.0,1,1,1,1),
(50018,81079,10.0,1,1,1,1),
(50018,81082,10.0,1,1,1,1),
(50018,81129,10.0,1,1,1,1),
(50018,81130,10.0,1,1,1,1),
(50018,81131,10.0,1,1,1,1),
(50018,81132,10.0,1,1,1,1);

-- creature spawns
DELETE FROM `creature` WHERE `guid` IN (8202637,8202638,8202639,8202640,8202641,8202642,8202643,8202644,8202645,8202646,8202647,8202648,8202649,8202650,8202651,8202652,8202653,8202654);
INSERT INTO `creature` (`guid`,`id`,`map`,`spawnMask`,`phaseId`,`phaseGroup`,`modelid`,`equipment_id`,`position_x`,`position_y`,`position_z`,`orientation`,`spawntimesecs`,`spawndist`,`currentwaypoint`,`curhealth`,`curmana`,`MovementType`,`npcflag`,`unit_flags`,`dynamicflags`) VALUES
(8202637,66771,960,262,0,0,0,0,931.169,-2523.76,181.322,2.43606,7200,6.0,0,17672,0,1,0,0,0),
(8202638,66771,960,262,0,0,0,0,974.816,-2496.0,181.306,5.23775,7200,6.0,0,17672,0,1,0,0,0),
(8202639,66771,960,262,0,0,0,0,970.833,-2541.65,181.301,2.66688,7200,6.0,0,17672,0,1,0,0,0),
(8202640,66771,960,262,0,0,0,0,937.667,-2593.63,181.339,1.33716,7200,6.0,0,17672,0,1,0,0,0),
(8202641,66771,960,262,0,0,0,0,903.818,-2537.13,181.321,5.17165,7200,6.0,0,17672,0,1,0,0,0),
(8202642,66771,960,262,0,0,0,0,862.908,-2703.68,192.152,1.97944,7200,6.0,0,17672,0,1,0,0,0),
(8202643,66771,960,262,0,0,0,0,939.07,-2483.59,180.981,5.01195,7200,6.0,0,17672,0,1,0,0,0),
(8202644,66771,960,262,0,0,0,0,966.716,-2492.76,181.232,1.84985,7200,6.0,0,17672,0,1,0,0,0),
(8202645,66771,960,262,0,0,0,0,929.363,-2522.61,181.322,0.504202,7200,6.0,0,17672,0,1,0,0,0),
(8202646,66771,960,262,0,0,0,0,859.74,-2726.41,192.152,5.65249,7200,6.0,0,17672,0,1,0,0,0),
(8202647,66771,960,262,0,0,0,0,899.232,-2702.06,192.151,3.72063,7200,6.0,0,17672,0,1,0,0,0),
(8202648,66771,960,262,0,0,0,0,863.275,-2729.21,192.152,2.58158,7200,6.0,0,17672,0,1,0,0,0),
(8202649,66771,960,262,0,0,0,0,872.987,-2685.9,192.145,1.13056,7200,6.0,0,17672,0,1,0,0,0),
(8202650,66771,960,262,0,0,0,0,955.021,-2589.68,181.293,3.75715,7200,6.0,0,17672,0,1,0,0,0),
(8202651,66771,960,262,0,0,0,0,899.334,-2724.02,192.152,4.79444,7200,6.0,0,17672,0,1,0,0,0),
(8202652,66771,960,262,0,0,0,0,898.856,-2725.23,192.151,0.287563,7200,6.0,0,17672,0,1,0,0,0),
(8202653,66771,960,262,0,0,0,0,920.187,-2505.25,181.323,1.16241,7200,6.0,0,17672,0,1,0,0,0),
(8202654,66771,960,262,0,0,0,0,971.436,-2518.64,180.51,2.15092,7200,6.0,0,17672,0,1,0,0,0);

-- gameobject spawns
DELETE FROM `gameobject` WHERE `guid` IN (901366,901367,901368,901369);
INSERT INTO `gameobject` (`guid`,`id`,`map`,`spawnMask`,`phaseId`,`phaseGroup`,`position_x`,`position_y`,`position_z`,`orientation`,`rotation0`,`rotation1`,`rotation2`,`rotation3`,`spawntimesecs`,`animprogress`,`state`) VALUES
(901366,211674,960,256,0,0,948.953,-2501.3,180.608,4.39458,0.0,0.0,0.810088,-0.586308,7200,255,1),
(901367,211991,960,256,0,0,967.469,-2507.24,180.504,2.827,0.0,0.0,0.987654,0.156653,7200,255,1),
(901368,211991,960,256,0,0,930.348,-2495.32,180.503,5.96743,0.0,0.0,0.987654,0.156653,7200,255,1),
(901369,221413,960,262,0,0,906.128,-2695.27,193.813,1.90005,0.0,0.0,0.813431,0.581662,7200,255,1);

-- smart_scripts
DELETE FROM `smart_scripts` WHERE `entryorguid`=59547 AND `source_type`=0;
DELETE FROM `smart_scripts` WHERE `entryorguid`=59555 AND `source_type`=0;
DELETE FROM `smart_scripts` WHERE `entryorguid`=221413 AND `source_type`=1;
INSERT INTO `smart_scripts` (`entryorguid`,`source_type`,`id`,`link`,`event_type`,`event_phase_mask`,`event_chance`,`event_flags`,`event_param1`,`event_param2`,`event_param3`,`event_param4`,`event_param5`,`action_type`,`action_param1`,`action_param2`,`action_param3`,`action_param4`,`action_param5`,`action_param6`,`target_type`,`target_param1`,`target_param2`,`target_param3`,`target_x`,`target_y`,`target_z`,`target_o`,`comment`) VALUES
(59547,0,0,0,4,0,100,0,0,0,0,0,0,11,114805,0,0,0,0,0,2,0,0,0,0.0,0.0,0.0,0.0,'Jiang - Coup de pied du trapziste'),
(59547,0,1,0,0,0,100,0,6000,6000,15000,15000,0,11,114803,0,0,0,0,0,5,0,0,0,0.0,0.0,0.0,0.0,'Jiang - Lancer une torche'),
(59547,0,2,0,1,0,100,1,0,0,0,0,0,11,117691,0,0,0,0,0,1,0,0,0,0.0,0.0,0.0,0.0,'Jiang - Masque Sha'),
(59555,0,0,0,1,0,100,1,0,0,0,0,0,11,75333,0,0,0,0,0,1,0,0,0,0.0,0.0,0.0,0.0,'Sha hanteur - vitation'),
(221413,1,0,1,62,0,100,0,15798,0,0,0,0,12,72726,8,0,0,0,0,8,0,0,0,878.609,-2717.94,192.249,1.25193,''),
(221413,1,1,0,61,0,100,0,0,0,0,0,0,72,0,0,0,0,0,0,7,0,0,0,0.0,0.0,0.0,0.0,'');
UPDATE `gameobject_template` SET `AIName`='SmartGameObjectAI' WHERE `entry`=221413 AND `ScriptName`='';

-- creature_text
DELETE FROM `creature_text` WHERE `entry` IN (56439,56448,56732,56843,59546,63420);
INSERT INTO `creature_text` (`entry`,`groupid`,`id`,`text`,`type`,`language`,`probability`,`emote`,`duration`,`sound`,`comment`) VALUES
(56439,0,0,'Die or surrender. You cannot defeat me.',14,0,100.0,0,0,28273,'Sha of Doubt'),
(56439,1,0,'You cannot escape me. I am... in... every...  breath...',14,0,100.0,0,0,28274,'Sha of Doubt'),
(56439,2,0,'See how effortlessly you become unraveled.',14,0,100.0,0,0,28275,'Sha of Doubt'),
(56439,2,1,'Succumb to the darkness inside your soul.',14,0,100.0,0,0,28276,'Sha of Doubt'),
(56439,3,0,'All is lost.',14,0,100.0,0,0,28277,'Sha of Doubt'),
(56439,4,0,'Abandon all hope.',14,0,100.0,0,0,28278,'Sha of Doubt'),
(56439,4,1,'Embrace your despair.',14,0,100.0,0,0,28279,'Sha of Doubt'),
(56448,0,0,'The water reacts violently to your touch!',41,0,100.0,0,0,0,'Wise Mari'),
(56448,1,0,'You dare to disturb these waters? You will drown!',12,0,100.0,0,0,28257,'Wise Mari'),
(56448,2,0,'Rise!',12,0,100.0,0,0,28260,'Wise Mari'),
(56448,2,1,'Spring to life!',12,0,100.0,0,0,28258,'Wise Mari'),
(56448,2,2,'Living water, bring them death!',12,0,100.0,0,0,28261,'Wise Mari'),
(56448,2,3,'Do not run, it only delays the inevitable.',12,0,100.0,0,0,28269,'Wise Mari'),
(56448,2,4,'Come in, the water\'s fine!',12,0,100.0,0,0,28270,'Wise Mari'),
(56448,2,5,'Let death wash over you!',12,0,100.0,0,0,28271,'Wise Mari'),
(56448,3,0,'Behold the power of torrential waters!',12,0,100.0,0,0,28266,'Wise Mari'),
(56448,4,0,'%s draws from the surrounding water!',41,0,100.0,0,0,0,'Wise Mari'),
(56448,5,0,'The darkness clears...',12,0,100.0,0,0,28262,'Wise Mari'),
(56448,6,0,'I peer into these waters and my reflection is empty...',12,0,100.0,0,0,28263,'Wise Mari'),
(56448,7,0,'Waters... claim me...',12,0,100.0,0,0,28264,'Wise Mari'),
(56448,8,0,'Your cause is hopeless.',12,0,100.0,0,0,28268,'Wise Mari'),
(56448,8,1,'As the waters foretold.',12,0,100.0,0,0,28267,'Wise Mari'),
(56732,0,0,'The heart of the Great Serpent will not fall into your hands!',14,0,100.0,0,0,27603,'Liu Flameheart'),
(56732,1,0,'The haze has been lifted from my eyes... forgive me for doubting you...',14,0,100.0,0,0,27604,'Liu Flameheart'),
(56732,2,0,'Jade Serpent, grant me strength!',14,0,100.0,0,0,27605,'Liu Flameheart'),
(56732,3,0,'The cycle must continue, the Jade Serpent must be reborn!',14,0,100.0,0,0,27606,'Liu Flameheart'),
(56732,4,0,'Leave this place.',14,0,100.0,0,0,27609,'Liu Flameheart to ðÆð¥ð╣Ðüð║ð¥ ð╝ðÁÐÇÐéð▓ÐïÐà'),
(56732,4,1,'The heart is MINE!',14,0,100.0,0,0,27608,'Liu Flameheart'),
(56732,5,0,'Your forces are weak!',14,0,100.0,0,0,27607,'Liu Flameheart'),
(56843,0,0,'You there! In the rafters! My sacred library has become possessed by the Sha!',14,0,100.0,0,0,30887,'Lorewalker Stonestep'),
(56843,1,0,'See how they pour from every letter of these sacred scrolls... corrupting them! Defiling them!',14,0,100.0,0,0,30888,'Lorewalker Stonestep'),
(56843,2,0,'I beseech you, kind strangers. Cleanse this place of their foul presence! Lay our stories to rest!',14,0,100.0,0,0,30889,'Lorewalker Stonestep'),
(56843,3,0,'You must destroy the source of the corruption! Quickly, before our cherished history is erased forever!',14,0,100.0,0,0,30890,'Lorewalker Stonestep'),
(56843,4,0,'Oh, my. If I am not mistaken, it appears that the tale of Zao Sunseeker has come to life before us.',14,0,100.0,0,0,30901,'Lorewalker Stonestep'),
(56843,5,0,'According to the legend, long ago, there were five suns that burned brightly in the sky. When the winds blew, their intense heat caused great wildfires.',14,0,100.0,0,0,30902,'Lorewalker Stonestep'),
(56843,6,0,'Crops wilted, villages burned, and families wandered endlessly to seek refuge from the flames...',14,0,100.0,0,0,30903,'Lorewalker Stonestep'),
(56843,7,0,'The fires, they are burning the library! Please, heroes! Purge this evil before the temple crumbles to ash!',14,0,100.0,0,0,30904,'Lorewalker Stonestep'),
(56843,8,0,'Zao has become corrupted too! Please, fight him with dignity...',14,0,100.0,0,0,30906,'Lorewalker Stonestep'),
(56843,9,0,'Ah, it is not yet over. From what I see, we face the trial of the yaungol. Let me shed some light...',14,0,100.0,0,0,30896,'Lorewalker Stonestep'),
(56843,10,0,'As the tale goes, the yaungol was traveling across the Kun\'lai plains when suddenly he was ambushed by two strange creatures!',14,0,100.0,0,0,30897,'Lorewalker Stonestep'),
(56843,11,0,'The yaungol attacked the beasts with great determination and ferocity. Yet, the more he attacked, the more fearsome the beasts grew!',14,0,100.0,0,0,30898,'Lorewalker Stonestep'),
(56843,12,0,'However, when he let it alone, the beast shrank to near nothingness.',14,0,100.0,0,0,30899,'Lorewalker Stonestep'),
(56843,13,0,'For you see, strife feeds on conflict. When you have peace in your heart, strife will find somewhere else to roam.',14,0,100.0,0,0,30900,'Lorewalker Stonestep'),
(59546,0,0,'When man asks favor of a fish...',12,0,100.0,0,0,0,''),
(59546,1,0,'\'Tis hard to decipher his true wish...',12,0,100.0,0,0,0,''),
(59546,2,0,'He often seeks a lordly dish...',12,0,100.0,0,0,0,''),
(59546,3,0,'To serve upon his table.',12,0,100.0,0,0,0,''),
(63420,0,0,'The cavern trembles violently!',41,0,100.0,0,0,0,''),
(63420,1,0,'An ancient beast stirs within the mists!',41,0,100.0,0,0,0,''),
(63420,2,0,'|cFFF00000Megaera|r rises from the mists!',41,0,100.0,0,0,0,''),
(63420,3,0,'|TInterface\\Icons\\inv_misc_volatilefire:20|tYou are afflicted with |cFFF00000|Hspell:139822|h[Cinders]|h|r!',41,0,100.0,0,0,0,''),
(63420,4,0,'|TInterface\\Icons\\spell_frost_piercing chill:20|tYou are being targeted by |cFFF00000|Hspell:139866|h[Torrent of Ice]|h|r!',41,0,100.0,0,0,0,''),
(63420,5,0,'|TInterface\\Icons\\trade_archaeology_whitehydrafigurine:20|tMegaera begins to |cFFF00000|Hspell:139458|h[Rampage]|h|r!',41,0,100.0,0,0,0,''),
(63420,6,0,'Megaera\'s rage subsides.',41,0,100.0,0,0,0,'');

-- conditions
DELETE FROM `conditions` WHERE `SourceTypeOrReferenceId`=1 AND `SourceGroup`=900021 AND `SourceEntry`=95470 AND `SourceId`=0 AND `ElseGroup`=0 AND `ConditionTypeOrReference`=7 AND `ConditionTarget`=0 AND `ConditionValue1`=755 AND `ConditionValue2`=500 AND `ConditionValue3`=0;
DELETE FROM `conditions` WHERE `SourceTypeOrReferenceId`=1 AND `SourceGroup`=900022 AND `SourceEntry`=95470 AND `SourceId`=0 AND `ElseGroup`=0 AND `ConditionTypeOrReference`=7 AND `ConditionTarget`=0 AND `ConditionValue1`=755 AND `ConditionValue2`=500 AND `ConditionValue3`=0;
DELETE FROM `conditions` WHERE `SourceTypeOrReferenceId`=1 AND `SourceGroup`=900021 AND `SourceEntry`=95471 AND `SourceId`=0 AND `ElseGroup`=0 AND `ConditionTypeOrReference`=7 AND `ConditionTarget`=0 AND `ConditionValue1`=755 AND `ConditionValue2`=500 AND `ConditionValue3`=0;
DELETE FROM `conditions` WHERE `SourceTypeOrReferenceId`=1 AND `SourceGroup`=900022 AND `SourceEntry`=95471 AND `SourceId`=0 AND `ElseGroup`=0 AND `ConditionTypeOrReference`=7 AND `ConditionTarget`=0 AND `ConditionValue1`=755 AND `ConditionValue2`=500 AND `ConditionValue3`=0;
DELETE FROM `conditions` WHERE `SourceTypeOrReferenceId`=1 AND `SourceGroup`=900023 AND `SourceEntry`=95470 AND `SourceId`=0 AND `ElseGroup`=0 AND `ConditionTypeOrReference`=7 AND `ConditionTarget`=0 AND `ConditionValue1`=755 AND `ConditionValue2`=500 AND `ConditionValue3`=0;
DELETE FROM `conditions` WHERE `SourceTypeOrReferenceId`=1 AND `SourceGroup`=900024 AND `SourceEntry`=95470 AND `SourceId`=0 AND `ElseGroup`=0 AND `ConditionTypeOrReference`=7 AND `ConditionTarget`=0 AND `ConditionValue1`=755 AND `ConditionValue2`=500 AND `ConditionValue3`=0;
DELETE FROM `conditions` WHERE `SourceTypeOrReferenceId`=1 AND `SourceGroup`=900023 AND `SourceEntry`=95471 AND `SourceId`=0 AND `ElseGroup`=0 AND `ConditionTypeOrReference`=7 AND `ConditionTarget`=0 AND `ConditionValue1`=755 AND `ConditionValue2`=500 AND `ConditionValue3`=0;
DELETE FROM `conditions` WHERE `SourceTypeOrReferenceId`=1 AND `SourceGroup`=900024 AND `SourceEntry`=95471 AND `SourceId`=0 AND `ElseGroup`=0 AND `ConditionTypeOrReference`=7 AND `ConditionTarget`=0 AND `ConditionValue1`=755 AND `ConditionValue2`=500 AND `ConditionValue3`=0;
DELETE FROM `conditions` WHERE `SourceTypeOrReferenceId`=1 AND `SourceGroup`=900037 AND `SourceEntry`=95470 AND `SourceId`=0 AND `ElseGroup`=0 AND `ConditionTypeOrReference`=7 AND `ConditionTarget`=0 AND `ConditionValue1`=755 AND `ConditionValue2`=500 AND `ConditionValue3`=0;
DELETE FROM `conditions` WHERE `SourceTypeOrReferenceId`=1 AND `SourceGroup`=900038 AND `SourceEntry`=95470 AND `SourceId`=0 AND `ElseGroup`=0 AND `ConditionTypeOrReference`=7 AND `ConditionTarget`=0 AND `ConditionValue1`=755 AND `ConditionValue2`=500 AND `ConditionValue3`=0;
DELETE FROM `conditions` WHERE `SourceTypeOrReferenceId`=1 AND `SourceGroup`=900037 AND `SourceEntry`=95471 AND `SourceId`=0 AND `ElseGroup`=0 AND `ConditionTypeOrReference`=7 AND `ConditionTarget`=0 AND `ConditionValue1`=755 AND `ConditionValue2`=500 AND `ConditionValue3`=0;
DELETE FROM `conditions` WHERE `SourceTypeOrReferenceId`=1 AND `SourceGroup`=900038 AND `SourceEntry`=95471 AND `SourceId`=0 AND `ElseGroup`=0 AND `ConditionTypeOrReference`=7 AND `ConditionTarget`=0 AND `ConditionValue1`=755 AND `ConditionValue2`=500 AND `ConditionValue3`=0;
INSERT INTO `conditions` (`SourceTypeOrReferenceId`,`SourceGroup`,`SourceEntry`,`SourceId`,`ElseGroup`,`ConditionTypeOrReference`,`ConditionTarget`,`ConditionValue1`,`ConditionValue2`,`ConditionValue3`,`NegativeCondition`,`ErrorType`,`ErrorTextId`,`ScriptName`,`Comment`) VALUES
(1,900021,95470,0,0,7,0,755,500,0,0,0,0,'',NULL),
(1,900022,95470,0,0,7,0,755,500,0,0,0,0,'',NULL),
(1,900021,95471,0,0,7,0,755,500,0,0,0,0,'',NULL),
(1,900022,95471,0,0,7,0,755,500,0,0,0,0,'',NULL),
(1,900023,95470,0,0,7,0,755,500,0,0,0,0,'',NULL),
(1,900024,95470,0,0,7,0,755,500,0,0,0,0,'',NULL),
(1,900023,95471,0,0,7,0,755,500,0,0,0,0,'',NULL),
(1,900024,95471,0,0,7,0,755,500,0,0,0,0,'',NULL),
(1,900037,95470,0,0,7,0,755,500,0,0,0,0,'',NULL),
(1,900038,95470,0,0,7,0,755,500,0,0,0,0,'',NULL),
(1,900037,95471,0,0,7,0,755,500,0,0,0,0,'',NULL),
(1,900038,95471,0,0,7,0,755,500,0,0,0,0,'',NULL);

-- creature_template_addon
DELETE FROM `creature_template_addon` WHERE `entry` IN (56448,56586,56843,56871,56872,56873,56874,57080,59149,59544,59545,59546,59547,59553,59555,59598,62229,65317,65393);
INSERT INTO `creature_template_addon` (`entry`,`path_id`,`mount`,`bytes1`,`bytes2`,`emote`,`auras`) VALUES
(56448,0,0,0,1,0,'106062'),
(56586,0,0,0,1,0,''),
(56843,0,0,0,1,0,'107048'),
(56871,0,0,0,1,0,'111237'),
(56872,0,0,0,1,0,'107034'),
(56873,0,0,0,1,0,'107034 29266'),
(56874,0,0,0,1,0,'107052'),
(57080,0,0,33554432,1,0,'107350 111452'),
(59149,0,0,0,1,0,'107034'),
(59544,0,0,0,1,0,'107034'),
(59545,0,0,0,1,0,'107034'),
(59546,0,0,33554432,1,0,'114549 107034'),
(59547,0,0,0,1,0,'120000'),
(59553,0,0,0,1,0,'107034 114789'),
(59555,0,0,50331648,1,0,'114650'),
(59598,0,0,0,1,0,''),
(62229,0,0,0,1,0,'35356'),
(65317,0,0,0,1,0,'120000'),
(65393,0,0,0,1,0,'115165');

-- creature_equip_template
DELETE FROM `creature_equip_template` WHERE `entry` IN (62171);
INSERT INTO `creature_equip_template` (`entry`,`id`,`itemEntry1`,`itemEntry2`,`itemEntry3`) VALUES
(62171,1,89901,89901,0);

-- instance_encounters
DELETE FROM `instance_encounters` WHERE `entry` IN (1439);
INSERT INTO `instance_encounters` (`entry`,`creditType`,`creditEntry`,`lastEncounterDungeon`,`comment`) VALUES
(1439,0,56439,464,'Sha of Doubt');

-- access_requirement
DELETE FROM `access_requirement` WHERE `mapId`=960 AND `difficulty`=8;
INSERT INTO `access_requirement` (`mapId`,`difficulty`,`level_min`,`level_max`,`item`,`item2`,`quest_done_A`,`quest_done_H`,`completed_achievement`,`quest_failed_text`,`iLvl`,`comment`) VALUES
(960,8,90,0,0,0,0,0,6758,NULL,0,'Temple of the Jade Serpent - Entrance');

-- spell_script_names
DELETE FROM `spell_script_names` WHERE `spell_id`=106112 AND `ScriptName`='spell_sod_release_doubt';
DELETE FROM `spell_script_names` WHERE `spell_id`=123916 AND `ScriptName`='spell_tjs_seed_of_doubt';
INSERT INTO `spell_script_names` (`spell_id`,`ScriptName`) VALUES
(106112,'spell_sod_release_doubt'),
(123916,'spell_tjs_seed_of_doubt');

-- spell_script_names
DELETE FROM `spell_script_names` WHERE `spell_id`=106055 AND `ScriptName`='spell_wise_hydrolance';
DELETE FROM `spell_script_names` WHERE `spell_id`=106098 AND `ScriptName`='spell_wise_hydrolance_pulse';
DELETE FROM `spell_script_names` WHERE `spell_id`=106331 AND `ScriptName`='spell_wash_away_tjs';
DELETE FROM `spell_script_names` WHERE `spell_id`=106612 AND `ScriptName`='spell_wise_bubble_brust';
DELETE FROM `spell_script_names` WHERE `spell_id`=106653 AND `ScriptName`='spell_tjs_sha_residue';
DELETE FROM `spell_script_names` WHERE `spell_id`=107045 AND `ScriptName`='spell_yu_lon_tjs_jade_fire';
DELETE FROM `spell_script_names` WHERE `spell_id`=113316 AND `ScriptName`='spell_tjs_intensity';
DELETE FROM `spell_script_names` WHERE `spell_id`=115167 AND `ScriptName`='spell_tjs_corrupted_waters_damage';
DELETE FROM `spell_script_names` WHERE `spell_id`=132387 AND `ScriptName`='spell_liu_flameheart_cleansing_breath';
INSERT INTO `spell_script_names` (`spell_id`,`ScriptName`) VALUES
(106055,'spell_wise_hydrolance'),
(106098,'spell_wise_hydrolance_pulse'),
(106331,'spell_wash_away_tjs'),
(106612,'spell_wise_bubble_brust'),
(106653,'spell_tjs_sha_residue'),
(107045,'spell_yu_lon_tjs_jade_fire'),
(113316,'spell_tjs_intensity'),
(115167,'spell_tjs_corrupted_waters_damage'),
(132387,'spell_liu_flameheart_cleansing_breath');

-- ScriptName bindings
UPDATE `creature_template` SET `ScriptName`='npc_corrupt_living_water' WHERE `entry`=56511;
UPDATE `creature_template` SET `ScriptName`='boss_yu_lon_tjs' WHERE `entry`=56762;
UPDATE `creature_template` SET `ScriptName`='npc_wave_trigger_liu_flameheart' WHERE `entry`=56789;
UPDATE `creature_template` SET `ScriptName`='npc_sun' WHERE `entry`=56915;
UPDATE `creature_template` SET `ScriptName`='npc_corrupted_scroll' WHERE `entry`=57080;
UPDATE `creature_template` SET `ScriptName`='npc_minion_of_doubt' WHERE `entry`=57109;
UPDATE `creature_template` SET `ScriptName`='npc_lesser_sha' WHERE `entry`=58319;
UPDATE `creature_template` SET `ScriptName`='npc_zao' WHERE `entry`=58826;
UPDATE `creature_template` SET `ScriptName`='npc_haunting_sha' WHERE `entry`=58856;
UPDATE `creature_template` SET `ScriptName`='npc_strife_peril' WHERE `entry`=59051;
UPDATE `creature_template` SET `ScriptName`='npc_nodding_tiger' WHERE `entry`=59544;
UPDATE `creature_template` SET `ScriptName`='npc_golden_beetle' WHERE `entry`=59545;
UPDATE `creature_template` SET `ScriptName`='npc_talking_fish' WHERE `entry`=59546;
UPDATE `creature_template` SET `ScriptName`='npc_jiang_xiang' WHERE `entry`=59547;
UPDATE `creature_template` SET `ScriptName`='npc_songbird_queen' WHERE `entry`=59553;
UPDATE `creature_template` SET `ScriptName`='npc_lesser_sha' WHERE `entry`=59598;
UPDATE `creature_template` SET `ScriptName`='npc_strife_peril' WHERE `entry`=59726;
UPDATE `creature_template` SET `ScriptName`='npc_corrupt_living_water' WHERE `entry`=59873;
UPDATE `creature_template` SET `ScriptName`='npc_lie_flameheart_trash_base_ai' WHERE `entry`=62171;
UPDATE `creature_template` SET `ScriptName`='npc_jiang_xiang' WHERE `entry`=65317;
UPDATE `creature_template` SET `ScriptName`='npc_minion_of_doubt' WHERE `entry`=65362;
UPDATE `creature_template` SET `ScriptName`='npc_wise_stalker' WHERE `entry`=65393;

-- conditions (spell targets)
DELETE FROM `conditions` WHERE `SourceTypeOrReferenceId` IN (13,17) AND `SourceEntry` IN (132387);
INSERT INTO `conditions` (`SourceTypeOrReferenceId`,`SourceGroup`,`SourceEntry`,`SourceId`,`ElseGroup`,`ConditionTypeOrReference`,`ConditionTarget`,`ConditionValue1`,`ConditionValue2`,`ConditionValue3`,`NegativeCondition`,`ErrorType`,`ErrorTextId`,`ScriptName`,`Comment`) VALUES
(13,1,132387,0,0,31,0,3,56732,0,0,0,0,'',NULL);

-- Summoned creature templates: level, class and hostile faction
UPDATE `creature_template` SET `minlevel`=90, `maxlevel`=90, `unit_class`=8, `faction_A`=16, `faction_H`=16 WHERE `entry` IN (59051,59726);
