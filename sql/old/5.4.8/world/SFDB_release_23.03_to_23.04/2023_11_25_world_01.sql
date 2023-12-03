DELETE FROM `npc_spellclick_spells` WHERE `npc_entry` IN (54993, 55083);
INSERT INTO `npc_spellclick_spells` (`npc_entry`, `spell_id`, `cast_flags`, `user_type`) VALUES
(54993, 102717, 0, 0),
(55083, 102717, 0, 0);
