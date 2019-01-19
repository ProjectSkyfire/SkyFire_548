-- battleground_template 5.4.8
DELETE FROM battleground_template WHERE id IN (1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 30, 32, 108, 120, 699, 708, 719, 754, 757);
INSERT INTO battleground_template (id, MinPlayersPerTeam, MaxPlayersPerTeam, MinLvl, MaxLvl, AllianceStartLoc, AllianceStartO, HordeStartLoc, HordeStartO, StartMaxDist, Weight, ScriptName, Comment) VALUES 
(1, 10, 40, 45, 90, 611, 3.16312, 610, 0.715504, 100, 5, '', 'Alterac Valley'),
(2, 5, 10, 10, 90, 769, 3.14159, 770, 0.151581, 75, 5, '', 'Warsong Gulch'),
(3, 8, 15, 10, 90, 890, 3.91571, 889, 0.813671, 75, 5, '', 'Arathi Basin'),
(4, 0, 5, 10, 90, 929, 0, 936, 3.14159, 0, 4, '', 'Nagrand Arena'),
(5, 0, 5, 10, 90, 939, 0, 940, 3.14159, 0, 4, '', 'Blades\'s Edge Arena'),
(6, 0, 5, 10, 90, 0, 0, 0, 0, 0, 1, '', 'All Arena'),
(7, 8, 15, 35, 90, 1103, 3.03123, 1104, 0.055761, 75, 4, '', 'Eye of The Storm'),
(8, 0, 5, 10, 90, 1258, 0, 1259, 3.14159, 0, 5, '', 'Ruins of Lordaeron'),
(9, 8, 15, 65, 90, 1367, 0, 1368, 0, 0, 3, '', 'Strand of the Ancients'),
(10, 0, 5, 10, 90, 1362, 0, 1363, 3.14159, 0, 3, '', 'Dalaran Sewers'),
(11, 0, 5, 10, 90, 1364, 0, 1365, 0, 0, 3, '', 'The Ring of Valor'),
(30, 10, 40, 71, 90, 1485, 0, 1486, 3.16124, 200, 3, '', 'Isle of Conquest'),
(32, 5, 40, 45, 90, 0, 0, 0, 0, 0, 1, '', 'Random battleground'),
(108, 5, 10, 85, 90, 1726, 0, 1727, 0, 75, 2, '', 'Twin Peaks'),
(120, 5, 10, 85, 90, 1740, 0, 1799, 0, 0, 2, '', 'Battle for Gilneas'),
(699, 5, 10, 90, 90, 4059, 0, 4060, 0, 0, 1, '', 'Kotmogu\'s temple'),
(708, 5, 10, 90, 90, 4062, 0, 4061, 0, 20, 0, '', 'SilverShard Mines'),
(719, 0, 5, 90, 90, 4136, 0, 4137, 0, 0, 2, '', 'Tol\'vir Arena'),
(754, 8, 15, 90, 90, 4487, 0, 4486, 0, 0, 1, '', 'Deepwind Gorge'),
(757, 0, 5, 90, 90, 4535, 0, 4534, 0, 0, 1, '', 'The Tiger\'s Peak Arena');
