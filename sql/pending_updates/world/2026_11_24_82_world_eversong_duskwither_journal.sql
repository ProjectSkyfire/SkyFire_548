-- Abandoned Investigations (8891): handing the journal in to Magister Duskwither (15951) should make him say his line,
-- cast his fireball and spawn Magister Duskwither's Journal (181012) for 4 seconds where it burns. He had no
-- script and no line of text. The rows and the line are those of Legends of Azeroth 5.4.8.

UPDATE `creature_template` SET `AIName` = 'SmartAI' WHERE `entry` = 15951 AND `ScriptName` = '';

DELETE FROM `smart_scripts` WHERE `entryorguid` IN (15951) AND `source_type` = 0;
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `event_param5`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES
(15951, 0, 0, 1, 20, 0, 100, 0, 8891, 0, 0, 0, 0, 50, 181012, 4, 0, 0, 0, 0, 8, 0, 0, 0, 9049.71, -7434.27, 84.6563, 2.09439, 'Magister Duskwither - On Quest Reward (Abandoned Investigations)  - Spawn Magister Duskwithers Journal'),
(15951, 0, 1, 2, 61, 0, 100, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Magister Duskwither - Linked with Previous Event  - Say Line 1'),
(15951, 0, 2, 0, 61, 0, 100, 0, 0, 0, 0, 0, 0, 11, 26660, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Magister Duskwither - Linked with Previous Event  - Cast Duskwither''s Fireball');

DELETE FROM `creature_text` WHERE `entry` = 15951;
INSERT INTO `creature_text` (`entry`, `groupid`, `id`, `text`, `type`, `language`, `probability`, `emote`, `duration`, `sound`, `comment`) VALUES
(15951, 0, 0, 'Good riddance... now none shall be able to repeat my mistakes!', 12, 0, 100, 0, 0, 0, 'Magister Duskwither - line 0');
