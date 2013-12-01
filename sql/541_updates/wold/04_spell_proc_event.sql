DELETE FROM spell_proc_event WHERE SpellFamilyName NOT IN (0, 13);
ALTER TABLE spell_proc_event ADD SpellFamilyMask3 INT(10) UNSIGNED DEFAULT 0  NOT NULL AFTER SpellFamilyMask2;
