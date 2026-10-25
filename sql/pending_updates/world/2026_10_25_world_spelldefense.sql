-- SPELLDEFENSE (DND) is not a player spell; it showed up in every spellbook
DELETE FROM `playercreateinfo_spell` WHERE `Spell` = 522;
