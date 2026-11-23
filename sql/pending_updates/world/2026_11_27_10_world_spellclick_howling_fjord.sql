-- A Carver and a Croaker (11476) asks for a Scalawag Frog (item 35803). The Scalawag Frogs (26503, 11 spawns) can be
-- clicked while the quest is taken; the script gives the item and the frog goes away. The frogs had no click flag, no
-- click row, no condition and no script. Spell 47121, the click spell, is not in the 5.4.8 client data, so the click
-- row was dropped when the table loaded. It is defined here as a server-side spell with one dummy effect, copying the
-- attributes and range of spell 73947, the click spell of the Lost Isles quest Escape Velocity. The item cannot be made
-- by the spell itself: the server-side spell tables have no item type column. The quest condition is not in Legends of
-- Azeroth, it is as in TrinityCore 3.3.5.

UPDATE `creature_template` SET `npcflag` = `npcflag` | 16777216 WHERE `entry` IN (26503);
UPDATE `creature_template` SET `AIName` = 'SmartAI' WHERE `entry` IN (26503) AND `AIName` = '' AND `ScriptName` = '';

DELETE FROM `spell_dbc` WHERE `Id` = 47121;
DELETE FROM `spellmisc_dbc` WHERE `Id` = 247121;
DELETE FROM `spelleffect_dbc` WHERE `EffectSpellId` = 47121;
INSERT INTO `spell_dbc` (`Id`, `SpellAuraOptionsId`, `SpellCastingRequirementsId`, `SpellCategoriesId`, `SpellClassOptionsId`, `SpellEquippedItemsId`, `SpellInterruptsId`, `SpellLevelsId`, `SpellTargetRestrictionsId`, `SpellMiscId`, `Comment`) VALUES
(47121, 0, 0, 0, 0, 0, 0, 0, 0, 247121, 'Scalawag Frog click');
INSERT INTO `spellmisc_dbc` (`Id`, `Attributes`, `AttributesEx`, `AttributesEx2`, `AttributesEx3`, `AttributesEx4`, `AttributesEx5`, `AttributesEx6`, `AttributesEx7`, `AttributesEx8`, `AttributesEx9`, `CastingTimeIndex`, `DurationIndex`, `RangeIndex`, `SchoolMask`, `Comment`) VALUES
(247121, 538968320, 1160, 67108868, 64, 2049, 0, 0, 0, 0, 0, 1, 0, 7, 1, 'Scalawag Frog click');
INSERT INTO `spelleffect_dbc` (`Id`, `Effect`, `EffectValueMultiplier`, `EffectApplyAuraName`, `EffectAplitude`, `EffectBasePoints`, `EffectBonusMultiplier`, `EffectDamageMultiplier`, `EffectChainTarget`, `EffectDieSides`, `EffectMechanic`, `EffectMiscValue`, `EffectMiscValueB`, `EffectRadiusIndex`, `EffectRadiusIndexMax`, `EffectRealPointsPerLevel`, `EffectSpellClassMaskA`, `EffectSpellClassMaskB`, `EffectSpellClassMaskC`, `EffectTriggerSpell`, `EffectImplicitTargetA`, `EffectImplicitTargetB`, `EffectSpellId`, `EffectIndex`) VALUES
(303411, 3, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 25, 0, 47121, 0);

DELETE FROM `npc_spellclick_spells` WHERE `npc_entry` IN (26503);
INSERT INTO `npc_spellclick_spells` (`npc_entry`, `spell_id`, `cast_flags`, `user_type`) VALUES
(26503, 47121, 0, 0);

DELETE FROM `conditions` WHERE `SourceTypeOrReferenceId` = 18 AND `SourceGroup` = 26503 AND `SourceEntry` = 47121;
INSERT INTO `conditions` (`SourceTypeOrReferenceId`, `SourceGroup`, `SourceEntry`, `SourceId`, `ElseGroup`, `ConditionTypeOrReference`, `ConditionTarget`, `ConditionValue1`, `ConditionValue2`, `ConditionValue3`, `NegativeCondition`, `ErrorType`, `ErrorTextId`, `ScriptName`, `Comment`) VALUES
(18, 26503, 47121, 0, 0, 9, 0, 11476, 0, 0, 0, 0, 0, '', 'Scalawag Frog - only with A Carver and a Croaker');

DELETE FROM `smart_scripts` WHERE `source_type` = 0 AND `entryorguid` = 26503 AND `id` IN (0, 1);
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `event_param5`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES
(26503, 0, 0, 1, 73, 0, 100, 0, 47121, 0, 0, 0, 0, 56, 35803, 1, 0, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0, 'Scalawag Frog - On Spellclick - Add Item Scalawag Frog'),
(26503, 0, 1, 0, 61, 0, 100, 0, 0, 0, 0, 0, 0, 41, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Scalawag Frog - Linked To Id 0 - Despawn');
