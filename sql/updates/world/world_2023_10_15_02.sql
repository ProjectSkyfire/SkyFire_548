-- Lions for Lambs (Monk)
DELETE FROM `creature_queststarter` WHERE `quest`=31140;
INSERT INTO `creature_queststarter` (`id`, `quest`) VALUES
(197, 31140);
DELETE FROM `creature_questender` WHERE `quest`=31140;
INSERT INTO `creature_questender` (`id`, `quest`) VALUES
(197, 31140);

-- Join the Battle! (Monk)
DELETE FROM `creature_questender` WHERE `quest`=31143;
INSERT INTO `creature_questender` (`id`, `quest`) VALUES
(823, 31143);

-- They Sent Assassins (Monk)
DELETE FROM `creature_queststarter` WHERE `quest`=31144;
INSERT INTO `creature_queststarter` (`id`, `quest`) VALUES
(823, 31144);
DELETE FROM `creature_questender` WHERE `quest`=31144;
INSERT INTO `creature_questender` (`id`, `quest`) VALUES
(823, 31144);

-- The Rear is Clear (Monk)
DELETE FROM `creature_queststarter` WHERE `quest`=31145;
INSERT INTO `creature_queststarter` (`id`, `quest`) VALUES
(823, 31145);
DELETE FROM `creature_questender` WHERE `quest`=31145;
INSERT INTO `creature_questender` (`id`, `quest`) VALUES
(197, 31145);
