DELETE FROM battleground_template WHERE id =108;
INSERT INTO `battleground_template` (`id`, `MinPlayersPerTeam`, `MaxPlayersPerTeam`, `MinLvl`, `MaxLvl`, `AllianceStartLoc`, `AllianceStartO`, `HordeStartLoc`, `HordeStartO`, `StartMaxDist`, `Weight`, `ScriptName`, `Comment`) VALUES 
(108, 5, 10, 81, 85, 1726, 0, 1727, 0, 75, 1, '', 'Twin Peaks');

DELETE FROM trinity_string WHERE entry IN (12000, 12001, 12002, 12003, 12004, 12005, 12006, 12007, 12008, 12009, 12010, 12011, 12012, 12013, 12014);   
INSERT INTO trinity_string (entry, content_default, content_loc1, content_loc2, content_loc3, content_loc4, content_loc5, content_loc6, content_loc7, content_loc8) values
('12001','The battle for Twin Peaks begins in 1 minute.',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL),
('12000','The battle for Twin Peaks begins in 2 minutes.',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL),
('12002','The battle for Twin Peaks begins in 30 seconds. Prepare yourselves!',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL),
('12003','Let the battle for Twin Peaks begin!',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL),
('12004','$n captured the Horde flag!',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL),
('12005','$n captured the Alliance flag!',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL),
('12006','The Horde flag was dropped by $n!',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL),
('12007','The Alliance Flag was dropped by $n!',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL),
('12008','The Alliance Flag was returned to its base by $n!',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL),
('12009','The Horde flag was returned to its base by $n!',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL),
('12010','The Horde flag was picked up by $n!',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL),
('12011','The Alliance Flag was picked up by $n!',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL),
('12012','The flags are now placed at their bases.',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL),
('12013','The Alliance flag is now placed at its base.',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL),
('12014','The Horde flag is now placed at its base.',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL);

DELETE FROM game_graveyard_zone WHERE id IN (1750, 1749, 1726, 1727, 1729, 1728);
INSERT INTO game_graveyard_zone VALUES
(1750, 5031, 67),
(1749, 5031, 469),
(1726, 5031, 469),
(1727, 5031, 67),
(1729, 5031, 469),
(1728, 5031, 67);