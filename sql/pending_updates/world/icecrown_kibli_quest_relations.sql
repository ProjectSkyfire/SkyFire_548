-- Icecrown: Kibli Killohertz (Alliance) gives and ends five of the Valley of Lost Hope dailies; the relations
-- exist in the other databases but not here (3.3.5 and AzerothCore agree, 5.4.8 keeps the enders).
DELETE FROM `creature_queststarter` WHERE (`id`, `quest`) IN ((32444, 13380), (32444, 13381), (32444, 13382), (32444, 13404));
INSERT INTO `creature_queststarter` (`id`, `quest`) VALUES
(32444, 13380),
(32444, 13381),
(32444, 13382),
(32444, 13404);

DELETE FROM `creature_questender` WHERE (`id`, `quest`) IN ((32444, 13380), (32444, 13381), (32444, 13382), (32444, 13383), (32444, 13404));
INSERT INTO `creature_questender` (`id`, `quest`) VALUES
(32444, 13380),
(32444, 13381),
(32444, 13382),
(32444, 13383),
(32444, 13404);
