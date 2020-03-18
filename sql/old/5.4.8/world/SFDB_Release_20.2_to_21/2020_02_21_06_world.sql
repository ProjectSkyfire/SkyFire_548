DELETE FROM `creature_queststarter` WHERE `id`=55789 AND `quest`=29548;
INSERT INTO `creature_queststarter` (`id`, `quest`) VALUES
(55789, 29548);
DELETE FROM `creature_questender` WHERE `id`=66292 AND `quest`=29548;
INSERT INTO `creature_questender` (`id`, `quest`) VALUES
(66292, 29548);
