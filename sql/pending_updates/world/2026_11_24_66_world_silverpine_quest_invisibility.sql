-- Admiral Hatchet (44916) and Warlord Torok (44917) give the quests at the Warchief's fleet on the Silverpine shore
-- (27065, 27069, 27093, 27096, 27231 and 27232 for Hatchet; 27073, 27094, 27095 and 27226 for Torok). Both carry
-- "Apply Quest Invis Zone 5" (84240) on their template, which hides them from players without "Detect: Quest Invis
-- Zone 5" (84241), and no spell_area, quest or script grants that detection, so their quests could not be taken.
-- In Legends of Azeroth 5.4.8 neither has the aura. Hatchet keeps his Spyglass.

DELETE FROM `creature_template_addon` WHERE `entry` IN (44916, 44917);
INSERT INTO `creature_template_addon` (`entry`, `path_id`, `mount`, `bytes1`, `bytes2`, `emote`, `auras`) VALUES
(44916, 0, 0, 65536, 0, 0, '83847'),
(44917, 0, 0, 65536, 1, 0, NULL);
