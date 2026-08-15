-- Pera Firestone <Fireworks Vendor>
DELETE FROM `npc_vendor` WHERE `entry`=64480;
INSERT INTO `npc_vendor` (`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`) VALUES
(64480, 0, 5740, 0, 0, 0, 1),
(64480, 1, 9314, 0, 0, 0, 1);

-- Wilhem Ken <Bread Vendor>
DELETE FROM `npc_vendor` WHERE `entry`=64129;
INSERT INTO `npc_vendor` (`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`) VALUES
(64129, 0, 81400, 0, 0, 0, 1),
(64129, 1, 81408, 0, 0, 0, 1),
(64129, 2, 82451, 0, 0, 0, 1);

-- Michael Bedernik <Meats>
DELETE FROM `npc_vendor` WHERE `entry`=64130;
INSERT INTO `npc_vendor` (`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`) VALUES
(64130, 0, 81405, 0, 0, 0, 1),
(64130, 1, 81413, 0, 0, 0, 1);
