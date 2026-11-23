-- Gilneas, first chapter: the phase aura 59073 (phase 170) in Gilneas City (4714, 4755) from "Lockdown!" (14078)
-- until "The Rebel Lord's Arsenal" (14159), so the spawns of phase 170 show from the start of the chapter. Until now
-- phase 170 only came from the area rules after quest 14094; those stay until the next chapters replace them.
DELETE FROM `spell_area` WHERE `spell` = 59073 AND `area` IN (4714, 4755);
INSERT INTO `spell_area` (`spell`, `area`, `quest_start`, `quest_end`, `aura_spell`, `racemask`, `gender`, `autocast`, `quest_start_status`, `quest_end_status`) VALUES
(59073, 4714, 14078, 14159, 0, 0, 2, 1, 64, 11),
(59073, 4755, 14078, 14159, 0, 0, 2, 1, 64, 11);
