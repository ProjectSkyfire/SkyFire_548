DELETE FROM `spell_linked_spell` WHERE spell_trigger=91847;
INSERT INTO `spell_linked_spell` (`spell_trigger`, `spell_effect`, `comment`) VALUES
(91847, 74100, 'Life Savings: Teleport & Bind to the Lost Isles');

DELETE FROM `spell_target_position` WHERE id=74100;
INSERT INTO `spell_target_position` (`id`, `effIndex`, `target_map`, `target_position_x`, `target_position_y`, `target_position_z`, `target_orientation`) VALUES
(74100, 1, 648, 534.5, 3273.4, 0.23, 4.79);
