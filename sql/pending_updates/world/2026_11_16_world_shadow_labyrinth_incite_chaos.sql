-- Shadow Labyrinth: Blackheart the Inciter, Incite Chaos
-- Spell 33676 summons one dummy (entries 19300 to 19304) on every player it hits with the spells 33677 and 33680 to 33683.
-- The dummy charms the player with 33684, and the five dummy entries have different factions so that the charmed players
-- fight each other. The summon spells, Laugh (33722) and Threat (34915) are not in the client data, so they are defined
-- here as server-side spells. Spell effect ids 303200 to 303208 are provisional.

DELETE FROM `spell_dbc` WHERE `Id` IN (33677, 33680, 33681, 33682, 33683, 33722, 34915);
DELETE FROM `spellmisc_dbc` WHERE `Id` IN (233677, 233680, 233681, 233682, 233683, 233722, 234915);
DELETE FROM `spelleffect_dbc` WHERE `EffectSpellId` IN (33677, 33680, 33681, 33682, 33683, 33722, 34915);

INSERT INTO `spell_dbc` (`Id`, `SpellAuraOptionsId`, `SpellCastingRequirementsId`, `SpellCategoriesId`, `SpellClassOptionsId`, `SpellEquippedItemsId`, `SpellInterruptsId`, `SpellLevelsId`, `SpellTargetRestrictionsId`, `SpellMiscId`, `Comment`) VALUES
(33677, 0, 0, 0, 0, 0, 0, 0, 0, 233677, 'Incite Chaos'),
(33680, 0, 0, 0, 0, 0, 0, 0, 0, 233680, 'Incite Chaos'),
(33681, 0, 0, 0, 0, 0, 0, 0, 0, 233681, 'Incite Chaos'),
(33682, 0, 0, 0, 0, 0, 0, 0, 0, 233682, 'Incite Chaos'),
(33683, 0, 0, 0, 0, 0, 0, 0, 0, 233683, 'Incite Chaos'),
(33722, 0, 0, 0, 0, 0, 0, 0, 0, 233722, 'Laugh'),
(34915, 0, 0, 0, 0, 0, 0, 0, 0, 234915, 'Threat');

INSERT INTO `spellmisc_dbc` (`Id`, `Attributes`, `AttributesEx`, `AttributesEx2`, `AttributesEx3`, `AttributesEx4`, `AttributesEx5`, `AttributesEx6`, `AttributesEx7`, `AttributesEx8`, `AttributesEx9`, `CastingTimeIndex`, `DurationIndex`, `RangeIndex`, `SchoolMask`, `Comment`) VALUES
(233677, 256, 0, 4, 256, 0, 0, 0, 0, 0, 0, 1, 9, 1, 1, 'Incite Chaos'),
(233680, 256, 0, 4, 256, 0, 0, 0, 0, 0, 0, 1, 9, 1, 1, 'Incite Chaos'),
(233681, 256, 0, 4, 256, 0, 0, 0, 0, 0, 0, 1, 9, 1, 1, 'Incite Chaos'),
(233682, 256, 0, 4, 256, 0, 0, 0, 0, 0, 0, 1, 9, 1, 1, 'Incite Chaos'),
(233683, 256, 0, 4, 256, 0, 0, 0, 0, 0, 0, 1, 9, 1, 1, 'Incite Chaos'),
(233722, 256, 268435456, 0, 268435712, 0, 0, 0, 0, 0, 0, 1, 387, 13, 1, 'Laugh'),
(234915, 256, 0, 4, 0, 0, 0, 0, 0, 0, 0, 1, 0, 13, 1, 'Threat');

INSERT INTO `spelleffect_dbc` (`Id`, `Effect`, `EffectValueMultiplier`, `EffectApplyAuraName`, `EffectAplitude`, `EffectBasePoints`, `EffectBonusMultiplier`, `EffectDamageMultiplier`, `EffectChainTarget`, `EffectDieSides`, `EffectMechanic`, `EffectMiscValue`, `EffectMiscValueB`, `EffectRadiusIndex`, `EffectRadiusIndexMax`, `EffectRealPointsPerLevel`, `EffectSpellClassMaskA`, `EffectSpellClassMaskB`, `EffectSpellClassMaskC`, `EffectTriggerSpell`, `EffectImplicitTargetA`, `EffectImplicitTargetB`, `EffectSpellId`, `EffectIndex`) VALUES
(303200, 28, 0, 0, 0, 0, 0, 1, 0, 1, 0, 19300, 64, 16, 0, 0, 0, 0, 0, 0, 18, 0, 33677, 0),
(303201, 28, 0, 0, 0, 0, 0, 1, 0, 1, 0, 19301, 64, 16, 0, 0, 0, 0, 0, 0, 18, 0, 33680, 0),
(303202, 28, 0, 0, 0, 0, 0, 1, 0, 1, 0, 19302, 64, 16, 0, 0, 0, 0, 0, 0, 18, 0, 33681, 0),
(303203, 28, 0, 0, 0, 0, 0, 1, 0, 1, 0, 19303, 64, 16, 0, 0, 0, 0, 0, 0, 18, 0, 33682, 0),
(303204, 28, 0, 0, 0, 0, 0, 1, 0, 1, 0, 19304, 64, 16, 0, 0, 0, 0, 0, 0, 18, 0, 33683, 0),
(303205, 6, 0, 26, 0, 0, 0, 1, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 33722, 0),
(303206, 6, 0, 25, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 33722, 1),
(303207, 6, 0, 23, 2000, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 33716, 1, 0, 33722, 2),
(303208, 63, 0, 0, 0, 9999, 0, 1, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 25, 0, 34915, 0);

-- The five dummy entries have to be hostile to each other and to the players. The one script covers all of them.
UPDATE `creature_template` SET `faction_A` = 1761, `faction_H` = 1761, `ScriptName` = 'npc_blackheart_the_inciter_dummy' WHERE `entry` = 19300;
UPDATE `creature_template` SET `faction_A` = 1762, `faction_H` = 1762, `ScriptName` = 'npc_blackheart_the_inciter_dummy' WHERE `entry` = 19301;
UPDATE `creature_template` SET `faction_A` = 1763, `faction_H` = 1763, `ScriptName` = 'npc_blackheart_the_inciter_dummy' WHERE `entry` = 19302;
UPDATE `creature_template` SET `faction_A` = 1764, `faction_H` = 1764, `ScriptName` = 'npc_blackheart_the_inciter_dummy' WHERE `entry` = 19303;
UPDATE `creature_template` SET `faction_A` = 1765, `faction_H` = 1765, `ScriptName` = 'npc_blackheart_the_inciter_dummy' WHERE `entry` = 19304;

DELETE FROM `spell_script_names` WHERE `spell_id` IN (33676, 33684);
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES
(33676, 'spell_blackheart_incite_chaos'),
(33684, 'spell_blackheart_incite_chaos_charm');
