-- Hellfire Peninsula: the starters and enders of the Citadel attunement quests (Entry Into the Citadel,
-- Grand Master Rohok, Rohok's Request and Hotter than Hell) were missing.

-- Citadel attunement and master profession quests: starters and enders
DELETE FROM `creature_queststarter` WHERE (`id` = 3230 AND `quest` = 10756) OR (`id` = 16583 AND `quest` = 10757) OR (`id` = 16583 AND `quest` = 10758);
INSERT INTO `creature_queststarter` (`id`, `quest`) VALUES
(3230, 10756),
(16583, 10757),
(16583, 10758);
DELETE FROM `creature_questender` WHERE (`id` = 16819 AND `quest` = 10754) OR (`id` = 3230 AND `quest` = 10755) OR (`id` = 16583 AND `quest` = 10756) OR (`id` = 16583 AND `quest` = 10757) OR (`id` = 16583 AND `quest` = 10758);
INSERT INTO `creature_questender` (`id`, `quest`) VALUES
(16819, 10754),
(3230, 10755),
(16583, 10756),
(16583, 10757),
(16583, 10758);
