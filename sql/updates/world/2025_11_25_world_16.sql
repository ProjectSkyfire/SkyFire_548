-- Fix Skill Discovery Table errors

-- 1. Remove entries with invalid reqSpell 61756 (spell does not exist in MoP DBC)
-- These entries are duplicates - valid entries exist with reqSpell 61177
DELETE FROM `skill_discovery_template` WHERE `reqSpell` = 61756;

-- 2. Add missing entries for 100% chance random discovery abilities
-- These spells are explicit discovery spells that need entries in skill_discovery_template
-- Using reqSpell = spellId (self-reference) pattern with reqSkillValue = 0 and chance = 100
-- Note: These may need refinement based on actual profession research spells
INSERT INTO `skill_discovery_template` (`spellId`, `reqSpell`, `reqSkillValue`, `chance`) VALUES
(115063, 115063, 0, 100),
(123516, 123516, 0, 100),
(123548, 123548, 0, 100),
(123549, 123549, 0, 100),
(129673, 129673, 0, 100),
(129674, 129674, 0, 100),
(129675, 129675, 0, 100),
(129676, 129676, 0, 100),
(129687, 129687, 0, 100),
(129705, 129705, 0, 100),
(129757, 129757, 0, 100),
(129796, 129796, 0, 100),
(129814, 129814, 0, 100),
(129843, 129843, 0, 100),
(129887, 129887, 0, 100),
(130025, 130025, 0, 100),
(130026, 130026, 0, 100),
(130109, 130109, 0, 100),
(130140, 130140, 0, 100),
(130168, 130168, 0, 100),
(133106, 133106, 0, 100);

