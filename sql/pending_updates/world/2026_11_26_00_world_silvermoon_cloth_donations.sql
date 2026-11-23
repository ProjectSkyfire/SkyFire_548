-- Silvermoon City: the five cloth donation quests (10359 to 10363) had no giver and no ender, so Sorim Lightsong,
-- the Horde Cloth Quartermaster, offered nothing. Based on Legends of Azeroth 5.4.8; the quest texts and rewards
-- (Silvermoon City reputation) are already in the quest table.
DELETE FROM `creature_queststarter` WHERE `id`=20612 AND `quest` IN (10359, 10360, 10361, 10362, 10363);
INSERT INTO `creature_queststarter` (`id`, `quest`) VALUES
(20612, 10359),
(20612, 10360),
(20612, 10361),
(20612, 10362),
(20612, 10363);
DELETE FROM `creature_questender` WHERE `id`=20612 AND `quest` IN (10359, 10360, 10361, 10362, 10363);
INSERT INTO `creature_questender` (`id`, `quest`) VALUES
(20612, 10359),
(20612, 10360),
(20612, 10361),
(20612, 10362),
(20612, 10363);
