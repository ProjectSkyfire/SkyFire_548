-- Burning Steppes: "Placing the Pawns" (28326, 28449) and "Locked and Loaded" (28327, 28453) had an ender, Colonel Troteman
-- (48307) for the Alliance and Eitrigg (48569) for the Horde, but no starter, so nobody offered them. Legends of
-- Azeroth 5.4.8 has the same two NPCs as their starters.

DELETE FROM `creature_queststarter` WHERE (`id` = 48307 AND `quest` IN (28326, 28327)) OR (`id` = 48569 AND `quest` IN (28449, 28453));
INSERT INTO `creature_queststarter` (`id`, `quest`) VALUES
(48307, 28326),
(48307, 28327),
(48569, 28449),
(48569, 28453);
