-- Howling Fjord: quest relations that exist in the other 3.3.5 and 5.4.8 databases but not here.
-- 11189 "One Last Time": Overseer Irena Stonemantle ends it (5.4.8).
-- 11411 "Camp Winterhoof" (Horde): Cormath the Courier gives it, Chieftain Ashtotem ends it.
-- 11452 "The Slumbering King" (Alliance): Captain Adams ends it; it is started by the item.
DELETE FROM `creature_queststarter` WHERE (`id`, `quest`) IN ((24125, 11411));
INSERT INTO `creature_queststarter` (`id`, `quest`) VALUES
(24125, 11411);

DELETE FROM `creature_questender` WHERE (`id`, `quest`) IN ((23891, 11189), (24129, 11411), (23749, 11452));
INSERT INTO `creature_questender` (`id`, `quest`) VALUES
(23891, 11189),
(24129, 11411),
(23749, 11452);
