-- Uldum, Schnottz's Landing (area 5602): Gobbles and the siege tank carry quest invisibility (types 18 and 31) and
-- nothing gave players the matching detection, so they were invisible to everyone. The two detection auras of the
-- area: 88144 until "Gobbles!" (27950) is complete, 88847 always.
DELETE FROM `spell_area` WHERE `spell` IN (88144, 88847) AND `area` = 5602;
INSERT INTO `spell_area` (`spell`, `area`, `quest_start`, `quest_end`, `aura_spell`, `racemask`, `gender`, `autocast`, `quest_start_status`, `quest_end_status`) VALUES
(88144, 5602, 0, 27950, 0, 0, 2, 1, 64, 9),
(88847, 5602, 0, 0, 0, 0, 2, 1, 64, 64);
