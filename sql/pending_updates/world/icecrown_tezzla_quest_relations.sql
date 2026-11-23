-- Icecrown: Fringe Engineer Tezzla (Horde) gives and ends five of the Valley of Lost Hope dailies; the relations
-- exist in the other databases but not here (3.3.5 and AzerothCore agree, 5.4.8 keeps the enders).
DELETE FROM `creature_queststarter` WHERE (`id`, `quest`) IN ((32430, 13373), (32430, 13374), (32430, 13376), (32430, 13406));
INSERT INTO `creature_queststarter` (`id`, `quest`) VALUES
(32430, 13373),
(32430, 13374),
(32430, 13376),
(32430, 13406);

DELETE FROM `creature_questender` WHERE (`id`, `quest`) IN ((32430, 13373), (32430, 13374), (32430, 13376), (32430, 13379), (32430, 13406));
INSERT INTO `creature_questender` (`id`, `quest`) VALUES
(32430, 13373),
(32430, 13374),
(32430, 13376),
(32430, 13379),
(32430, 13406);
