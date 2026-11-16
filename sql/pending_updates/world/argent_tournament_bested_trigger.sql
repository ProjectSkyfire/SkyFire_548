-- Argent Tournament: the "Bested Trigger" spells cast by the defeated valiants and champions on the player.
-- Each one is a script effect whose base points hold the spell to cast on the player (the quest credit item or the
-- "Bested" aura that stops a second fight with the same valiant).
DELETE FROM `spell_script_names` WHERE `spell_id` IN (63060,63597,64830,64831,64832,64833,64834,64835,64836,64837,64838,64839);
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES
(63060, 'spell_icecrown_bested_trigger'),
(63597, 'spell_icecrown_bested_trigger'),
(64830, 'spell_icecrown_bested_trigger'),
(64831, 'spell_icecrown_bested_trigger'),
(64832, 'spell_icecrown_bested_trigger'),
(64833, 'spell_icecrown_bested_trigger'),
(64834, 'spell_icecrown_bested_trigger'),
(64835, 'spell_icecrown_bested_trigger'),
(64836, 'spell_icecrown_bested_trigger'),
(64837, 'spell_icecrown_bested_trigger'),
(64838, 'spell_icecrown_bested_trigger'),
(64839, 'spell_icecrown_bested_trigger');
