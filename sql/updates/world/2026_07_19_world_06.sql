-- Pending world update: teach Track Pets when Battle Pets is learned.
DELETE FROM `spell_learn_spell` WHERE `entry` = 119467 AND `SpellID` = 122026;
INSERT INTO `spell_learn_spell` (`entry`, `SpellID`, `Active`) VALUES
(119467, 122026, 1);
