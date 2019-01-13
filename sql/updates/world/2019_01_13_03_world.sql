-- Remove wrong spell click spells
DELETE FROM npc_spellclick_spells WHERE spell_id IN (46407, 47121, 47416, 82359, 82456);

-- Pomfruit (58767) - Spellclick Spell - Force Pomfruit (111626)
DELETE FROM npc_spellclick_spells WHERE npc_entry=58767;
INSERT INTO npc_spellclick_spells (npc_entry, spell_id, cast_flags) VALUES
(58767, 111626, 0x03);
