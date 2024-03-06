UPDATE `creature_template` SET `AIName`='SmartAI' WHERE `entry`=33266;

DELETE FROM `smart_scripts` WHERE `entryorguid`=33266;
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `event_param5`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES
(33266, 0, 1, 2, 8, 0, 100, 1, 62644, 0, 0, 0, 0, 17, 26, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Wounded Mor\'shan Defender - On Spell Hit "Apply Salve" - Set Emote State (EMOTE_CONGRATULATE)'),
(33266, 0, 2, 3, 61, 0, 100, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Wounded Mor\'shan Defender - linked - say random text'),
(33266, 0, 3, 4, 61, 0, 100, 0, 0, 0, 0, 0, 0, 33, 33266, 0, 0, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0, 'Wounded Mor\'shan Defender - linked - Give Killcredit (33266)'),
(33266, 0, 4, 5, 61, 0, 100, 0, 0, 0, 0, 0, 0, 59, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Wounded Mor\'shan Defender - linked - set run'),
(33266, 0, 5, 6, 61, 0, 100, 0, 0, 0, 0, 0, 0, 69, 1, 0, 0, 0, 0, 0, 8, 0, 0, 0, 1217.22, -2246.04, 92.172, 0, 'Wounded Mor\'shan Defender - linked - move position'),
(33266, 0, 6, 0, 61, 0, 100, 0, 0, 0, 0, 0, 0, 41, 3000, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Wounded Mor\'shan Defender - linked - Despawn after 3 seconds');

DELETE FROM `creature_text` WHERE `entry`=33266;
INSERT INTO `creature_text` (`entry`, `groupid`, `id`, `text`, `type`, `language`, `probability`, `emote`, `duration`, `sound`, `comment`) VALUES
(33266, 0, 0, 'Death to the Alliance!', 12, 0, 100, 0, 0, 0, 'Wounded Mor\'shan Defender - RANDOM_SAY_0'),
(33266, 0, 2, 'For the Horde!', 12, 0, 100, 0, 0, 0, 'Wounded Mor\'shan Defender - RANDOM_SAY_2'),
(33266, 0, 1, 'Many thanks, $c!', 12, 0, 100, 0, 0, 0, 'Wounded Mor\'shan Defender - RANDOM_SAY_1');
