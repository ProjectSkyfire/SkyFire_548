-- Zul'Drak: quest relations that exist in the other databases but not here.
-- 12501 and 12563 "Troll Patrol": Commander Kunz gives and ends both (3.3.5 and 5.4.8).
DELETE FROM `creature_queststarter` WHERE (`id`, `quest`) IN ((28039, 12501), (28039, 12563));
INSERT INTO `creature_queststarter` (`id`, `quest`) VALUES
(28039, 12501),
(28039, 12563);

DELETE FROM `creature_questender` WHERE (`id`, `quest`) IN ((28039, 12501), (28039, 12563));
INSERT INTO `creature_questender` (`id`, `quest`) VALUES
(28039, 12501),
(28039, 12563);
