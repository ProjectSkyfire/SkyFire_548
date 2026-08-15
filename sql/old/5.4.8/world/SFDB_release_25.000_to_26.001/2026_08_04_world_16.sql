-- Burst of Speed: cast 108212 (snare clear/immunity) also applies 137573 (+70% speed)
DELETE FROM `spell_linked_spell` WHERE `spell_trigger` = 108212 AND `spell_effect` = 137573;
INSERT INTO `spell_linked_spell` (`spell_trigger`, `spell_effect`, `type`, `comment`) VALUES
(108212, 137573, 0, 'Burst of Speed');
