-- Quest 25027: Bramblestaff (52073 / spell 73141) on Three-Tooth.
-- 73141 is a dummy with no server handler, so C++ spell_q25027_bramblestaff gives the credit and fires
-- DoAction 1 into the SAI below. Phase 1 = idle ritual, 3 = greeted, 2 = bramblestaff RP.
--
-- Notes on things that are easy to get wrong here:
--  * She is left as a normal mob: attackable, free to chase, fights back. The staff is only an alternative
--    to killing her, and the RP works wherever she is - only the bunny/crone visuals need her spawn area.
--  * Phases are set from SMART_EVENT_RESET, not RESPAWN - OnReset() calls SetPhase(0) after RESPAWN fires.
--  * Never SET_ROOT her: a rooted creature never finishes MoveTargetedHome(), so it sticks in evade mode
--    and Creature::Update() then skips UpdateAI() entirely, freezing the whole script.
--  * CALL_TIMED_ACTIONLIST timerType is action_param2 (struct is just { id, timerType }); 2 = tick in and
--    out of combat. Left at 0 the RP loads on cast and sits frozen until she drops combat.
--  * The two ritual auras are permanent, so casting them OOC is enough - they stay on through the fight
--    and are only stripped on evade, which re-applies them.
--  * Bunny 38821 had modelid1 328 (CREATURE\RABBIT) alongside 21072 (INVISIBLESTALKER), and
--    GetRandomValidModelId() picks at random, so the effect trigger was visible half the time.
--  * Open-world linked_respawn is instance-only, so Three-Tooth's On Respawn forces both crone guids up
--    with her. Their own 120s timers are left alone so killing only the crones still recovers normally.

UPDATE `creature_template` SET `modelid1`=21072, `modelid2`=0 WHERE `entry`=38821;
UPDATE `creature_template` SET `AIName`='SmartAI', `ScriptName`='', `unit_flags`=64 WHERE `entry`=38941;
UPDATE `creature_template` SET `AIName`='SmartAI', `ScriptName`='', `unit_flags`=33536, `VehicleId`=588 WHERE `entry`=38942;

DELETE FROM `spell_script_names` WHERE `spell_id`=73141;
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES
(73141, 'spell_q25027_bramblestaff');

DELETE FROM `creature_text` WHERE `entry`=38941;
INSERT INTO `creature_text` (`entry`, `groupid`, `id`, `text`, `type`, `language`, `probability`, `emote`, `duration`, `sound`, `comment`) VALUES
(38941, 0, 0, 'They come... <Snort> Pretty $r, yes? So pretty...', 12, 0, 100, 0, 0, 0, 'Three-Tooth - On Player Approach'),
(38941, 1, 0, 'EeeKKeech - it stings! It crushes! $GHe:She; tricked us, kills $Ghim:her;, kill!', 14, 0, 100, 0, 0, 0, 'Three-Tooth - Bramblestaff');

