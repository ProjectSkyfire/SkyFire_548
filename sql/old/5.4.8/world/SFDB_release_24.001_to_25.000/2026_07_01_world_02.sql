-- =============================================================================
-- DUSKWOOD: quest 26760 Cry For The Moon (Oliver Harris RP scene)
-- Duskwood chain part 3/3 (apply 2026-07-01_world_00 -> _01 -> _02 in order)
-- =============================================================================
-- Symptom:  Accepting the quest from Oliver Harris (43730) did nothing; scene
--           dialogue was out of order, spammed, or ended before quest credit.
-- Cause:    Missing event spawns, visibility spell_area rows, creature_text,
--           and SmartAI choreography; quest giver needs C++ scene controller.
-- Fix:      Full Raven Hill RP scene with TEXT_OVER dialogue chains, soft hide
--           cleanup (no FORCE_DESPAWN), and npc_oliver_harris C++ script.
-- NPCs:     43730 quest giver, 288 Master Harris (hidden during scene),
--           43858 RP Oliver, 43859 RP Jitters, 43950 RP Worgen (Sven),
--           43969 kill credit
-- Requires: zone_duskwood.cpp (npc_oliver_harris) — rebuild worldserver.
-- Apply:    .reload spell_area, .reload creature, .reload creature_template,
--           .reload smart_scripts, .reload creature_text
-- =============================================================================

-- -----------------------------------------------------------------------------
-- Visibility: player auras to see quest-invisible event NPCs during 26760
-- -----------------------------------------------------------------------------
DELETE FROM `spell_area`
WHERE `spell` IN (49416, 49417)
  AND `area` = 94
  AND `quest_start` = 26760;

INSERT INTO `spell_area`
    (`spell`, `area`, `quest_start`, `quest_end`, `aura_spell`, `racemask`, `gender`, `autocast`, `quest_start_status`, `quest_end_status`)
VALUES
    (49416, 94, 26760, 26760, 0, 0, 2, 1, 74, 9),
    (49417, 94, 26760, 0, 0, 0, 2, 1, 66, 11);

-- -----------------------------------------------------------------------------
-- Event spawns at Raven Hill (hidden until scene; short respawn for re-tests)
-- -----------------------------------------------------------------------------
DELETE FROM `creature` WHERE `guid` IN (571929, 571930, 571931);

INSERT INTO `creature`
    (`guid`, `id`, `map`, `spawnMask`, `phaseId`, `phaseGroup`, `modelid`, `equipment_id`,
     `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`,
     `currentwaypoint`, `curhealth`, `curmana`, `MovementType`, `npcflag`, `unit_flags`, `dynamicflags`)
VALUES
    (571930, 43858, 0, 1, 0, 0, 0, 0, -10754.7, 338.823, 37.3415, 3.56867, 15, 0, 0, 42, 0, 0, 0, 0, 0),
    (571929, 43859, 0, 1, 0, 0, 0, 0, -10767.1, 330.665, 37.6074, 5.31068, 15, 0, 0, 43, 0, 0, 0, 0, 0),
    (571931, 43950, 0, 1, 0, 0, 0, 0, -10747.4375, 331.902496, 37.739048, 4.55199, 15, 0, 0, 699, 0, 0, 0, 0, 0);

-- -----------------------------------------------------------------------------
-- Templates: C++ on QG Oliver; SmartAI on RP actors
-- -----------------------------------------------------------------------------
UPDATE `creature_template`
SET `AIName` = '', `ScriptName` = 'npc_oliver_harris'
WHERE `entry` = 43730;

UPDATE `creature_template`
SET `AIName` = 'SmartAI', `ScriptName` = ''
WHERE `entry` IN (43858, 43859, 43950);

-- RP worgen starts in stocks (aura 69196); RP actors have no quest invis addon.
DELETE FROM `creature_template_addon` WHERE `entry` IN (43858, 43859, 43950);
INSERT INTO `creature_template_addon` (`entry`, `path_id`, `mount`, `bytes1`, `bytes2`, `emote`, `auras`)
VALUES
    (43858, 0, 0, 0, 0, 0, NULL),
    (43859, 0, 0, 0, 0, 0, NULL),
    (43950, 0, 0, 0, 0, 0, '69196');

-- -----------------------------------------------------------------------------
-- Dialogue
-- -----------------------------------------------------------------------------
DELETE FROM `creature_text` WHERE `entry` IN (43858, 43859, 43950);
INSERT INTO `creature_text`
    (`entry`, `groupid`, `id`, `text`, `type`, `language`, `probability`, `emote`, `duration`, `sound`, `comment`)
