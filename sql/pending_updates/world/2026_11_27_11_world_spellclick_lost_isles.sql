-- Lost Isles quests that are finished by clicking a creature, as in Legends of Azeroth 5.4.8. None of the creatures had a
-- click flag, a click row, a condition or a script, so the click did nothing.
--   Irresistible Pool Pony (24864) asks for 12 Naga Hatchlings lured: the hatchlings (38412, 44578, 44580) are clicked
--   while the quest is taken, the click (spell 71916) gives the credit of Naga Hatchling Kill Credit (38413) and the
--   hatchling goes away. Spell 71916 is not in the 5.4.8 client data, so the click rows were dropped when the table
--   loaded. It is defined here as a server-side spell with one dummy effect on the clicker, copying the attributes,
--   range and target of spell 73947, the click spell of Escape Velocity.
--   Escape Velocity (25214) asks for 8 cages launched: the Captured Goblins (39456, 42 spawns) are clicked while the
--   quest is taken (spell 73947, a dummy), which gives the credit and removes the goblin.

UPDATE `creature_template` SET `npcflag` = `npcflag` | 16777216 WHERE `entry` IN (38412, 44578, 44580, 39456);
UPDATE `creature_template` SET `AIName` = 'SmartAI' WHERE `entry` IN (38412, 44578, 44580, 39456) AND `AIName` = '' AND `ScriptName` = '';

DELETE FROM `spell_dbc` WHERE `Id` = 71916;
DELETE FROM `spellmisc_dbc` WHERE `Id` = 271916;
DELETE FROM `spelleffect_dbc` WHERE `EffectSpellId` = 71916;
INSERT INTO `spell_dbc` (`Id`, `SpellAuraOptionsId`, `SpellCastingRequirementsId`, `SpellCategoriesId`, `SpellClassOptionsId`, `SpellEquippedItemsId`, `SpellInterruptsId`, `SpellLevelsId`, `SpellTargetRestrictionsId`, `SpellMiscId`, `Comment`) VALUES
(71916, 0, 0, 0, 0, 0, 0, 0, 0, 271916, 'Irresistible Pool Pony click');
INSERT INTO `spellmisc_dbc` (`Id`, `Attributes`, `AttributesEx`, `AttributesEx2`, `AttributesEx3`, `AttributesEx4`, `AttributesEx5`, `AttributesEx6`, `AttributesEx7`, `AttributesEx8`, `AttributesEx9`, `CastingTimeIndex`, `DurationIndex`, `RangeIndex`, `SchoolMask`, `Comment`) VALUES
(271916, 538968320, 1160, 67108868, 64, 2049, 0, 0, 0, 0, 0, 1, 0, 7, 1, 'Irresistible Pool Pony click');
INSERT INTO `spelleffect_dbc` (`Id`, `Effect`, `EffectValueMultiplier`, `EffectApplyAuraName`, `EffectAplitude`, `EffectBasePoints`, `EffectBonusMultiplier`, `EffectDamageMultiplier`, `EffectChainTarget`, `EffectDieSides`, `EffectMechanic`, `EffectMiscValue`, `EffectMiscValueB`, `EffectRadiusIndex`, `EffectRadiusIndexMax`, `EffectRealPointsPerLevel`, `EffectSpellClassMaskA`, `EffectSpellClassMaskB`, `EffectSpellClassMaskC`, `EffectTriggerSpell`, `EffectImplicitTargetA`, `EffectImplicitTargetB`, `EffectSpellId`, `EffectIndex`) VALUES
(303412, 3, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 25, 0, 71916, 0);

DELETE FROM `npc_spellclick_spells` WHERE `npc_entry` IN (38412, 39456, 44578, 44580);
INSERT INTO `npc_spellclick_spells` (`npc_entry`, `spell_id`, `cast_flags`, `user_type`) VALUES
(38412, 71916, 1, 0),
(44578, 71916, 1, 0),
(44580, 71916, 1, 0),
(39456, 73947, 1, 0);

