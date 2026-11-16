-- Grizzly Hills: quest relations that exist in 5.4.8 but not here.
-- 13524 "Escape from Silverbrook": Lieutenant Dumont ends it.
DELETE FROM `creature_questender` WHERE (`id`, `quest`) IN ((26875, 13524));
INSERT INTO `creature_questender` (`id`, `quest`) VALUES
(26875, 13524);
