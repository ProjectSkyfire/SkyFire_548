-- Icecrown: quest relations that exist in the other databases but not here.
-- 12852 "The Admiral Revealed" (Lord-Commander Arete 29344), 13293 "Get to Ymirheim!" (Blast Thunderbomb) and
-- 24798 "A Victory For The Sunreavers" (Myralion Sunblaze) had only a giver that is not spawned or none.
DELETE FROM `creature_queststarter` WHERE (`id`, `quest`) IN ((29344, 12852), (31781, 13293), (36642, 24798));
INSERT INTO `creature_queststarter` (`id`, `quest`) VALUES
(29344, 12852),
(31781, 13293),
(36642, 24798);