DELETE FROM `conditions` WHERE `SourceTypeOrReferenceId` = 18 AND `SourceGroup` = 38412 AND `SourceEntry` = 71916;
DELETE FROM `conditions` WHERE `SourceTypeOrReferenceId` = 18 AND `SourceGroup` = 44578 AND `SourceEntry` = 71916;
DELETE FROM `conditions` WHERE `SourceTypeOrReferenceId` = 18 AND `SourceGroup` = 44580 AND `SourceEntry` = 71916;
DELETE FROM `conditions` WHERE `SourceTypeOrReferenceId` = 18 AND `SourceGroup` = 39456 AND `SourceEntry` = 73947;
INSERT INTO `conditions` (`SourceTypeOrReferenceId`, `SourceGroup`, `SourceEntry`, `SourceId`, `ElseGroup`, `ConditionTypeOrReference`, `ConditionTarget`, `ConditionValue1`, `ConditionValue2`, `ConditionValue3`, `NegativeCondition`, `ErrorType`, `ErrorTextId`, `ScriptName`, `Comment`) VALUES
(18, 38412, 71916, 0, 0, 9, 0, 24864, 0, 0, 0, 0, 0, '', 'Naga Hatchling - only with Irresistible Pool Pony'),
(18, 44578, 71916, 0, 0, 9, 0, 24864, 0, 0, 0, 0, 0, '', 'Naga Hatchling - only with Irresistible Pool Pony'),
(18, 44580, 71916, 0, 0, 9, 0, 24864, 0, 0, 0, 0, 0, '', 'Naga Hatchling - only with Irresistible Pool Pony'),
(18, 39456, 73947, 0, 0, 9, 0, 25214, 0, 0, 0, 0, 0, '', 'Captured Goblin - only with Escape Velocity');

DELETE FROM `smart_scripts` WHERE `source_type` = 0 AND `entryorguid` = 38412 AND `id` IN (0, 1);
DELETE FROM `smart_scripts` WHERE `source_type` = 0 AND `entryorguid` = 44578 AND `id` IN (0, 1);
DELETE FROM `smart_scripts` WHERE `source_type` = 0 AND `entryorguid` = 44580 AND `id` IN (0, 1);
DELETE FROM `smart_scripts` WHERE `source_type` = 0 AND `entryorguid` = 39456 AND `id` IN (0, 1);
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `event_param5`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES
(38412, 0, 0, 1, 73, 0, 100, 0, 0, 0, 0, 0, 0, 33, 38413, 0, 0, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0, 'Naga Hatchling - On Spellclick - Quest Credit'),
(38412, 0, 1, 0, 61, 0, 100, 0, 0, 0, 0, 0, 0, 41, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Naga Hatchling - Linked To Id 0 - Despawn'),
(44578, 0, 0, 1, 73, 0, 100, 0, 0, 0, 0, 0, 0, 33, 38413, 0, 0, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0, 'Naga Hatchling - On Spellclick - Quest Credit'),
(44578, 0, 1, 0, 61, 0, 100, 0, 0, 0, 0, 0, 0, 41, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Naga Hatchling - Linked To Id 0 - Despawn'),
(44580, 0, 0, 1, 73, 0, 100, 0, 0, 0, 0, 0, 0, 33, 38413, 0, 0, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0, 'Naga Hatchling - On Spellclick - Quest Credit'),
(44580, 0, 1, 0, 61, 0, 100, 0, 0, 0, 0, 0, 0, 41, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Naga Hatchling - Linked To Id 0 - Despawn'),
(39456, 0, 0, 1, 73, 0, 100, 0, 0, 0, 0, 0, 0, 33, 39456, 0, 0, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0, 'Captured Goblin - On Spellclick - Quest Credit'),
(39456, 0, 1, 0, 61, 0, 100, 0, 0, 0, 0, 0, 0, 41, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Captured Goblin - Linked To Id 0 - Despawn');
