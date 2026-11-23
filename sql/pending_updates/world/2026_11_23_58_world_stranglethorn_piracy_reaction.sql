-- Seeing Where Your Loyalties Lie (26629) to Call of Booty (26665): once the player has joined the Bloodsail Buccaneers,
-- the aura Piracy (82942) makes the Bloodsail faction friendly in the Cape of Stranglethorn until Call of Booty is
-- handed in, so that the player can talk to Pretty Boy Duncan, Fleet Master Firallon and the crew on the Bloodsail ship.
-- The spell_area row for the aura was missing, so those quest givers stayed hostile. The row is as in Legends of Azeroth
-- 5.4.8; the source's race mask 52430847 (every race) is 0 here, as for the other rows of this table.

DELETE FROM `spell_area` WHERE `spell` = 82942 AND `area` = 5287;
INSERT INTO `spell_area` (`spell`, `area`, `quest_start`, `quest_end`, `aura_spell`, `racemask`, `gender`, `autocast`, `quest_start_status`, `quest_end_status`) VALUES
(82942, 5287, 26629, 26665, 0, 0, 2, 1, 64, 11);
