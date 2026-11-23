-- The three "Enemies Below" breadcrumbs of the rebuilt Ragefire Chasm (31034, 31036, 31037) start with Garrosh,
-- Baine and Sylvanas and end with Commander Bagran. Here Bagran started them himself, so nobody sent players to him.
DELETE FROM `creature_queststarter` WHERE `id`=61724 AND `quest` IN (31034, 31036, 31037);
DELETE FROM `creature_queststarter` WHERE (`id`=39605 AND `quest`=31034) OR (`id`=36648 AND `quest`=31036) OR (`id`=10181 AND `quest`=31037);
INSERT INTO `creature_queststarter` (`id`, `quest`) VALUES
(39605, 31034),
(36648, 31036),
(10181, 31037);
