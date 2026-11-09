-- The Winterspring Cub quests (29035, 29052, 29037, 29053, 29040, 29051) had no credit source. Feeding the cub with
-- Shardtooth Meat (95968) or Smoked Meat (95994) now reaches it as a spell target and credits the matching feeding
-- bunny. The Wintersaber Cat Toy (96059) summons a toy and the cub plays with it, which credits the toy bunny.
UPDATE `creature_template` SET `AIName` = 'SmartAI' WHERE `entry` IN (51677, 51749);

DELETE FROM `conditions` WHERE `SourceTypeOrReferenceId` = 13 AND `SourceEntry` IN (95967, 95968, 95994);
INSERT INTO `conditions` (`SourceTypeOrReferenceId`, `SourceGroup`, `SourceEntry`, `SourceId`, `ElseGroup`, `ConditionTypeOrReference`, `ConditionTarget`, `ConditionValue1`, `ConditionValue2`, `ConditionValue3`, `NegativeCondition`, `ErrorType`, `ErrorTextId`, `ScriptName`, `Comment`) VALUES
(13, 1, 95967, 0, 0, 31, 0, 3, 51677, 0, 0, 0, 0, '', 'Feed Meat Visual targets the Winterspring Cub'),
(13, 1, 95968, 0, 0, 31, 0, 3, 51677, 0, 0, 0, 0, '', 'Feed Wintersaber Cub (Shardtooth Meat) targets the Winterspring Cub'),
(13, 1, 95994, 0, 0, 31, 0, 3, 51677, 0, 0, 0, 0, '', 'Feed Wintersaber Cub (Smoked Meat) targets the Winterspring Cub');

DELETE FROM `smart_scripts` WHERE `source_type` = 0 AND `entryorguid` IN (51677, 51749);
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `event_param5`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES
(51677, 0, 0, 0, 8, 0, 100, 0, 95968, 0, 0, 0, 0, 33, 51694, 0, 0, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0, 'Winterspring Cub - On Spellhit Feed Wintersaber Cub (Shardtooth Meat) - Quest Credit Shardtooth Meat Feeding Credit'),
(51677, 0, 1, 0, 8, 0, 100, 0, 95994, 0, 0, 0, 0, 33, 51700, 0, 0, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0, 'Winterspring Cub - On Spellhit Feed Wintersaber Cub (Smoked Meat) - Quest Credit Smoked Meat Feeding Credit'),
(51749, 0, 0, 0, 54, 0, 100, 0, 0, 0, 0, 0, 0, 33, 51763, 0, 0, 0, 0, 0, 23, 0, 0, 0, 0, 0, 0, 0, 'Wintersaber Cat Toy - On Just Summoned - Quest Credit Wintersaber Toy Credit to Summoner');
