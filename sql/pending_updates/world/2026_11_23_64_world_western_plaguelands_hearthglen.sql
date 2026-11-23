-- The Hearthglen quests Good People of Hearthglen (27153), Bagging Bisp (27154) and It's About Time! (27156) cannot
-- be finished. The four townsfolk never gave their talk credits, Magus Bisp did not turn hostile when asked about the
-- gnolls, and the Hearthglen Trainees had no script and no speech for the duel. The gossip options were always shown;
-- they now need the quest, as in the source.
-- The townsfolk keep their reply text (the options open the sniffed follow up menus), so only the credit rows of
-- Legends of Azeroth 5.4.8 are used for them, and the Trainee duel rows are as in the source with its combat stop
-- action (203) written as this core's action 27. The speech is as in the source; Bisp's own gossip speech line has no
-- text in the source and is left out.

UPDATE `creature_template` SET `AIName` = 'SmartAI' WHERE `entry` IN (45148, 45149, 45150, 45151, 45162) AND `ScriptName` = '';

DELETE FROM `smart_scripts` WHERE `source_type` = 0 AND (`entryorguid` IN (45148, 45149, 45150, 45151, 45162) OR (`entryorguid` = 45152 AND `id` IN (3, 4)));
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `event_param5`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES
(45148, 0, 0, 0, 62, 0, 100, 0, 11934, 0, 0, 0, 0, 33, 45148, 0, 0, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0, 'Morris Vant - On Gossip Select - Quest Credit'),
(45149, 0, 0, 0, 62, 0, 100, 0, 11936, 0, 0, 0, 0, 33, 45149, 0, 0, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0, 'Outfitter Mendelev - On Gossip Select - Quest Credit'),
(45150, 0, 0, 0, 62, 0, 100, 0, 11938, 0, 0, 0, 0, 33, 45150, 0, 0, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0, 'Bree Ironstock - On Gossip Select - Quest Credit'),
(45151, 0, 0, 0, 62, 0, 100, 0, 11940, 0, 0, 0, 0, 33, 45151, 0, 0, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0, 'High Cleric Alphus - On Gossip Select - Quest Credit'),
(45152, 0, 3, 4, 62, 0, 100, 0, 11942, 0, 0, 0, 0, 2, 14, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Magus Bisp - On Gossip Select - Set Faction Hostile'),
(45152, 0, 4, 0, 61, 0, 100, 0, 0, 0, 0, 0, 0, 72, 0, 0, 0, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0, 'Magus Bisp - Linked To Id 3 - Close Gossip'),
(45162, 0, 0, 1, 11, 0, 100, 0, 0, 0, 0, 0, 0, 8, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Hearthglen Trainee - On Respawn - Set React State Defensive'),
(45162, 0, 1, 0, 61, 0, 100, 0, 0, 0, 0, 0, 0, 18, 32784, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Hearthglen Trainee - Linked To Id 0 - Set UnitFlag 32784'),
(45162, 0, 2, 3, 62, 0, 100, 0, 11943, 0, 0, 0, 0, 64, 1, 0, 0, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0, 'Hearthglen Trainee - On Gossip Select - Store Target List'),
(45162, 0, 3, 4, 61, 0, 100, 0, 0, 0, 0, 0, 0, 72, 0, 0, 0, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0, 'Hearthglen Trainee - Linked To Id 2 - Close Gossip'),
(45162, 0, 4, 5, 61, 0, 100, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Hearthglen Trainee - Linked To Id 3 - Say Line 0'),
(45162, 0, 5, 6, 61, 0, 100, 0, 0, 0, 0, 0, 0, 2, 14, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Hearthglen Trainee - Linked To Id 4 - Set Faction 14'),
(45162, 0, 6, 7, 61, 0, 100, 0, 0, 0, 0, 0, 0, 49, 0, 0, 0, 0, 0, 0, 12, 1, 0, 0, 0, 0, 0, 0, 'Hearthglen Trainee - Linked To Id 5 - Attack Start'),
(45162, 0, 7, 0, 61, 0, 100, 0, 0, 0, 0, 0, 0, 42, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Hearthglen Trainee - Linked To Id 6 - Set Invincibility Hp 1'),
(45162, 0, 8, 9, 2, 0, 100, 1, 0, 0, 0, 0, 0, 27, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Hearthglen Trainee - On Between 0-1% - Combat Stop'),
(45162, 0, 9, 10, 61, 0, 100, 0, 0, 0, 0, 0, 0, 18, 256, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Hearthglen Trainee - Set UnitFlag 256'),
(45162, 0, 10, 11, 61, 0, 100, 0, 0, 0, 0, 0, 0, 33, 45162, 0, 0, 0, 0, 0, 12, 1, 0, 0, 0, 0, 0, 0, 'Hearthglen Trainee - Linked To Id 9 - Quest Credit'),
(45162, 0, 11, 12, 61, 0, 100, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Hearthglen Trainee - Linked To Id 10 - Say Line 1'),
(45162, 0, 12, 0, 61, 0, 100, 0, 0, 0, 0, 0, 0, 41, 5000, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Hearthglen Trainee - Linked To Id 11 - Despawn after 5 sec');

DELETE FROM `conditions` WHERE `SourceTypeOrReferenceId` = 15 AND `SourceGroup` IN (11934, 11936, 11938, 11940, 11942, 11943) AND `SourceEntry` = 0;
INSERT INTO `conditions` (`SourceTypeOrReferenceId`, `SourceGroup`, `SourceEntry`, `SourceId`, `ElseGroup`, `ConditionTypeOrReference`, `ConditionTarget`, `ConditionValue1`, `ConditionValue2`, `ConditionValue3`, `NegativeCondition`, `ErrorType`, `ErrorTextId`, `ScriptName`, `Comment`) VALUES
(15, 11934, 0, 0, 0, 9, 0, 27153, 0, 0, 0, 0, 0, '', 'Show option if quest 27153 is taken'),
(15, 11936, 0, 0, 0, 9, 0, 27153, 0, 0, 0, 0, 0, '', 'Show option if quest 27153 is taken'),
(15, 11938, 0, 0, 0, 9, 0, 27153, 0, 0, 0, 0, 0, '', 'Show option if quest 27153 is taken'),
(15, 11940, 0, 0, 0, 9, 0, 27153, 0, 0, 0, 0, 0, '', 'Show option if quest 27153 is taken'),
(15, 11942, 0, 0, 0, 9, 0, 27154, 0, 0, 0, 0, 0, '', 'Show option if quest 27154 is taken'),
(15, 11943, 0, 0, 0, 9, 0, 27156, 0, 0, 0, 0, 0, '', 'Show option if quest 27156 is taken');

DELETE FROM `creature_text` WHERE `entry` = 45162;
INSERT INTO `creature_text` (`entry`, `groupid`, `id`, `text`, `type`, `language`, `probability`, `emote`, `duration`, `sound`, `comment`) VALUES
(45162, 0, 0, 'Fine.  You want some of this?', 12, 0, 100, 0, 0, 0, ''),
(45162, 0, 1, 'The time for the Crusade is now!', 12, 0, 100, 0, 0, 0, ''),
(45162, 0, 2, 'Fine... but I won''t hold back!', 12, 0, 100, 0, 0, 0, ''),
(45162, 0, 3, 'Get ready, ''cause here I come!', 12, 0, 100, 0, 0, 0, ''),
(45162, 1, 0, 'Well fought.', 12, 0, 100, 0, 0, 0, ''),
(45162, 1, 1, 'I suppose I need some more practice.', 12, 0, 100, 0, 0, 0, ''),
(45162, 1, 2, 'Mom was right... I''ll NEVER make it into the Argent Crusade.', 12, 0, 100, 0, 0, 0, ''),
(45162, 1, 3, 'You win.  This time.', 12, 0, 100, 0, 0, 0, '');
