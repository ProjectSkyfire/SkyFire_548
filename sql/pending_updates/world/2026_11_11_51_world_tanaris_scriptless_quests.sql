-- Rocket Rescue (24910, 25050): nothing gave the "Life Rockets Delivered" credit. Clicking a Steamwheedle Survivor
-- while either quest is active rescues it: the survivor is credited, blasts off with its rocket pack and is removed.
-- The balloon ride is not implemented, the Southsea Blockaders are killed on foot.
UPDATE `creature_template` SET `AIName` = 'SmartAI' WHERE `entry` = 38571;

DELETE FROM `npc_spellclick_spells` WHERE `npc_entry` = 38571;
INSERT INTO `npc_spellclick_spells` (`npc_entry`, `spell_id`, `cast_flags`, `user_type`) VALUES
(38571, 85372, 1, 0);

DELETE FROM `conditions` WHERE `SourceTypeOrReferenceId` = 18 AND `SourceGroup` = 38571 AND `SourceEntry` = 85372;
INSERT INTO `conditions` (`SourceTypeOrReferenceId`, `SourceGroup`, `SourceEntry`, `SourceId`, `ElseGroup`, `ConditionTypeOrReference`, `ConditionTarget`, `ConditionValue1`, `ConditionValue2`, `ConditionValue3`, `NegativeCondition`, `ErrorType`, `ErrorTextId`, `ScriptName`, `Comment`) VALUES
(18, 38571, 85372, 0, 0, 9, 0, 24910, 0, 0, 0, 0, 0, '', 'Spellclick allowed only with Rocket Rescue taken (Horde)'),
(18, 38571, 85372, 0, 1, 9, 0, 25050, 0, 0, 0, 0, 0, '', 'Spellclick allowed only with Rocket Rescue taken (Alliance)');

DELETE FROM `smart_scripts` WHERE `source_type` = 0 AND `entryorguid` = 38571;
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `event_param5`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES
(38571, 0, 0, 1, 73, 0, 100, 0, 0, 0, 0, 0, 0, 33, 38576, 0, 0, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0, 'Steamwheedle Survivor - On Spellclick - Quest Credit Steamwheedle Survivor Credit Bunny'),
(38571, 0, 1, 2, 61, 0, 100, 0, 0, 0, 0, 0, 0, 83, 16777216, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Steamwheedle Survivor - Linked - Remove Spellclick Flag'),
(38571, 0, 2, 3, 61, 0, 100, 0, 0, 0, 0, 0, 0, 11, 75710, 2, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Steamwheedle Survivor - Linked - Cast Rocket Rescue Pack Blastoff'),
(38571, 0, 3, 0, 61, 0, 100, 0, 0, 0, 0, 0, 0, 41, 3000, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Steamwheedle Survivor - Linked - Despawn');
