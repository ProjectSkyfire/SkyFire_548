-- Mount Hyjal: The Return of Baron Geddon (quest 25464) and Death to the Broodmother (quest 25553)

-- The Flameseer's Staff (item 54463, spell 75192 Weakening) is channelled on Baron Geddon and triggers spell 75193 every
-- second, the credit for the 20 "Baron Geddon Weakened" objectives of the quest. Spell 75193 is not in the client data
-- (the 5.4.8 data defines it as a server-side spell with the credit effect for creature 40334); it is defined here so that the
-- channel finds it, and the credit itself is given by the Weakening script.
-- Provisional spell effect id 303096
DELETE FROM `spell_dbc` WHERE `Id` = 75193;
DELETE FROM `spellmisc_dbc` WHERE `Id` = 275193;
DELETE FROM `spelleffect_dbc` WHERE `EffectSpellId` = 75193;
INSERT INTO `spell_dbc` (`Id`, `SpellAuraOptionsId`, `SpellCastingRequirementsId`, `SpellCategoriesId`, `SpellClassOptionsId`, `SpellEquippedItemsId`, `SpellInterruptsId`, `SpellLevelsId`, `SpellTargetRestrictionsId`, `SpellMiscId`, `Comment`) VALUES
(75193, 0, 0, 0, 0, 0, 0, 0, 0, 275193, 'Baron Geddon Credit Dummy');
INSERT INTO `spellmisc_dbc` (`Id`, `Attributes`, `AttributesEx`, `AttributesEx2`, `AttributesEx3`, `AttributesEx4`, `AttributesEx5`, `AttributesEx6`, `AttributesEx7`, `AttributesEx8`, `AttributesEx9`, `CastingTimeIndex`, `DurationIndex`, `RangeIndex`, `SchoolMask`, `Comment`) VALUES
(275193, 696254848, 1056, 273170437, 269681152, 8388736, 393224, 4608, 0, 0, 0, 1, 0, 1, 1, 'Baron Geddon Credit Dummy');
INSERT INTO `spelleffect_dbc` (`Id`, `Effect`, `EffectValueMultiplier`, `EffectApplyAuraName`, `EffectAplitude`, `EffectBasePoints`, `EffectBonusMultiplier`, `EffectDamageMultiplier`, `EffectChainTarget`, `EffectDieSides`, `EffectMechanic`, `EffectMiscValue`, `EffectMiscValueB`, `EffectRadiusIndex`, `EffectRadiusIndexMax`, `EffectRealPointsPerLevel`, `EffectSpellClassMaskA`, `EffectSpellClassMaskB`, `EffectSpellClassMaskC`, `EffectTriggerSpell`, `EffectImplicitTargetA`, `EffectImplicitTargetB`, `EffectSpellId`, `EffectIndex`) VALUES
(303096, 90, 0, 0, 0, 0, 0, 0, 0, 0, 0, 40334, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 75193, 0);

-- Desperiona, the broodmother of quest 25553, was in the data with her spawn switched off; she stands at the Hatchery
-- (Wowhead: Mount Hyjal 54.2, 85.2) and her template had the skinnable flag instead of the 5.4.8 flags
UPDATE `creature` SET `spawnMask` = 1 WHERE `guid` = 8271592 AND `id` = 40974 AND `spawnMask` = 0 AND ABS(`position_x` - 3782.34) < 0.01 AND ABS(`position_y` + 3227.22) < 0.01;
UPDATE `creature_template` SET `unit_flags` = 33280 WHERE `entry` = 40974 AND `unit_flags` = 67141632;
