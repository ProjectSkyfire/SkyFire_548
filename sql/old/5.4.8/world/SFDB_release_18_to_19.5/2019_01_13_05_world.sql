-- Remove Obsolete Spells from skill_extra_item_template
DELETE FROM skill_extra_item_template WHERE spellId IN
(42736, 17638, 11453, 3172, 67025, 62213);
