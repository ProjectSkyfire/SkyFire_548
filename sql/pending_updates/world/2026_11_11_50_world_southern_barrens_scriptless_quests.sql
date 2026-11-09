-- Claim the Battle Scar (24618) and Claim the Battlescar (25081) had no way to finish the "scout the flag" objective.
-- The two area triggers cover the flag in the middle of the Battlescar, one for each faction's quest.
DELETE FROM `areatrigger_involvedrelation` WHERE `id` IN (5667, 5784);
INSERT INTO `areatrigger_involvedrelation` (`id`, `quest`) VALUES
(5667, 24618),
(5784, 25081);

-- Clap 'Em In Irons (25057): the Northwatch Manacles spell (73387) only has a dummy effect, so nothing gave the
-- "Taurajo Looters Arrested" credit. It can be used on a wounded Taurajo Looter, which is credited and removed.
DELETE FROM `conditions` WHERE `SourceTypeOrReferenceId` = 17 AND `SourceEntry` = 73387;
INSERT INTO `conditions` (`SourceTypeOrReferenceId`, `SourceGroup`, `SourceEntry`, `SourceId`, `ElseGroup`, `ConditionTypeOrReference`, `ConditionTarget`, `ConditionValue1`, `ConditionValue2`, `ConditionValue3`, `NegativeCondition`, `ErrorType`, `ErrorTextId`, `ScriptName`, `Comment`) VALUES
(17, 0, 73387, 0, 0, 31, 1, 3, 37743, 0, 0, 0, 0, '', 'Northwatch Manacles can target only Taurajo Looters'),
(17, 0, 73387, 0, 0, 38, 1, 30, 4, 0, 0, 0, 0, '', 'Northwatch Manacles can target only a wounded looter (health 30% or less)');

DELETE FROM `smart_scripts` WHERE `source_type` = 0 AND `entryorguid` = 37743 AND `id` IN (4, 5);
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `event_param5`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES
(37743, 0, 4, 5, 8, 0, 100, 0, 73387, 0, 0, 0, 0, 33, 39050, 0, 0, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0, 'Taurajo Looter - On Spellhit Clap ''em in Irons - Quest Credit Arrested Taurajo Looter'),
(37743, 0, 5, 0, 61, 0, 100, 0, 0, 0, 0, 0, 0, 41, 1000, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Taurajo Looter - Linked - Despawn');

-- Bael'dun Rescue (25153): the survivors in the keep had no way to be rescued. Clicking one while the quest is
-- active rescues it, like the Orsis Survivors in Uldum.
UPDATE `creature_template` SET `AIName` = 'SmartAI' WHERE `entry` = 39218;

DELETE FROM `npc_spellclick_spells` WHERE `npc_entry` = 39218;
INSERT INTO `npc_spellclick_spells` (`npc_entry`, `spell_id`, `cast_flags`, `user_type`) VALUES
(39218, 85372, 1, 0);

DELETE FROM `conditions` WHERE `SourceTypeOrReferenceId` = 18 AND `SourceGroup` = 39218 AND `SourceEntry` = 85372;
INSERT INTO `conditions` (`SourceTypeOrReferenceId`, `SourceGroup`, `SourceEntry`, `SourceId`, `ElseGroup`, `ConditionTypeOrReference`, `ConditionTarget`, `ConditionValue1`, `ConditionValue2`, `ConditionValue3`, `NegativeCondition`, `ErrorType`, `ErrorTextId`, `ScriptName`, `Comment`) VALUES
(18, 39218, 85372, 0, 0, 9, 0, 25153, 0, 0, 0, 0, 0, '', 'Spellclick allowed only with Bael''dun Rescue taken');

DELETE FROM `smart_scripts` WHERE `source_type` = 0 AND `entryorguid` = 39218;
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `event_param5`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES
(39218, 0, 0, 1, 73, 0, 100, 0, 0, 0, 0, 0, 0, 33, 39219, 0, 0, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0, 'Bael''dun Survivor - On Spellclick - Quest Credit Bael''dun Survivor Rescue Credit'),
(39218, 0, 1, 2, 61, 0, 100, 0, 0, 0, 0, 0, 0, 83, 16777216, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Bael''dun Survivor - Linked - Remove Spellclick Flag'),
(39218, 0, 2, 0, 61, 0, 100, 0, 0, 0, 0, 0, 0, 41, 5000, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Bael''dun Survivor - Linked - Despawn');
