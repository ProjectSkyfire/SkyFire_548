-- Shattrath City: Report to Nasuun had no starter or ender, and Learning to Fly (Alliance and Horde) could not be
-- taken from Nutral or handed in to the Shattrath flight masters.

DELETE FROM `creature_queststarter` WHERE (`id` = 25034 AND `quest` = 11517) OR (`id` = 18940 AND `quest` = 11497) OR (`id` = 18940 AND `quest` = 11498);
INSERT INTO `creature_queststarter` (`id`, `quest`) VALUES
(25034, 11517),
(18940, 11497),
(18940, 11498);
DELETE FROM `creature_questender` WHERE (`id` = 24932 AND `quest` = 11517) OR (`id` = 20511 AND `quest` = 11497) OR (`id` = 20500 AND `quest` = 11498);
INSERT INTO `creature_questender` (`id`, `quest`) VALUES
(24932, 11517),
(20511, 11497),
(20500, 11498);