VALUES
    (43858, 0, 0, 'Here we go...', 12, 0, 100, 396, 0, 0, 'Oliver Harris - quest 26760'),
    (43858, 1, 0, 'It''s working. Hold him still, Jitters.', 12, 0, 100, 396, 0, 0, 'Oliver Harris - quest 26760'),
    (43858, 2, 0, 'Damn it, Jitters, I said HOLD!', 12, 0, 100, 5, 0, 0, 'Oliver Harris - quest 26760'),
    (43858, 3, 0, 'Letting him go is the only thing that''s going to separate you from the beasts now, my friend.', 12, 0, 100, 0, 0, 0, 'Oliver Harris - quest 26760'),
    (43859, 0, 0, 'I... I can''t...', 12, 0, 100, 0, 0, 0, 'Jitters - quest 26760'),
    (43950, 0, 0, 'Jitters...', 12, 0, 100, 0, 0, 0, 'Sven Yorgen - quest 26760'),
    (43950, 1, 0, 'JITTERS!', 14, 0, 100, 0, 0, 0, 'Sven Yorgen - quest 26760'),
    (43950, 2, 0, 'You brought the worgen to Duskwood! You led the Dark Riders to my farm, and hid while they murdered my family!', 12, 0, 100, 0, 0, 0, 'Sven Yorgen - quest 26760'),
    (43950, 3, 0, 'Every speck of suffering in my life is YOUR PATHETIC FAULT! I SHOULD KILL YOU!', 14, 0, 100, 0, 0, 0, 'Sven Yorgen - quest 26760'),
    (43950, 4, 0, 'You''ve got a lot to make up for, Jitters. I won''t give you the easy way out.', 12, 0, 100, 0, 0, 0, 'Sven Yorgen - quest 26760'),
    (43950, 5, 0, 'I remember now... it''s all your fault!', 12, 0, 100, 0, 0, 0, 'Sven Yorgen - quest 26760');

-- -----------------------------------------------------------------------------
-- SmartAI: scene choreography (quest accept handled by C++ on 43730)
-- -----------------------------------------------------------------------------
DELETE FROM `smart_scripts`
WHERE `entryorguid` IN (43730, 43858, 43859, 43950) AND `source_type` = 0;

