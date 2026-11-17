-- No Mercy! (13233) and Make Them Pay! (13234) had no turn-in NPC, so the completed quests could not be handed in.
-- Their givers on the gunships take them back.
DELETE FROM `creature_questender` WHERE `quest` IN (13233, 13234);
INSERT INTO `creature_questender` (`id`, `quest`) VALUES
(30344, 13233),
(30824, 13234);