DELETE FROM `smart_scripts` WHERE `entryorguid` IN (38941, 38942) AND `source_type`=0;
DELETE FROM `smart_scripts` WHERE `entryorguid`=3894100 AND `source_type`=9;
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES
-- Three-Tooth 38941
(38941, 0, 0, 0, 25, 0, 100, 0, 0, 0, 0, 0, 22, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Three-Tooth - On Reset - Set Phase 1'),
(38941, 0, 1, 0, 1, 5, 100, 0, 1000, 1000, 5000, 5000, 11, 28892, 32, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Three-Tooth - OOC - Channel Nature Channeling on Self'),
(38941, 0, 2, 0, 1, 5, 100, 0, 2000, 2000, 5000, 5000, 11, 62923, 34, 0, 0, 0, 0, 19, 38821, 10, 0, 0, 0, 0, 0, 'Three-Tooth - OOC - Entangling Roots Visual on Ritual Bunny'),
(38941, 0, 3, 4, 10, 1, 100, 0, 0, 20, 60000, 60000, 1, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Three-Tooth - OOC LoS - Say Line 0'),
(38941, 0, 4, 0, 61, 0, 100, 0, 0, 0, 0, 0, 22, 3, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Three-Tooth - Linked - Set Phase 3 (greeted)'),
(38941, 0, 5, 6, 4, 1, 100, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Three-Tooth - On Aggro - Say Line 0'),
(38941, 0, 6, 0, 61, 0, 100, 0, 0, 0, 0, 0, 22, 3, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Three-Tooth - Linked - Set Phase 3 (greeted)'),
(38941, 0, 7, 8, 72, 0, 100, 1, 1, 0, 0, 0, 22, 2, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Three-Tooth - On DoAction 1 (Bramblestaff) - Set Phase 2'),
(38941, 0, 8, 0, 61, 0, 100, 0, 0, 0, 0, 0, 80, 3894100, 2, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Three-Tooth - Linked - Run Vine Grab Script (timerType 2 = tick in or out of combat)'),
(38941, 0, 9, 10, 11, 0, 100, 0, 0, 0, 0, 0, 70, 0, 0, 0, 0, 0, 0, 10, 153226, 38940, 0, 0, 0, 0, 0, 'Three-Tooth - On Respawn - Respawn Deviate Crone 153226'),
(38941, 0, 10, 0, 61, 0, 100, 0, 0, 0, 0, 0, 70, 0, 0, 0, 0, 0, 0, 10, 153229, 38940, 0, 0, 0, 0, 0, 'Three-Tooth - Linked - Respawn Deviate Crone 153229'),
-- Merciless Jungle Vine 38942
(38942, 0, 0, 1, 54, 0, 100, 0, 0, 0, 0, 0, 11, 68295, 2, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Merciless Jungle Vine - Just Summoned - Cast Entangling Roots Visual'),
(38942, 0, 1, 0, 61, 0, 100, 0, 0, 0, 0, 0, 41, 10000, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Merciless Jungle Vine - Linked - Despawn in 10s (releases passenger)');

INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES
(3894100, 9, 0, 0, 0, 0, 100, 0, 0, 0, 0, 0, 22, 2, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Three-Tooth - Script - Set Phase 2 (stop idle RP)'),
(3894100, 9, 1, 0, 0, 0, 100, 0, 0, 0, 0, 0, 28, 62923, 0, 0, 0, 0, 0, 19, 38821, 10, 0, 0, 0, 0, 0, 'Three-Tooth - Script - Remove Roots Visual from Ritual Bunny'),
(3894100, 9, 2, 0, 0, 0, 100, 0, 0, 0, 0, 0, 28, 28892, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Three-Tooth - Script - Stop Nature Channeling'),
(3894100, 9, 3, 0, 0, 0, 100, 0, 0, 0, 0, 0, 18, 768, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Three-Tooth - Script - Set Immune to PC/NPC'),
(3894100, 9, 4, 0, 0, 0, 100, 0, 0, 0, 0, 0, 11, 73136, 2, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Three-Tooth - Script - Cast Summon Vine with Ground Burst'),
(3894100, 9, 5, 0, 0, 0, 100, 0, 600, 600, 0, 0, 11, 46598, 3, 0, 0, 0, 0, 19, 38942, 15, 0, 0, 0, 0, 0, 'Three-Tooth - Script - Ride Vine Grab Seat'),
(3894100, 9, 6, 0, 0, 0, 100, 0, 600, 600, 0, 0, 1, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Three-Tooth - Script - Yell Line 1 While Flailed'),
(3894100, 9, 7, 0, 0, 0, 100, 0, 0, 0, 0, 0, 11, 62923, 2, 0, 0, 0, 0, 11, 38940, 20, 0, 0, 0, 0, 0, 'Three-Tooth - Script - Entangling Roots Visual on Deviate Crones'),
(3894100, 9, 8, 0, 0, 0, 100, 0, 0, 0, 0, 0, 11, 65918, 2, 0, 0, 0, 0, 11, 38940, 20, 0, 0, 0, 0, 0, 'Three-Tooth - Script - Stun Deviate Crones (65918, 10s)'),
(3894100, 9, 9, 0, 0, 0, 100, 0, 8000, 8000, 0, 0, 28, 62923, 0, 0, 0, 0, 0, 11, 38940, 20, 0, 0, 0, 0, 0, 'Three-Tooth - Script - Remove Roots Visual from Crones'),
(3894100, 9, 10, 0, 0, 0, 100, 0, 0, 0, 0, 0, 28, 65918, 0, 0, 0, 0, 0, 11, 38940, 20, 0, 0, 0, 0, 0, 'Three-Tooth - Script - Remove Stun from Crones'),
(3894100, 9, 11, 0, 0, 0, 100, 0, 1300, 1300, 0, 0, 37, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Three-Tooth - Script - Die After Vine Releases');

-- Type 13 is rejected for this spell (TargetA is not AREA/CONE/NEARBY). Quest-taken is enough; the C++ script checks the NPC entry.
DELETE FROM `conditions` WHERE `SourceTypeOrReferenceId` IN (13, 17) AND `SourceEntry`=73141;
INSERT INTO `conditions` (`SourceTypeOrReferenceId`, `SourceGroup`, `SourceEntry`, `SourceId`, `ElseGroup`, `ConditionTypeOrReference`, `ConditionTarget`, `ConditionValue1`, `ConditionValue2`, `ConditionValue3`, `NegativeCondition`, `ErrorType`, `ErrorTextId`, `ScriptName`, `Comment`) VALUES
(17, 0, 73141, 0, 0, 9, 0, 25027, 0, 0, 0, 0, 0, '', 'Bramblestaff - requires quest 25027 taken');
