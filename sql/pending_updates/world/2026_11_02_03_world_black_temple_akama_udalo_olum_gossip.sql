-- Black Temple gossip. Akama's and the Spirit of Olum's menus had no conditions, so every option and text showed at
-- once; TrinityCore 3.3.5's conditions show each only in its state. Olum's second "Chamber of Command" entry did
-- nothing (his script answers option 2) and the teleport to the Chamber of Command had no destination. The Spirit of
-- Udalo's "Chamber of Command" option had no script; it now casts the same teleport once the Illidari Council is dead,
-- like Olum's.
DELETE FROM `gossip_menu_option` WHERE `MenuID` = 8750 AND `OptionID` = 0;
DELETE FROM `spell_target_position` WHERE `id` = 41570 AND `effIndex` = 0;
INSERT INTO `spell_target_position` (`id`, `effIndex`, `target_map`, `target_position_x`, `target_position_y`, `target_position_z`, `target_orientation`) VALUES
(41570, 0, 564, 603.42, 305.982, 271.9, 0);
DELETE FROM `conditions` WHERE `SourceTypeOrReferenceId` IN (14, 15) AND `SourceGroup` IN (8713, 8749, 8750);
INSERT INTO `conditions` (`SourceTypeOrReferenceId`, `SourceGroup`, `SourceEntry`, `SourceId`, `ElseGroup`, `ConditionTypeOrReference`, `ConditionTarget`, `ConditionValue1`, `ConditionValue2`, `ConditionValue3`, `NegativeCondition`, `ErrorType`, `ErrorTextId`, `ScriptName`, `Comment`) VALUES
(14, 8713, 10835, 0, 0, 13, 0, 10, 2, 0, 0, 0, 0, '', 'Show Gossip if Akama is in Waiting Fight State'),
(14, 8713, 10960, 0, 0, 13, 0, 10, 1, 0, 0, 0, 0, '', 'Show Gossip if Akama is in Intro State'),
(14, 8750, 11081, 0, 0, 13, 1, 1, 3, 2, 0, 0, 0, '', 'Show gossip text only if SUPREMUS done'),
(14, 8750, 11082, 0, 0, 13, 1, 1, 3, 2, 1, 0, 0, '', 'Show gossip text only if SUPREMUS not done'),
(15, 8713, 0, 0, 0, 13, 0, 10, 1, 0, 0, 0, 0, '', 'Show Option if Akama in Intro State'),
(15, 8713, 1, 0, 0, 13, 0, 10, 2, 0, 0, 0, 0, '', 'Show Option if Akama is in Waiting Fight State'),
(15, 8749, 0, 0, 0, 13, 1, 7, 3, 2, 0, 0, 0, '', 'Show options for gossip only if COUNCIL done'),
(15, 8750, 1, 0, 0, 13, 1, 1, 3, 2, 0, 0, 0, '', 'Show options for gossip only if SUPREMUS done'),
(15, 8750, 2, 0, 0, 13, 1, 7, 3, 2, 0, 0, 0, '', 'Show options for gossip only if COUNCIL done');
UPDATE `creature_template` SET `AIName` = 'SmartAI' WHERE `entry` = 23410;
DELETE FROM `smart_scripts` WHERE `source_type` = 0 AND `entryorguid` = 23410;
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `event_param5`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES
(23410, 0, 0, 1, 62, 0, 100, 0, 8749, 0, 0, 0, 0, 85, 41570, 0, 0, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0, 'Spirit of Udalo - On gossip select - cast spell'),
(23410, 0, 1, 0, 61, 0, 100, 0, 0, 0, 0, 0, 0, 72, 0, 0, 0, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0, 'Spirit of Udalo - On gossip select - Close gossip');
