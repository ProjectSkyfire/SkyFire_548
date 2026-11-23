-- Shadra (43007) stood in the world all the time, but the quests "Summoning Shadra" (26558 and 26531) say she emerges
-- once the idol has been used at the three temples of Shadra'Alor, and the quests "Shadra the Venom Queen" (26419 and
-- 26532) are taken afterwards at the lookouts, 75 to 80 yards from her lair. She is now summoned by the altar bunnies
-- (43067, 43068, 43069) when the Idol of Shadra gives the credit of the third temple, at the spot of the old spawn
-- (-348.934, -2890.28, 65.4504), for 10 minutes unless she is fought, and only while none is alive within 300 yards.
-- The summon row runs before the credit row: it needs the other two temple objectives done and this one still open, so
-- using the idol again at a finished temple summons nothing.
--
-- Evidence for removing the static spawn (creature guid 8316830, entry 43007, added by 2026_11_21_world_hinterlands_quest_spawns):
-- the only spawn of the entry, so no duplicate to compare; it is replaced by the summon above (checked against the
-- Wowhead text of Summoning Shadra on 2026-11-22). The Legends of Azeroth 5.4.8 data has her static (unsniffed) and the
-- Cataclysm Preservation Project 4.3.4 does not spawn her at all. No addon, formation, pool, event, linked respawn,
-- SmartAI-by-guid or condition rows reference the guid.
--
-- The lookouts (43298 Wildhammer, 43299 Deathstalker) give the follow-up quests (26532 and 26419, "Kill Shadra"), and
-- summon her the same way when one is accepted, so a player who is slower than the timer, or who lost her, still finds
-- her. They get SmartAI only for this one row.
--
-- Her SmartAI rows (2026_11_22_97) never ran, because the template has an empty AIName; set it so the summoned Shadra casts them.

UPDATE `creature_template` SET `AIName` = 'SmartAI' WHERE `entry` IN (43007, 43298, 43299) AND `ScriptName` = '';

DELETE FROM `creature` WHERE `guid` = 8316830 AND `id` = 43007;

DELETE FROM `smart_scripts` WHERE `entryorguid` IN (43067, 43068, 43069, 43298, 43299) AND `source_type` = 0;
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `event_param5`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES
(43067, 0, 0, 0, 8, 0, 100, 0, 80469, 0, 0, 0, 0, 12, 43007, 1, 600000, 0, 0, 0, 8, 0, 0, 0, -348.934, -2890.28, 65.4504, 5.9096, 'Shadra''Alor Northwestern Altar Bunny - On SpellHit Idol of Shadra - Summon Shadra (third temple, none alive)'),
(43067, 0, 1, 0, 8, 0, 100, 0, 80469, 0, 0, 0, 0, 33, 43067, 0, 0, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0, 'Shadra''Alor Northwestern Altar Bunny - On SpellHit Idol of Shadra - Quest Credit'),
(43068, 0, 0, 0, 8, 0, 100, 0, 80469, 0, 0, 0, 0, 12, 43007, 1, 600000, 0, 0, 0, 8, 0, 0, 0, -348.934, -2890.28, 65.4504, 5.9096, 'Shadra''Alor Southwestern Altar Bunny - On SpellHit Idol of Shadra - Summon Shadra (third temple, none alive)'),
(43068, 0, 1, 0, 8, 0, 100, 0, 80469, 0, 0, 0, 0, 33, 43068, 0, 0, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0, 'Shadra''Alor Southwestern Altar Bunny - On SpellHit Idol of Shadra - Quest Credit'),
(43069, 0, 0, 0, 8, 0, 100, 0, 80469, 0, 0, 0, 0, 12, 43007, 1, 600000, 0, 0, 0, 8, 0, 0, 0, -348.934, -2890.28, 65.4504, 5.9096, 'Shadra''Alor Eastern Altar Bunny - On SpellHit Idol of Shadra - Summon Shadra (third temple, none alive)'),
(43069, 0, 1, 0, 8, 0, 100, 0, 80469, 0, 0, 0, 0, 33, 43069, 0, 0, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0, 'Shadra''Alor Eastern Altar Bunny - On SpellHit Idol of Shadra - Quest Credit'),
(43298, 0, 0, 0, 19, 0, 100, 0, 26532, 0, 0, 0, 0, 12, 43007, 1, 600000, 0, 0, 0, 8, 0, 0, 0, -348.934, -2890.28, 65.4504, 5.9096, 'Wildhammer Lookout - On Quest 26532 Accepted - Summon Shadra (none alive)'),
(43299, 0, 0, 0, 19, 0, 100, 0, 26419, 0, 0, 0, 0, 12, 43007, 1, 600000, 0, 0, 0, 8, 0, 0, 0, -348.934, -2890.28, 65.4504, 5.9096, 'Deathstalker Lookout - On Quest 26419 Accepted - Summon Shadra (none alive)');

