-- Twilight Highlands: "Four Heads are Better than None" (27501, Horde) follows "Signal the Attack" (27590) in its chain and leads
-- to "Up to the Citadel" (27503), but the quest had no ender: Lady Cozwynn (45665) ends it in the 5.4.8 data, as Cassius the White
-- (45669) ends the Alliance twin 27500. Based on Legends of Azeroth 5.4.8.

DELETE FROM `creature_questender` WHERE `quest` = 27501;
INSERT INTO `creature_questender` (`id`, `quest`) VALUES
(45665, 27501);
