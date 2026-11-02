-- Restore the riding trainers as quest enders for Learn to Ride in Dun Morogh.
-- https://github.com/ProjectSkyfire/SkyFire_548/issues/1517
DELETE FROM `creature_questender` WHERE (`id` = 4772 AND `quest` = 14083) OR (`id` = 7954 AND `quest` = 14084);
INSERT INTO `creature_questender` (`id`, `quest`) VALUES
(4772, 14083), -- Ultham Ironhorn: dwarf Riding Training Pamphlet
(7954, 14084); -- Binjy Featherwhistle: gnome Riding Training Pamphlet
