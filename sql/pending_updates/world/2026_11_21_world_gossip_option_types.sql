-- Gossip menu options that did not do what their text says. The shop, "Make this inn your home", flight master, battlemaster,
-- spirit healer and bank options of these menus were plain gossip options (type 0 or 1, no NPC flag), so clicking them did
-- nothing; some vendor options had the wrong type or flag. They get the type and NPC flag the 5.4.8 menus use. Matched by
-- menu and option text against Legends of Azeroth 5.4.8; the option ids are this database's.

-- vendor options (38)
UPDATE `gossip_menu_option` SET `OptionType`=3, `OptionNpcflag`=128 WHERE (`MenuID`, `OptionID`) IN ((342, 1), (344, 1), (345, 1), (349, 1), (441, 1), (1291, 2), (1582, 1), (3161, 1), (3162, 1), (3186, 1), (4001, 1), (4002, 0), (4003, 0), (4004, 1), (4005, 1), (4006, 1), (5241, 2), (5541, 0), (6917, 2), (6924, 0), (7288, 1), (7857, 5), (7869, 3), (7948, 1), (7949, 1), (8659, 0), (8884, 1), (9112, 1), (9427, 1), (9478, 1), (9549, 1), (10029, 0), (10358, 1), (10426, 1), (11727, 0), (11743, 1), (12759, 3), (12761, 0));

-- innkeeper options (20)
UPDATE `gossip_menu_option` SET `OptionType`=8, `OptionNpcflag`=65536 WHERE (`MenuID`, `OptionID`) IN ((342, 0), (345, 0), (347, 0), (349, 0), (441, 0), (1581, 1), (1582, 0), (6525, 1), (7857, 0), (7869, 2), (7896, 1), (7948, 0), (7949, 0), (8884, 0), (8903, 1), (9112, 0), (9427, 0), (9460, 0), (9478, 0), (9763, 0));

-- battlemaster options (7)
UPDATE `gossip_menu_option` SET `OptionType`=12, `OptionNpcflag`=1048576 WHERE (`MenuID`, `OptionID`) IN ((6508, 0), (8073, 0), (8075, 0), (8076, 0), (8077, 0), (9983, 0), (10602, 0));

-- flight master options (5)
UPDATE `gossip_menu_option` SET `OptionType`=4, `OptionNpcflag`=8192 WHERE (`MenuID`, `OptionID`) IN ((10861, 1), (11885, 0), (11892, 1), (12370, 1), (13046, 0));

-- banker options (2)
UPDATE `gossip_menu_option` SET `OptionType`=9, `OptionNpcflag`=131072 WHERE (`MenuID`, `OptionID`) IN ((7812, 0), (10358, 0));

-- spirit healer options (1)
UPDATE `gossip_menu_option` SET `OptionType`=6, `OptionNpcflag`=16384 WHERE (`MenuID`, `OptionID`) IN ((9800, 0));
