-- Brewmaster Skye <Innkeeper>
DELETE FROM `npc_vendor` WHERE `entry`=63008;
INSERT INTO `npc_vendor` (`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`) VALUES
(63008, 0, 81407, 0, 0, 0, 1),
(63008, 1, 81415, 0, 0, 0, 1);