INSERT INTO `smart_scripts`
    (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`,
     `event_param1`, `event_param2`, `event_param3`, `event_param4`, `event_param5`,
     `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`,
     `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`)
VALUES
    -- 43858 RP Oliver
    (43858, 0, 0, 0, 37, 0, 100, 0, 0, 0, 0, 0, 0, 47, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'RP Oliver - On AI Init - Hide'),
    (43858, 0, 1, 0, 38, 0, 100, 0, 0, 7, 0, 0, 0, 47, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'RP Oliver - On Data 7 - Show'),
    (43858, 0, 2, 0, 38, 0, 100, 0, 0, 7, 0, 0, 0, 59, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'RP Oliver - On Data 7 - Walk'),
    (43858, 0, 3, 0, 38, 0, 100, 0, 0, 7, 0, 0, 0, 69, 1, 0, 0, 0, 0, 0, 8, 0, 0, 0, -10744.6, 332.517, 37.8412, 3.612242, 'RP Oliver - On Data 7 - Move to scene'),
    (43858, 0, 4, 0, 38, 0, 100, 0, 0, 7, 0, 0, 0, 22, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'RP Oliver - On Data 7 - Set phase 1'),
    (43858, 0, 5, 0, 1, 1, 100, 1, 5500, 5500, 0, 0, 0, 66, 0, 0, 0, 0, 0, 0, 8, 0, 0, 0, 0, 0, 0, 3.612242, 'RP Oliver - Face guillotine'),
    (43858, 0, 6, 0, 1, 1, 100, 1, 6000, 6000, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'RP Oliver - Here we go'),
    (43858, 0, 7, 0, 1, 1, 100, 1, 13000, 13000, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'RP Oliver - Hold him still'),
    (43858, 0, 8, 0, 1, 1, 100, 1, 20000, 20000, 0, 0, 0, 45, 0, 6, 0, 0, 0, 0, 19, 43859, 100, 0, 0, 0, 0, 0, 'RP Oliver - Trigger Jitters line'),
    (43858, 0, 9, 0, 1, 1, 100, 1, 20000, 20000, 0, 0, 0, 45, 0, 1, 0, 0, 0, 0, 19, 43950, 100, 0, 0, 0, 0, 0, 'RP Oliver - Trigger Worgen Jitters...'),
    (43858, 0, 10, 0, 1, 1, 100, 1, 32000, 32000, 0, 0, 0, 1, 2, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'RP Oliver - I said HOLD'),
    (43858, 0, 11, 0, 1, 1, 100, 1, 40000, 40000, 0, 0, 0, 45, 0, 2, 0, 0, 0, 0, 19, 43950, 100, 0, 0, 0, 0, 0, 'RP Oliver - Trigger Worgen JITTERS yell'),
    (43858, 0, 22, 0, 34, 0, 100, 1, 8, 2, 0, 0, 0, 47, 1, 0, 0, 0, 0, 0, 19, 43730, 100, 0, 0, 0, 0, 0, 'RP Oliver - Show Oliver QG when back'),
    (43858, 0, 23, 0, 34, 0, 100, 1, 8, 2, 0, 0, 0, 47, 1, 0, 0, 0, 0, 0, 19, 288, 100, 0, 0, 0, 0, 0, 'RP Oliver - Show Master Harris when back'),
    (43858, 0, 24, 0, 34, 0, 100, 1, 8, 2, 0, 0, 0, 45, 0, 8, 0, 0, 0, 0, 19, 43730, 100, 0, 0, 0, 0, 0, 'RP Oliver - Restore QG script state when back'),
    (43858, 0, 25, 0, 34, 0, 100, 1, 8, 2, 0, 0, 0, 47, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'RP Oliver - Hide when back (scene reset via C++)'),
    (43858, 0, 26, 0, 11, 0, 100, 0, 0, 0, 0, 0, 0, 47, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'RP Oliver - On Respawn - Hide'),
    (43858, 0, 27, 0, 38, 0, 100, 1, 0, 20, 0, 0, 0, 1, 3, 9000, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'RP Oliver - Letting him go...'),
    (43858, 0, 28, 0, 52, 0, 100, 1, 3, 43858, 0, 0, 0, 45, 0, 10, 0, 0, 0, 0, 19, 43859, 100, 0, 0, 0, 0, 0, 'RP Oliver - After Letting TEXT_OVER - Jitters run off'),
    (43858, 0, 29, 0, 52, 0, 100, 1, 3, 43858, 0, 0, 0, 45, 0, 6, 0, 0, 0, 0, 19, 43950, 100, 0, 0, 0, 0, 0, 'RP Oliver - After Letting TEXT_OVER - Sven final line'),
    (43858, 0, 30, 0, 38, 0, 100, 1, 0, 21, 0, 0, 0, 33, 43969, 0, 0, 0, 0, 0, 18, 100, 0, 0, 0, 0, 0, 0, 'RP Oliver - Quest credit'),
    (43858, 0, 31, 0, 38, 0, 100, 1, 0, 21, 0, 0, 0, 45, 0, 11, 0, 0, 0, 0, 19, 43950, 100, 0, 0, 0, 0, 0, 'RP Oliver - Worgen walk off'),
    (43858, 0, 32, 0, 38, 0, 100, 1, 0, 21, 0, 0, 0, 67, 2, 5000, 5000, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'RP Oliver - Delay walk back 5s'),
    (43858, 0, 33, 0, 59, 0, 100, 1, 2, 0, 0, 0, 0, 59, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'RP Oliver - Walk back to QG'),
    (43858, 0, 34, 0, 59, 0, 100, 1, 2, 0, 0, 0, 0, 69, 2, 0, 0, 0, 0, 0, 8, 0, 0, 0, -10754.7, 338.823, 37.3415, 3.56867, 'RP Oliver - Move back to QG spot'),

    -- 43859 RP Jitters
    (43859, 0, 0, 0, 37, 0, 100, 0, 0, 0, 0, 0, 0, 47, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'RP Jitters - On AI Init - Hide'),
    (43859, 0, 1, 0, 38, 0, 100, 0, 0, 7, 0, 0, 0, 47, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'RP Jitters - On Data 7 - Show'),
    (43859, 0, 2, 0, 38, 0, 100, 0, 0, 7, 0, 0, 0, 59, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'RP Jitters - On Data 7 - Run'),
    (43859, 0, 3, 0, 38, 0, 100, 0, 0, 7, 0, 0, 0, 69, 1, 0, 0, 0, 0, 0, 8, 0, 0, 0, -10749.8, 333.932, 37.3837, 5.31068, 'RP Jitters - On Data 7 - Move to scene'),
    (43859, 0, 4, 0, 38, 0, 100, 1, 0, 6, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'RP Jitters - I cannot'),
    (43859, 0, 5, 0, 38, 0, 100, 1, 0, 10, 0, 0, 0, 69, 2, 0, 0, 0, 0, 0, 8, 0, 0, 0, -10770.5, 327.6, 37.7, 5.6, 'RP Jitters - Run away'),
    (43859, 0, 6, 0, 34, 0, 100, 1, 8, 2, 0, 0, 0, 47, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'RP Jitters - Hide after run'),
    (43859, 0, 7, 0, 11, 0, 100, 0, 0, 0, 0, 0, 0, 47, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'RP Jitters - On Respawn - Hide'),

    -- 43950 RP Worgen (Sven)
    (43950, 0, 0, 0, 37, 0, 100, 0, 0, 0, 0, 0, 0, 47, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'RP Worgen - On AI Init - Hide'),
    (43950, 0, 1, 0, 38, 0, 100, 0, 0, 7, 0, 0, 0, 47, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'RP Worgen - On Data 7 - Show'),
    (43950, 0, 2, 0, 38, 0, 100, 0, 0, 7, 0, 0, 0, 17, 490, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'RP Worgen - InStocks emote state'),
    (43950, 0, 3, 0, 38, 0, 100, 1, 0, 1, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'RP Worgen - Jitters...'),
    (43950, 0, 4, 0, 38, 0, 100, 1, 0, 2, 0, 0, 0, 17, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'RP Worgen - Stand up'),
    (43950, 0, 5, 0, 38, 0, 100, 1, 0, 2, 0, 0, 0, 5, 15, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'RP Worgen - Roar emote'),
    (43950, 0, 6, 0, 38, 0, 100, 1, 0, 2, 0, 0, 0, 1, 1, 4000, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'RP Worgen - JITTERS! (TEXT_OVER to remember)'),
    (43950, 0, 7, 0, 38, 0, 100, 1, 0, 2, 0, 0, 0, 28, 69196, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'RP Worgen - Remove InStocks aura (69196)'),
    (43950, 0, 8, 0, 38, 0, 100, 1, 0, 3, 0, 0, 0, 1, 5, 4000, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'RP Worgen - remember now (TEXT_OVER to accusation)'),
    (43950, 0, 9, 0, 38, 0, 100, 1, 0, 4, 0, 0, 0, 1, 2, 9000, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'RP Worgen - accusation (wait for TEXT_OVER)'),
    (43950, 0, 10, 0, 52, 0, 100, 1, 2, 43950, 0, 0, 0, 1, 3, 7000, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'RP Worgen - Every speck yell (TEXT_OVER to Oliver Letting)'),
    (43950, 0, 11, 0, 38, 0, 100, 1, 0, 11, 0, 0, 0, 59, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'RP Worgen - Walk'),
    (43950, 0, 12, 0, 38, 0, 100, 1, 0, 11, 0, 0, 0, 69, 2, 0, 0, 0, 0, 0, 8, 0, 0, 0, -10763.819336, 339.096375, 38.501102, 6.0, 'RP Worgen - Walk to final coord'),
    (43950, 0, 13, 0, 34, 0, 100, 1, 8, 2, 0, 0, 0, 47, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'RP Worgen - Hide at final point'),
    (43950, 0, 14, 0, 38, 0, 100, 1, 0, 6, 0, 0, 0, 1, 4, 6000, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'RP Worgen - easy way out (TEXT_OVER to quest credit)'),
    (43950, 0, 15, 0, 11, 0, 100, 0, 0, 0, 0, 0, 0, 47, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'RP Worgen - On Respawn - Hide'),
    (43950, 0, 16, 0, 52, 0, 100, 1, 1, 43950, 0, 0, 0, 45, 0, 3, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'RP Worgen - After JITTERS TEXT_OVER - remember now'),
    (43950, 0, 17, 0, 38, 0, 100, 1, 0, 2, 0, 0, 0, 66, 0, 0, 0, 0, 0, 0, 19, 43859, 30, 0, 0, 0, 0, 0, 'RP Worgen - Face Jitters'),
    (43950, 0, 18, 0, 52, 0, 100, 1, 5, 43950, 0, 0, 0, 45, 0, 4, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'RP Worgen - After remember TEXT_OVER - accusation'),
    (43950, 0, 20, 0, 52, 0, 100, 1, 3, 43950, 0, 0, 0, 45, 0, 20, 0, 0, 0, 0, 19, 43858, 100, 0, 0, 0, 0, 0, 'RP Worgen - After Every speck TEXT_OVER - Oliver Letting'),
    (43950, 0, 21, 0, 52, 0, 100, 1, 4, 43950, 0, 0, 0, 45, 0, 21, 0, 0, 0, 0, 19, 43858, 100, 0, 0, 0, 0, 0, 'RP Worgen - After final line TEXT_OVER - quest credit');
