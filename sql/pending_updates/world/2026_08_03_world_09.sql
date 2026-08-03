-- Camran
DELETE FROM `npc_vendor` WHERE `entry`=44283;
INSERT INTO `npc_vendor` (`entry`, `slot`, `item`, `maxcount`, `incrtime`) VALUES 
(44283, 0, 4797, 1, 3600),
(44283, 1, 4798, 1, 3600),
(44283, 2, 4799, 1, 3600),
(44283, 3, 4800, 1, 3600),
(44283, 4, 4816, 1, 3600);

-- Quartmaster Rutherford
DELETE FROM `npc_vendor` WHERE `entry`=44303;
INSERT INTO `npc_vendor` (`entry`, `item`) VALUES
(44303, 159),
(44303, 1179),
(44303, 1205),
(44303, 4470),
(44303, 4497),
(44303, 4498),
(44303, 4541),
(44303, 4542);
