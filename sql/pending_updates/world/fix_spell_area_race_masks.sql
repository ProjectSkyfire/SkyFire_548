-- spell_area still carries the pre-5.4.8 race masks: 690 is the Horde without Goblin and Pandaren, 1101 the Alliance
-- without Worgen and Pandaren. Goblins, Worgen and Pandaren therefore never got these auras (disguises, phase auras,
-- invisibility detection and faction markers). The 5.4.8 masks are 33555378 (Horde with Goblin and Pandaren) and
-- 18875469 (Alliance with Worgen and Pandaren); the quest ids of each row already match the 5.4.8 data.
UPDATE `spell_area` SET `racemask` = 33555378 WHERE `racemask` = 690 AND `spell` IN (30181, 35480, 35481, 49416, 49417, 58354, 58551, 60197, 70056, 73822);
UPDATE `spell_area` SET `racemask` = 18875469 WHERE `racemask` = 1101 AND `spell` IN (30181, 42316, 49416, 49417, 58354, 58530, 60194, 70057, 73828);
