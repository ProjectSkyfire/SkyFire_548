-- Zul'Drak: quest relations that exist in the other databases but not here.
-- 12601 and 12602 "The Alchemist's Apprentice": Alchemist Finklestein (5.4.8).
-- 13097 "Unfinished Business": Har'koa gives it, Tol'mar ends it. 13099 "Just Checkin'": Chronicler To'kini gives it,
-- Chronicler Bah'Kini ends it.
DELETE FROM `creature_queststarter` WHERE (`id`, `quest`) IN ((28205, 12601), (28205, 12602), (28401, 13097), (28527, 13099));
INSERT INTO `creature_queststarter` (`id`, `quest`) VALUES
(28205, 12601),
(28205, 12602),
(28401, 13097),
(28527, 13099);

DELETE FROM `creature_questender` WHERE (`id`, `quest`) IN ((28205, 12601), (28205, 12602), (30671, 13097), (30676, 13099));
INSERT INTO `creature_questender` (`id`, `quest`) VALUES
(28205, 12601),
(28205, 12602),
(30671, 13097),
(30676, 13099);
