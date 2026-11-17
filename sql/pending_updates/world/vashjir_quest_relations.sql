-- Vashj'ir: quest givers and enders the 5.4.8 data has for these quests and the base data lacks (the same NPC in its
-- other story stage, or the second NPC of a quest).

DELETE FROM `creature_queststarter` WHERE (`id` = 36915 AND `quest` = 14482) OR (`id` = 39226 AND `quest` = 25471) OR (`id` = 39667 AND `quest` = 27685) OR (`id` = 39876 AND `quest` = 25222) OR (`id` = 39882 AND `quest` = 25216) OR (`id` = 39883 AND `quest` = 25587) OR (`id` = 39887 AND `quest` = 25405) OR (`id` = 40105 AND `quest` = 25558) OR (`id` = 40105 AND `quest` = 25949) OR (`id` = 40221 AND `quest` = 25359) OR (`id` = 40916 AND `quest` = 25958) OR (`id` = 40920 AND `quest` = 27717) OR (`id` = 41341 AND `quest` = 27708) OR (`id` = 41531 AND `quest` = 25890) OR (`id` = 41531 AND `quest` = 25995) OR (`id` = 41600 AND `quest` = 26182) OR (`id` = 41618 AND `quest` = 25924) OR (`id` = 41636 AND `quest` = 25984) OR (`id` = 41640 AND `quest` = 26057) OR (`id` = 41663 AND `quest` = 26092) OR (`id` = 41663 AND `quest` = 26221) OR (`id` = 41711 AND `quest` = 25941) OR (`id` = 41711 AND `quest` = 25946) OR (`id` = 41770 AND `quest` = 25968) OR (`id` = 41770 AND `quest` = 25969) OR (`id` = 41810 AND `quest` = 25991) OR (`id` = 41863 AND `quest` = 26000) OR (`id` = 41908 AND `quest` = 26086) OR (`id` = 41908 AND `quest` = 26090) OR (`id` = 42197 AND `quest` = 26140) OR (`id` = 44540 AND `quest` = 26194) OR (`id` = 46458 AND `quest` = 25670) OR (`id` = 46463 AND `quest` = 25743);
INSERT INTO `creature_queststarter` (`id`, `quest`) VALUES
(36915, 14482),
(39226, 25471),
(39667, 27685),
(39876, 25222),
(39882, 25216),
(39883, 25587),
(39887, 25405),
(40105, 25558),
(40105, 25949),
(40221, 25359),
(40916, 25958),
(40920, 27717),
(41341, 27708),
(41531, 25890),
(41531, 25995),
(41600, 26182),
(41618, 25924),
(41636, 25984),
(41640, 26057),
(41663, 26092),
(41663, 26221),
(41711, 25941),
(41711, 25946),
(41770, 25968),
(41770, 25969),
(41810, 25991),
(41863, 26000),
(41908, 26086),
(41908, 26090),
(42197, 26140),
(44540, 26194),
(46458, 25670),
(46463, 25743);

DELETE FROM `creature_questender` WHERE (`id` = 42197 AND `quest` = 26144) OR (`id` = 42197 AND `quest` = 26149) OR (`id` = 42197 AND `quest` = 26154);
INSERT INTO `creature_questender` (`id`, `quest`) VALUES
(42197, 26144),
(42197, 26149),
(42197, 26154);

