-- Eviscerate: AP * 0.18 per combo point via SpellDamageBonusDone.
-- direct/dot -1 blocks SP; combo multiply uses IsScaledByComboPoints (2098).
DELETE FROM `spell_bonus_data` WHERE `entry` = 2098;
INSERT INTO `spell_bonus_data` (`entry`, `direct_bonus`, `dot_bonus`, `ap_bonus`, `ap_dot_bonus`, `comments`) VALUES
(2098, -1, -1, 0.18, -1, 'Rogue - Eviscerate');
