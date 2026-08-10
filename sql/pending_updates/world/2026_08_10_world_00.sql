DELETE FROM `spell_script_names` WHERE `spell_id`=72940;
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES
(72940, 'spell_item_paxtons_torch');

-- Rageroar Catapult
DELETE FROM `conditions` WHERE `SourceTypeOrReferenceId`=22 AND `SourceEntry`=38818 AND `ConditionValue1`=72942;
INSERT INTO `conditions` (`SourceTypeOrReferenceId`, `SourceGroup`, `SourceEntry`, `SourceId`, `ElseGroup`, `ConditionTypeOrReference`, `ConditionTarget`, `ConditionValue1`, `ConditionValue2`, `ConditionValue3`, `NegativeCondition`, `ErrorType`, `ErrorTextId`, `ScriptName`, `Comment`) VALUES
(22, 1, 38818, 0, 0, 1, 1, 72942, 0, 0, 1, 0, 0, '', 'SAI only activates if npc has not aura Explode and Burn');

UPDATE `creature_template` SET `AIName`='SmartAI' WHERE `entry`=38818;

DELETE FROM `smart_scripts` WHERE `entryorguid`=38818;
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `event_param5`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES
(38818, 0, 0, 0, 8, 0, 100, 0, 72940, 0, 0, 0, 0, 80, 3881800, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Rageroar Catapult - On Spellhit - Run Script');

DELETE FROM `smart_scripts` WHERE `entryorguid`=3881800;
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `event_param5`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES
(3881800, 9, 0, 0, 0, 0, 100, 0, 1000, 1000, 0, 0, 0, 11, 72942, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Rageroar Catapult - On Script - Cast Spell'),
(3881800, 9, 1, 0, 0, 0, 100, 0, 1000, 1000, 0, 0, 0, 33, 38818, 0, 0, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0, 'Rageroar Catapult - On Script - Give Credit'),
(3881800, 9, 2, 0, 0, 0, 100, 0, 30000, 30000, 0, 0, 0, 28, 72942, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Rageroar Catapult - On Script - Remove Aura');

-- Rageroar Blade Thrower
DELETE FROM `conditions` WHERE `SourceTypeOrReferenceId`=22 AND `SourceEntry`=38820 AND `ConditionValue1`=72942;
INSERT INTO `conditions` (`SourceTypeOrReferenceId`, `SourceGroup`, `SourceEntry`, `SourceId`, `ElseGroup`, `ConditionTypeOrReference`, `ConditionTarget`, `ConditionValue1`, `ConditionValue2`, `ConditionValue3`, `NegativeCondition`, `ErrorType`, `ErrorTextId`, `ScriptName`, `Comment`) VALUES
(22, 1, 38820, 0, 0, 1, 1, 72942, 0, 0, 1, 0, 0, '', 'SAI only activates if npc has not aura Explode and Burn');

UPDATE `creature_template` SET `scale`=1 WHERE `entry`=38820;

UPDATE `creature_template` SET `AIName`='SmartAI' WHERE `entry`=38820;

DELETE FROM `smart_scripts` WHERE `entryorguid`=38820;
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `event_param5`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES
(38820, 0, 0, 0, 8, 0, 100, 0, 72940, 0, 0, 0, 0, 80, 3882000, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Rageroar Blade Thrower - On Spellhit - Run Script');

DELETE FROM `smart_scripts` WHERE `entryorguid`=3882000;
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `event_param5`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES
(3882000, 9, 0, 0, 0, 0, 100, 0, 1000, 1000, 0, 0, 0, 11, 72942, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Rageroar Blade Thrower - On Script - Cast Spell'),
(3882000, 9, 1, 0, 0, 0, 100, 0, 1000, 1000, 0, 0, 0, 33, 38820, 0, 0, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0, 'Rageroar Blade Thrower - On Script - Give Credit'),
(3882000, 9, 2, 0, 0, 0, 100, 0, 30000, 30000, 0, 0, 0, 28, 72942, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Rageroar Blade Thrower - On Script - Remove Aura');

-- Lifting the Siege
UPDATE `quest_template` SET `RequestItemsText`='Burn it all, $c. Burn it to cinders!' WHERE `Id`=24948;
UPDATE `quest_template` SET `OfferRewardText`='Good! Without heavy equipment, the Horde will have to pull back to regroup. We\'ve bought ourselves some time to re-secure the fortress. Well done!' WHERE `Id`=24948;
