-- Link the 600-skill gathering perks to their existing rank chains.
-- SkillLineAbility.dbc identifies the preceding rank for each spell:
-- Toughness 102163 -> 74496, Master of Anatomy 102219 -> 74495,
-- Lifeblood 121279 -> 74497. Lifeblood includes an initial rank at skill 1.
-- Without these links, the previous perk remains active alongside the final rank.
-- Mining then exposes three profession abilities and aborts the client's UI update.
DELETE FROM `spell_ranks` WHERE `spell_id` IN (102163, 102219, 121279);
INSERT INTO `spell_ranks` (`first_spell_id`, `spell_id`, `spell_rank`) VALUES
(53120, 102163, 8),
(53125, 102219, 8),
(81708, 121279, 9);
