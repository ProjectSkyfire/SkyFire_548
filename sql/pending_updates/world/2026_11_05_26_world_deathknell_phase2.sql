-- Deathknell (Undead starting zone): second pass over the quest flow. Two fixes: the Monk chain could not reach
-- "Novice Elreth", and the kill targets of "Assault on the Rotbrain Encampment" had no combat AI.
-- Data is the 5.4.8 data (the Monk class and the Rotbrain camp AI exist only there). No new spawns, so no new guids.

-- ---------------------------------------------------------------------------------------------------------------
-- "Tiger Palm" (31147, Monk) pointed at the Monk copy of "Novice Elreth" (31148), which has no giver and no
-- turn-in in any data set, and "Novice Elreth" (28651) only follows the other classes' quests (24964 to 24969). A Monk
-- could therefore never take 28651 and stopped before "The Truth of the Grave" (24961) and everything after it.
-- 5.4.8 links 31147 to 28651 instead. 31148 stays without a giver, as in every data set.
-- ---------------------------------------------------------------------------------------------------------------
UPDATE `quest_template` SET `NextQuestId` = 28651 WHERE `Id` = 31147;

-- ---------------------------------------------------------------------------------------------------------------
-- Rotbrain Berserker (49422, kill objective of "Assault on the Rotbrain Encampment" 24971), Marshal Redpath
-- (49424, kill objective of 24971) and Rotbrain Magus (49423, the camp's casters) had no AI, so the elite fight was
-- plain melee with no enrage, slam, leap, whirlwind or fire casts. Rows and lines are the 5.4.8 ones, with two changes:
-- the "start attack on Deathguard Protector (49428)" rows are left out because the protectors are not spawned, and the
-- Fireball cast flag 64 is 0 (the flag is not in this core).
-- ---------------------------------------------------------------------------------------------------------------
UPDATE `creature_template` SET `AIName` = 'SmartAI' WHERE `entry` IN (49422, 49423, 49424) AND `ScriptName` = '';

DELETE FROM `smart_scripts` WHERE `entryorguid` IN (49422, 49423, 49424) AND `source_type` = 0;
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `event_param5`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES
(49422, 0, 0, 0, 4, 0, 100, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Rotbrain Berserker - On Aggro - Say Line 0 (No Repeat)'),
(49422, 0, 1, 2, 2, 0, 100, 1, 0, 30, 0, 0, 0, 11, 63227, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Rotbrain Berserker - Between 0-30% Health - Cast ''Enrage'' (No Repeat)'),
(49422, 0, 2, 0, 61, 0, 100, 1, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Rotbrain Berserker - Between 0-30% Health (Link) - Say Line 1 (No Repeat)'),
(49422, 0, 3, 0, 0, 0, 100, 0, 4000, 7000, 15000, 18000, 0, 11, 79881, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 'Rotbrain Berserker - In Combat - Cast ''Slam'''),
(49423, 0, 0, 0, 4, 0, 100, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Rotbrain Magus - On Aggro - Say Line 0 (No Repeat)'),
(49423, 0, 1, 0, 0, 0, 100, 0, 0, 0, 3400, 4700, 0, 11, 9053, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 'Rotbrain Magus - In Combat - Cast ''Fireball'''),
(49423, 0, 2, 0, 0, 0, 100, 0, 7000, 9000, 18000, 25000, 0, 11, 11962, 1, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 'Rotbrain Magus - In Combat - Cast ''Immolate'''),
(49424, 0, 0, 1, 4, 0, 100, 1, 0, 0, 0, 0, 0, 11, 83015, 0, 0, 0, 0, 0, 21, 25, 0, 0, 0, 0, 0, 0, 'Marshal Redpath - On Aggro - Cast ''Heroic Leap'' (No Repeat)'),
(49424, 0, 1, 0, 61, 0, 100, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Marshal Redpath - On Aggro (Link) - Say Line 0 (No Repeat)'),
(49424, 0, 2, 0, 0, 0, 100, 1, 5000, 9000, 6000, 10000, 0, 11, 33239, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 'Marshal Redpath - In Combat - Cast ''Whirlwind''');

-- Their lines (5.4.8 creature_text, ids and probabilities as there).
DELETE FROM `creature_text` WHERE `entry` IN (49422, 49423, 49424);
INSERT INTO `creature_text` (`entry`, `groupid`, `id`, `text`, `type`, `language`, `probability`, `emote`, `duration`, `sound`, `comment`) VALUES
(49422, 0, 0, 'I''ll kill you!', 12, 0, 15, 0, 0, 0, 'Rotbrain Berserker'),
(49422, 0, 1, 'Look what they''ve done to me!', 12, 0, 15, 0, 0, 0, 'Rotbrain Berserker'),
(49422, 0, 2, 'Who am I?', 12, 0, 15, 0, 0, 0, 'Rotbrain Berserker'),
(49422, 1, 0, '%s becomes enraged!', 16, 0, 100, 0, 0, 0, 'Rotbrain Berserker'),
(49423, 0, 0, 'I''ll kill you!', 12, 0, 15, 0, 0, 0, 'Rotbrain Magus'),
(49423, 0, 1, 'Look what they''ve done to me!', 12, 0, 15, 0, 0, 0, 'Rotbrain Magus'),
(49423, 0, 2, 'Who am I?', 12, 0, 15, 0, 0, 0, 'Rotbrain Magus'),
(49424, 0, 0, 'BLEEAAAGGHHH! I''m a monster, don''t look at me!', 14, 0, 100, 0, 0, 0, 'Marshal Redpath');
