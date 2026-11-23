-- Fresh out of the Grave (24959) is the first quest of the Undead in Deathknell and starts with Agatha (49044). Agatha
-- carries "Generic Quest Invisibility 1" (49414) on her template, which hides her from players without "Generic Quest
-- Invisibility Detection 1" (49416), and nothing granted that detection, so a new Undead character could not see her
-- and had no starting quest. As in Legends of Azeroth 5.4.8 the detection is applied in Deathknell (area 6454) to
-- Undead players until the quest has been completed.

DELETE FROM `spell_area` WHERE `spell` = 49416 AND `area` = 6454;
INSERT INTO `spell_area` (`spell`, `area`, `quest_start`, `quest_end`, `aura_spell`, `racemask`, `gender`, `autocast`, `quest_start_status`, `quest_end_status`) VALUES
(49416, 6454, 0, 24959, 0, 16, 2, 1, 74, 11);
