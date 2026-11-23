-- A Slippery Snack (29325) asks for 6 baby octopuses. Clicking a Baby Octopus (53522, 36 spawns) with the quest taken
-- casts spell 99339. The spell itself does nothing; the script of the octopus reacts to it by casting spell 99340
-- (Pickup Baby Octopus) on the player, which creates the Baby Octopus item (69915), and removes the octopus. The octopus
-- had no click flag, no click row, no condition and no script. As in Legends of Azeroth 5.4.8.

UPDATE `creature_template` SET `npcflag` = `npcflag` | 16777216 WHERE `entry` IN (53522);
UPDATE `creature_template` SET `AIName` = 'SmartAI' WHERE `entry` IN (53522) AND `AIName` = '' AND `ScriptName` = '';

DELETE FROM `npc_spellclick_spells` WHERE `npc_entry` IN (53522);
INSERT INTO `npc_spellclick_spells` (`npc_entry`, `spell_id`, `cast_flags`, `user_type`) VALUES
(53522, 99339, 1, 0);

DELETE FROM `conditions` WHERE `SourceTypeOrReferenceId` = 18 AND `SourceGroup` = 53522 AND `SourceEntry` = 99339;
INSERT INTO `conditions` (`SourceTypeOrReferenceId`, `SourceGroup`, `SourceEntry`, `SourceId`, `ElseGroup`, `ConditionTypeOrReference`, `ConditionTarget`, `ConditionValue1`, `ConditionValue2`, `ConditionValue3`, `NegativeCondition`, `ErrorType`, `ErrorTextId`, `ScriptName`, `Comment`) VALUES
(18, 53522, 99339, 0, 0, 9, 0, 29325, 0, 0, 0, 0, 0, '', 'Baby Octopus - only with A Slippery Snack');

DELETE FROM `smart_scripts` WHERE `source_type` = 0 AND `entryorguid` = 53522 AND `id` IN (0, 1);
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `event_param5`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES
(53522, 0, 0, 1, 8, 0, 100, 0, 99339, 0, 0, 0, 0, 11, 99340, 2, 0, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0, 'Baby Octopus - On Spellhit Pickup Baby Octopus - Cast Pickup Baby Octopus'),
(53522, 0, 1, 0, 61, 0, 100, 0, 0, 0, 0, 0, 0, 41, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Baby Octopus - Linked To Id 0 - Despawn');
