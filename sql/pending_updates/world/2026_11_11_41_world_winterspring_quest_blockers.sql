-- Winterspring: Witch Doctor Mau'ari starts Strange Life Forces.
-- Based on Legends of Azeroth 5.4.8.
DELETE FROM `creature_queststarter` WHERE `id`=10307 AND `quest`=28656;
INSERT INTO `creature_queststarter` (`id`,`quest`) VALUES
(10307,28656);
