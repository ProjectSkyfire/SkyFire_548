-- Redridge Mountains, the Bravo Company rescue chain (26587, 26560, 26561, 26562, 26563): once a soldier is freed
-- (his quest rewarded) the 5.4.8 client data summons the free, friendly version (Messner 43300, Jorgensen 43305,
-- Krakauer 43303, Danforth 43302) next to the player through a "Summon" spell tied to the area. Those friendly
-- soldiers are the quest givers of the next step (26560 to 26563) and have no spawn of their own, so the chain
-- stopped after 26587. The summon lasts until "Return of the Bravo Company" (26563) is done. Based on Legends of
-- Azeroth 5.4.8.

DELETE FROM `spell_area` WHERE `spell` IN (80893, 80940, 80941, 80943) AND `area` IN (97, 996, 998);
INSERT INTO `spell_area` (`spell`, `area`, `quest_start`, `quest_end`, `aura_spell`, `racemask`, `gender`, `autocast`, `quest_start_status`, `quest_end_status`) VALUES
(80893, 97, 26587, 26563, 0, 0, 2, 1, 64, 11),
(80940, 996, 26560, 26563, 0, 0, 2, 1, 64, 11),
(80941, 998, 26561, 26563, 0, 0, 2, 1, 64, 11),
(80943, 998, 26562, 26563, 0, 0, 2, 1, 64, 11);