DELETE FROM `conditions` WHERE `SourceTypeOrReferenceId` = 22 AND `SourceGroup` = 1 AND `SourceEntry` IN (43067, 43068, 43069, 43298, 43299) AND `SourceId` = 0;
INSERT INTO `conditions` (`SourceTypeOrReferenceId`, `SourceGroup`, `SourceEntry`, `SourceId`, `ElseGroup`, `ConditionTypeOrReference`, `ConditionTarget`, `ConditionValue1`, `ConditionValue2`, `ConditionValue3`, `NegativeCondition`, `ErrorType`, `ErrorTextId`, `ScriptName`, `Comment`) VALUES
(22, 1, 43067, 0, 0, 48, 0, 267007, 0, 1, 0, 0, 0, '', 'Summoning Shadra (26558) - the other temple objective 267007 is done'),
(22, 1, 43067, 0, 0, 48, 0, 267006, 0, 1, 0, 0, 0, '', 'Summoning Shadra (26558) - the other temple objective 267006 is done'),
(22, 1, 43067, 0, 0, 48, 0, 267005, 0, 0, 0, 0, 0, '', 'Summoning Shadra (26558) - this temple objective 267005 is not done yet'),
(22, 1, 43067, 0, 0, 29, 1, 43007, 300, 0, 1, 0, 0, '', 'Summoning Shadra (26558) - no living Shadra within 300 yards'),
(22, 1, 43067, 0, 1, 48, 0, 267084, 0, 1, 0, 0, 0, '', 'Summoning Shadra (26531) - the other temple objective 267084 is done'),
(22, 1, 43067, 0, 1, 48, 0, 267083, 0, 1, 0, 0, 0, '', 'Summoning Shadra (26531) - the other temple objective 267083 is done'),
(22, 1, 43067, 0, 1, 48, 0, 267082, 0, 0, 0, 0, 0, '', 'Summoning Shadra (26531) - this temple objective 267082 is not done yet'),
(22, 1, 43067, 0, 1, 29, 1, 43007, 300, 0, 1, 0, 0, '', 'Summoning Shadra (26531) - no living Shadra within 300 yards'),
(22, 1, 43068, 0, 0, 48, 0, 267005, 0, 1, 0, 0, 0, '', 'Summoning Shadra (26558) - the other temple objective 267005 is done'),
(22, 1, 43068, 0, 0, 48, 0, 267006, 0, 1, 0, 0, 0, '', 'Summoning Shadra (26558) - the other temple objective 267006 is done'),
(22, 1, 43068, 0, 0, 48, 0, 267007, 0, 0, 0, 0, 0, '', 'Summoning Shadra (26558) - this temple objective 267007 is not done yet'),
(22, 1, 43068, 0, 0, 29, 1, 43007, 300, 0, 1, 0, 0, '', 'Summoning Shadra (26558) - no living Shadra within 300 yards'),
(22, 1, 43068, 0, 1, 48, 0, 267082, 0, 1, 0, 0, 0, '', 'Summoning Shadra (26531) - the other temple objective 267082 is done'),
(22, 1, 43068, 0, 1, 48, 0, 267083, 0, 1, 0, 0, 0, '', 'Summoning Shadra (26531) - the other temple objective 267083 is done'),
(22, 1, 43068, 0, 1, 48, 0, 267084, 0, 0, 0, 0, 0, '', 'Summoning Shadra (26531) - this temple objective 267084 is not done yet'),
(22, 1, 43068, 0, 1, 29, 1, 43007, 300, 0, 1, 0, 0, '', 'Summoning Shadra (26531) - no living Shadra within 300 yards'),
(22, 1, 43069, 0, 0, 48, 0, 267005, 0, 1, 0, 0, 0, '', 'Summoning Shadra (26558) - the other temple objective 267005 is done'),
(22, 1, 43069, 0, 0, 48, 0, 267007, 0, 1, 0, 0, 0, '', 'Summoning Shadra (26558) - the other temple objective 267007 is done'),
(22, 1, 43069, 0, 0, 48, 0, 267006, 0, 0, 0, 0, 0, '', 'Summoning Shadra (26558) - this temple objective 267006 is not done yet'),
(22, 1, 43069, 0, 0, 29, 1, 43007, 300, 0, 1, 0, 0, '', 'Summoning Shadra (26558) - no living Shadra within 300 yards'),
(22, 1, 43069, 0, 1, 48, 0, 267082, 0, 1, 0, 0, 0, '', 'Summoning Shadra (26531) - the other temple objective 267082 is done'),
(22, 1, 43069, 0, 1, 48, 0, 267084, 0, 1, 0, 0, 0, '', 'Summoning Shadra (26531) - the other temple objective 267084 is done'),
(22, 1, 43069, 0, 1, 48, 0, 267083, 0, 0, 0, 0, 0, '', 'Summoning Shadra (26531) - this temple objective 267083 is not done yet'),
(22, 1, 43069, 0, 1, 29, 1, 43007, 300, 0, 1, 0, 0, '', 'Summoning Shadra (26531) - no living Shadra within 300 yards'),
(22, 1, 43298, 0, 0, 29, 1, 43007, 300, 0, 1, 0, 0, '', 'Shadra the Venom Queen (26532) - no living Shadra within 300 yards'),
(22, 1, 43299, 0, 0, 29, 1, 43007, 300, 0, 1, 0, 0, '', 'Shadra the Venom Queen (26419) - no living Shadra within 300 yards');
