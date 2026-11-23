-- Redridge Mountains, "In Search of Bravo Company" (26586) and "Breaking Out is Hard to Do" (26587): the captive
-- Bravo Company soldiers carry an "Apply Quest Invis Zone" aura (Messner 81003, Jorgensen 81009, Krakauer 81018,
-- Danforth 81079) and stay invisible to everyone who does not carry the matching "Detect: Quest Invis Zone" aura.
-- The spell_area rows that hand out the detect auras were missing, so Messner at Alther's Mill and Jorgensen at
-- Render's Camp could never be seen, and the quest marker pointed at an empty spot. Each detect aura applies in the
-- soldier's area until the quest that frees him is rewarded. Based on Legends of Azeroth 5.4.8.

DELETE FROM `spell_area` WHERE `spell` IN (81004, 81010, 81019, 81080) AND `area` IN (97, 996, 998);
INSERT INTO `spell_area` (`spell`, `area`, `quest_start`, `quest_end`, `aura_spell`, `racemask`, `gender`, `autocast`, `quest_start_status`, `quest_end_status`) VALUES
(81004, 97, 0, 26587, 0, 0, 2, 1, 64, 11),
(81010, 996, 0, 26560, 0, 0, 2, 1, 64, 11),
(81019, 998, 0, 26561, 0, 0, 2, 1, 64, 11),
(81080, 998, 0, 26562, 0, 0, 2, 1, 64, 11);
