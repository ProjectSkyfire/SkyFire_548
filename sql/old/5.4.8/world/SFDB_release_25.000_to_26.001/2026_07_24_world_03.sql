-- Finzy Watchwoozle <Stable Master>
DELETE FROM `npc_vendor` WHERE `entry`=44191;
INSERT INTO `npc_vendor` (`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`) VALUES
(44191, 1, 37460, 0, 0, 0, 1);

-- Marpi Greenwrench <Herbalism Supplies>
DELETE FROM `npc_vendor` WHERE `entry`=44193;
INSERT INTO `npc_vendor` (`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`) VALUES
(44193, 0, 3371, 0, 0, 0, 1),
(44193, 1, 22250, 0, 0, 0, 1);

-- Bobber Spazzspark <Fishing Supplies>
DELETE FROM `npc_vendor` WHERE `entry`=44192;
INSERT INTO `npc_vendor` (`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`) VALUES
(44192, 0, 6256, 0, 0, 0, 1), -- Fishing Pole
(44192, 1, 6365, 1, 9000, 0, 1), -- Strong Fishing Pole
(44192, 2, 6529, 0, 0, 0, 1), -- Shiny Bauble
(44192, 3, 6530, 0, 0, 0, 1), -- Nightcrawlers
(44192, 4, 6532, 0, 0, 0, 1), -- Bright Baubles
(44192, 5, 6533, 2, 9000, 0, 1); -- Aquadynamic Fish Attractor

-- Smity Hammerhead <Blacksmithing Supplies>
DELETE FROM `npc_vendor` WHERE `entry`=44195;
INSERT INTO `npc_vendor` (`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`) VALUES
(44195, 0, 2880, 0, 0, 0, 1),
(44195, 0, 2901, 0, 0, 0, 1),
(44195, 0, 3466, 0, 0, 0, 1),
(44195, 0, 3857, 0, 0, 0, 1),
(44195, 0, 5956, 0, 0, 0, 1),
(44195, 0, 18567, 0, 0, 0, 1);
