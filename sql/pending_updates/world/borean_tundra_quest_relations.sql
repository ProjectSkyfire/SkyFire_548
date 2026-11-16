-- Borean Tundra: quest relations that exist in the other 3.3.5 and 5.4.8 databases but not here.
-- 11867 "Can't Get Ear-nough...": Arch Druid Lathorius ends it here but nobody gave it out.
-- 12490 "Veehja's Revenge": Veehja gives and ends it in 5.4.8; the quest and her spawn were here without the link.
DELETE FROM `creature_queststarter` WHERE (`id`, `quest`) IN ((25809, 11867), (25450, 12490));
INSERT INTO `creature_queststarter` (`id`, `quest`) VALUES
(25809, 11867),
(25450, 12490);

DELETE FROM `creature_questender` WHERE (`id`, `quest`) IN ((25450, 12490));
INSERT INTO `creature_questender` (`id`, `quest`) VALUES
(25450, 12490);
