-- Gilneas: Save the Children! (14368). Clicking Cynthia, Ashley or James casts the rescue spell on them; nothing gave the
-- quest credit, so the quest could not be completed and the children did not run for the basement. SmartAI version of
-- the TrinityCore script: credit, thanks, run to the basement (points and timings of the original) and despawn.
-- Ported from the Cataclysm Preservation Project 4.3.4, texts and spells from Legends of Azeroth 5.4.8.

UPDATE `creature_template` SET `AIName`='SmartAI' WHERE `entry` IN (36287, 36288, 36289);
DELETE FROM `smart_scripts` WHERE (`source_type`=0 AND `entryorguid` IN (36287, 36288, 36289)) OR (`source_type`=9 AND `entryorguid` IN (3628700, 3628800, 3628900));
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `event_param5`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES
(36287, 0, 0, 1, 8, 0, 100, 0, 68597, 0, 0, 0, 0, 18, 33554432, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Cynthia - On Spell Hit (Save the Children) - Make Unselectable'),
(36287, 0, 1, 2, 61, 0, 100, 0, 0, 0, 0, 0, 0, 33, 36287, 0, 0, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0, 'Cynthia - Linked - Give Quest Credit'),
(36287, 0, 2, 0, 61, 0, 100, 0, 0, 0, 0, 0, 0, 80, 3628700, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Cynthia - Linked - Run Script'),
(36287, 0, 3, 0, 1, 0, 100, 0, 1000, 1500, 1000, 1500, 0, 5, 18, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Cynthia - Out Of Combat - Cry'),
(36287, 0, 4, 0, 34, 0, 100, 0, 8, 1, 0, 0, 0, 69, 2, 0, 0, 0, 0, 0, 8, 0, 0, 0, -1947.472, 2515.521, 2.318746, 0, 'Cynthia - On Point 1 Reached - Move To Next Point'),
(36287, 0, 5, 0, 34, 0, 100, 0, 8, 2, 0, 0, 0, 69, 3, 0, 0, 0, 0, 0, 8, 0, 0, 0, -1926.536, 2519.312, 2.246772, 0, 'Cynthia - On Point 2 Reached - Move To Next Point'),
(3628700, 9, 0, 0, 0, 0, 100, 0, 2500, 2500, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0, 'Cynthia - Say Thanks'),
(3628700, 9, 1, 0, 0, 0, 100, 0, 4800, 4800, 0, 0, 0, 69, 1, 0, 0, 0, 0, 0, 8, 0, 0, 0, -1969.23, 2517.465, 2.580818, 0, 'Cynthia - Run To The Basement'),
(3628700, 9, 2, 0, 0, 0, 100, 0, 13300, 13300, 0, 0, 0, 41, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Cynthia - Despawn'),
(36288, 0, 0, 1, 8, 0, 100, 0, 68598, 0, 0, 0, 0, 18, 33554432, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Ashley - On Spell Hit (Save the Children) - Make Unselectable'),
(36288, 0, 1, 2, 61, 0, 100, 0, 0, 0, 0, 0, 0, 33, 36288, 0, 0, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0, 'Ashley - Linked - Give Quest Credit'),
(36288, 0, 2, 0, 61, 0, 100, 0, 0, 0, 0, 0, 0, 80, 3628800, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Ashley - Linked - Run Script'),
(36288, 0, 3, 0, 34, 0, 100, 0, 8, 1, 0, 0, 0, 69, 2, 0, 0, 0, 0, 0, 8, 0, 0, 0, -1920.023, 2558.055, 7.076692, 0, 'Ashley - On Point 1 Reached - Move To Next Point'),
(3628800, 9, 0, 0, 0, 0, 100, 0, 2500, 2500, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0, 'Ashley - Say Thanks'),
(3628800, 9, 1, 0, 0, 0, 100, 0, 4800, 4800, 0, 0, 0, 69, 1, 0, 0, 0, 0, 0, 8, 0, 0, 0, -1923.283, 2552.308, 12.73581, 0, 'Ashley - Run To The Basement'),
(3628800, 9, 2, 0, 0, 0, 100, 0, 8600, 8600, 0, 0, 0, 41, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Ashley - Despawn'),
(36289, 0, 0, 1, 8, 0, 100, 0, 68596, 0, 0, 0, 0, 18, 33554432, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'James - On Spell Hit (Save the Children) - Make Unselectable'),
(36289, 0, 1, 2, 61, 0, 100, 0, 0, 0, 0, 0, 0, 33, 36289, 0, 0, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0, 'James - Linked - Give Quest Credit'),
(36289, 0, 2, 0, 61, 0, 100, 0, 0, 0, 0, 0, 0, 80, 3628900, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'James - Linked - Run Script'),
(3628900, 9, 0, 0, 0, 0, 100, 0, 2500, 2500, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0, 'James - Say Thanks'),
(3628900, 9, 1, 0, 0, 0, 100, 0, 6100, 6100, 0, 0, 0, 69, 0, 0, 0, 0, 0, 0, 8, 0, 0, 0, -1913.021, 2558.333, 1.511007, 0, 'James - Run To The Basement'),
(3628900, 9, 2, 0, 0, 0, 100, 0, 11300, 11300, 0, 0, 0, 41, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'James - Despawn');

DELETE FROM `creature_text` WHERE `entry` IN (36287, 36288, 36289);
INSERT INTO `creature_text` (`entry`, `groupid`, `id`, `text`, `type`, `language`, `probability`, `emote`, `duration`, `sound`, `comment`) VALUES
(36287, 0, 0, 'You are scary!  I just want my mommy!', 12, 0, 100, 0, 0, 0, 'Cynthia - rescued'),
(36288, 0, 0, 'Are you one of the good worgen, $g mister:ma''am;?  Did you see Cynthia hiding in the sheds outside?', 12, 0, 100, 0, 0, 0, 'Ashley - rescued'),
(36289, 0, 0, 'Don''t hurt me!  I was just looking for my sisters!  I think Ashley''s inside that house!', 12, 0, 100, 0, 0, 0, 'James - rescued');
