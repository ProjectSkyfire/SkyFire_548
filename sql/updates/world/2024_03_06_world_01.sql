DELETE FROM `conditions` WHERE `SourceTypeOrReferenceId`=17 AND `SourceEntry`=69855;
INSERT INTO `conditions` (`SourceTypeOrReferenceId`, `SourceGroup`, `SourceEntry`, `SourceId`, `ElseGroup`, `ConditionTypeOrReference`, `ConditionTarget`, `ConditionValue1`, `ConditionValue2`, `ConditionValue3`, `NegativeCondition`, `ErrorType`, `ErrorTextId`, `ScriptName`, `Comment`) VALUES
(17, 0, 69855, 0, 0, 31, 1, 3, 37080, 0, 0, 0, 0, '', 'Spell only hits Wounded Coldridge Mountaineer');

UPDATE `creature_template` SET `AIName`='SmartAI' WHERE `entry`=37080;

DELETE FROM `smart_scripts` WHERE `entryorguid` IN (37080, 3708000);
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `event_param5`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES
(37080, 0, 1, 0, 11, 0, 100, 0, 1000, 1000, 1000, 1000, 0, 5, 16, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Wounded Coldridge Mountaineer - On Respawn - Set Emote State (EMOTE_ONESHOT_KNEEL)'),
(37080, 0, 2, 3, 8, 0, 100, 1, 69855, 0, 0, 0, 0, 5, 26, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Wounded Coldridge Mountaineer - On Spell Hit "Heal Wounded Mountaineer" - Set Emote State (EMOTE_STATE_STAND)'),
(37080, 0, 3, 0, 61, 0, 100, 0, 0, 1, 0, 0, 0, 80, 3708000, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Wounded Coldridge Mountaineer - On Linked - Run Script'),
(3708000, 9, 1, 0, 0, 0, 100, 0, 2000, 2000, 2000, 2000, 0, 1, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Wounded Coldridge Mountaineer - Script - say random text'),
(3708000, 9, 2, 0, 0, 0, 100, 0, 0, 0, 0, 0, 0, 59, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Wounded Coldridge Mountaineer - Script - set run'),
(3708000, 9, 3, 0, 0, 0, 100, 0, 2000, 2000, 2000, 2000, 0, 46, 5, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Wounded Coldridge Mountaineer - Script - move 5 yards'),
(3708000, 9, 4, 0, 0, 0, 100, 0, 0, 0, 0, 0, 0, 41, 2000, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Wounded Coldridge Mountaineer - Script - Despawn after 2 seconds');

DELETE FROM `creature_text` WHERE `entry`=37080;
INSERT INTO `creature_text` (`entry`, `groupid`, `id`, `text`, `type`, `language`, `probability`, `emote`, `duration`, `sound`, `comment`) VALUES
(37080, 0, 0, 'Thanks! I can make it from here.', 12, 0, 100, 0, 0, 0, 'Wounded Coldridge Mountaineer - RANDOM_SAY_0'),
(37080, 0, 2, 'Yer a lifesaver! for Ironforge!', 12, 0, 100, 0, 0, 0, 'Wounded Coldridge Mountaineer - RANDOM_SAY_2'),
(37080, 0, 1, 'Ahh, much better. I owe ya one.', 12, 0, 100, 0, 0, 0, 'Wounded Coldridge Mountaineer - RANDOM_SAY_1');